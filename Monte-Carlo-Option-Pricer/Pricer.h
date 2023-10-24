#pragma once
#include <iostream>
#include <vector>
#include <functional>


class Pricer
{
public:
	//Create a single path 
	virtual void ProcessPath(const std::vector<double>& arr) = 0;

	//Notify end of simulation
	virtual void PostProcess() = 0;

	//Discounting, should be delegated/signal
	virtual double DiscountFactor() = 0;

	//Option price
	virtual double Price() = 0;

	std::function<double(double)> m_payoff;
	std::function<double()> m_discounter;

	Pricer() = default;
	Pricer(const std::function<double(double)>& payOff, const std::function<double()>& discounter)
	{
		m_payoff = payOff;
		m_discounter = discounter;

	}

};


class EuropeanPricer : public Pricer
{
private:
	double price;
	double sum;
	int NSim;


public:
	EuropeanPricer();
	EuropeanPricer(const std::function<double(double)>& payOff, const std::function<double()>& discounter) : Pricer(payOff, discounter)
	{
		price = 0.0;
		sum = 0.0;
		NSim = 0;
	}

	void ProcessPath(const std::vector<double>& arr) override
	{//A path for each simulation/draw

		//Sum of option values at terminal time T
		sum += m_payoff(arr[arr.size() - 1]); NSim++;
	}

	double DiscountFactor() override
	{//Discounting
		return m_discounter();
	}

	void PostProcess() override
	{
		price = DiscountFactor() * sum / NSim;
	}

	double Price() override
	{
		return price;
	}

};
