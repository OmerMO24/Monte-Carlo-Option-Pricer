

#include "SDE.h"
#include "Mediator.h"
#include "Pricer.h"
#include "RNG.h"
#include "FDM.h"


int main()
{
	int Nsim = 1'000'000;
	int NT = 500;

	double driftCoefficient = 0.08;
	double diffusionCoefficient = 0.3;
	double dividentYield = 0.0;
	double initialCondition = 60.0;
	double expiry = 0.25;

	auto sde = std::shared_ptr<GBM>(new GBM(driftCoefficient, diffusionCoefficient, dividentYield, initialCondition, expiry));

	double K = 65.0;

	//Factories for object in context diagram
	std::function<double(double)> payoffPut = [&K](double x) {return std::max<double>(0.0, K - x);  };

	std::function<double(double)> payoffCall = [&K](double x) {return std::max<double>(0.0, x - K);  };

	double r = 0.08; double T = 0.25;
	std::function<double()> discounter = [&r, &T]() {return std::exp(-r * T); };

	auto pricerCall = std::shared_ptr<Pricer>(new EuropeanPricer(payoffCall, discounter));
	auto pricerPut = std::shared_ptr<Pricer>(new EuropeanPricer(payoffPut, discounter));

	auto fdm = std::shared_ptr<EulerFDM<GBM>>(new EulerFDM<GBM>(sde, NT));

	auto rngPM = std::shared_ptr<Rng>(new PolarMarsaglia());
	auto rngCPP = std::shared_ptr<Rng>(new CPPRNG());


	SUD<GBM, EuropeanPricer, EulerFDM<GBM>, CPPRNG>
		s(sde, pricerPut, fdm, rngCPP, Nsim, NT);

	std::cout << "\n C++11 RNG: " << pricerPut->Price() << '\n';
	std::cout << "Elapsed time in seconds: " << "NA" << '\n';

	SUD<GBM, EuropeanPricer, EulerFDM<GBM>, CPPRNG>
		s2(sde, pricerPut, fdm, rngPM, Nsim, NT);

	std::cout << "Polar Marsaglia: " << pricerCall->Price();
	std::cout << "Elapsed time in seconds: " << "NA" << '\n';


	return 0;



}

