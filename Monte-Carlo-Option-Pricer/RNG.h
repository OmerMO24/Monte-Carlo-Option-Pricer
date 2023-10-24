#pragma once
#include <random>

class Rng
{
public:
	virtual double GenerateRn() = 0;

};

class PolarMarsaglia : public Rng
{//Only for educational purposes

private:
	std::default_random_engine dre;
	std::uniform_real_distribution<double> unif;


public:
	PolarMarsaglia() : dre(std::default_random_engine()), unif(std::uniform_real_distribution<double>(0.0, 1.0)) {}

	double GenerateRn() override
	{
		double u, v, S;

		do
		{
			u = 2.0 * unif(dre) - 1.0;
			v = 2.0 * unif(dre) - 1.0;
			S = u * u + v * v;
		} while (S > 1.0 || S <= 0.0);

		double fac = std::sqrtf(-2.0 * std::log(S) / S);
		return u * fac;
	}

};

class CPPRNG : public Rng
{
private:
	//Normal (0,1) rng
	std::default_random_engine dre;
	std::normal_distribution<double> norm;

public:
	CPPRNG() : dre(std::default_random_engine()), norm(std::normal_distribution<double>(0.0, 1.0)) {}

	double GenerateRn() override
	{
		return norm(dre);
	}
};
