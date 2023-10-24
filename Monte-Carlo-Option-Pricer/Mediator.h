#pragma once
#include <iostream>
#include <vector>

template <typename Sde, typename Pricer, typename Fdm, typename Rng>
class SUD : private Sde, private Pricer, private Fdm, private Rng
{
private:
	//Four main components
	std::shared_ptr<Sde> sde;
	std::shared_ptr<Fdm> fdm;
	std::shared_ptr<Rng> rng;
	std::shared_ptr<Pricer> pricer;

private:
	//Other Monte Carlo related data

	int NSim;
	std::vector<double> res;


public:
	SUD() {}
	SUD(const std::shared_ptr<Sde>& s, const std::shared_ptr<Pricer>& p, const std::shared_ptr<Fdm>& f, const std::shared_ptr<Rng>& r, int numberOfSimulations, int NT) : sde(s), pricer(p), fdm(f), rng(r)
	{
		NSim = numberOfSimulations;
		res = std::vector<double>(NT + 1);
	}

	void start()
	{
		//Main loop for path generation

		double VOld, VNew;

		for (int i = 1; i <= NSim; ++i)
		{//Calculate a path at each iteration

			if ((i / 100000) * 100000 == i)
			{//Give status after a certain amount of iterations

				std::cout << i << ",";
			}

			VOld = sde->InitialCondition(); res[0] = VOld;

			for (std::size_t n = 1; n < res.size(); n++)
			{//Computed the soltuion at level n+1
				VNew = fdm->advance(VOld, fdm->advance[n - 1], fdm->k, rng->GenerateRn(), rng->GenerateRN());
				res[n] = VNew;
				VOld = VNew;
			}

			//Send this path data to the Pricer(s)
			pricer->ProcessPath(res);
			pricer->PostProcess();



		}


	}

};
