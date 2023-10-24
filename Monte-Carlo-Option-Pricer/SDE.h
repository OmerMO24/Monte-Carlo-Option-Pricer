#pragma once
#include <iostream>

//Instance Systems
class GBM
{
private:
	double mu; //drift
	double vol; //constant volatility 
	double d; //constant dividend yield
	double ic; //Initial condition 
	double exp; //Expiry 

public:
	GBM();
	GBM(double driftCoefficient, double difussionCoefficient, double dividendYield, double initialCondition, double expiry)
	{
		mu = driftCoefficient;
		vol = difussionCoefficient;
		d = dividendYield;
		ic = initialCondition;
		exp = expiry;
	}

	double Drift(double x, double t) { return (mu - d) * x; }

	double Diffusion(double x, double t) { return vol * x; }

	double DriftCorrected(double x, double t, double B)
	{
		return Drift(x, t) - B * Diffusion(x, t) * DiffusionDerivative(x, t);
	}

	double DiffusionDerivative(double x, double t)
	{
		return vol;
	}


	double InitialCondition() const
	{
		return ic;
	}

	double Expiry() const
	{
		return exp;
	}

};
