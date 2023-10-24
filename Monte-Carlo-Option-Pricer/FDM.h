#pragma once
#include <vector>
#include <iostream>
#include "SDE.h"

template <typename Sde>
class EulerFDM
{
private:
	std::shared_ptr<Sde> sde;
	int NT;

public:
	std::vector<double> x;
	double k;

	double dtSqrt;

public:
	EulerFDM();
	EulerFDM(const std::shared_ptr<Sde>& stochasticEquation, int numSubDivisions) : sde(stochasticEquation), NT(numSubDivisions)
	{
		NT = numSubDivisions;
		k = sde->Expiry() / static_cast<double>(NT);
		dtSqrt = std::sqrt(k);
		x = std::vector<double>(NT + 1);

		//create the mesh array 
		x[0] = 0.0;
		for (std::size_t n = 1; n < x.size(); ++n)
		{
			x[n] = x[n - 1] + k;
		}
	}

	double advance(double xn, double tn, double dt, double normalVar, double normalVar2) const
	{
		return xn + sde->Drift(xn, tn) * dt + sde->Diffusion(xn, tn) * dtSqrt * normalVar;
	}


};

