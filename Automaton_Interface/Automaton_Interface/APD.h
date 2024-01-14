#pragma once
#include <iostream>
#include "Prod.h"
#include <stack>
#include <fstream>

class APD
{
public:
	APD();
	APD(const uint16_t& sizeQ, const uint16_t& sizeSum, const uint16_t& sizeDelta, const uint16_t& sizeF, const std::vector<int>& Q,
		const std::vector<char>& Sum, const std::vector<Prod>& Delta, uint16_t q0, const std::vector<int>& F);

	void setSizeQ(const uint16_t& sizeQ);
	void setSizeSum(const uint16_t& sizeSum);
	void setSizeDelta(const uint16_t& sizeDelta);
	void setSizeF(const uint16_t& sizeF);
	void setQ(const std::vector<int>& Q);
	void setSum(const std::vector<char>& Sum);
	void setDelta(const std::vector<Prod>& Delta);
	void setq0(uint16_t q0);
	void setF(const std::vector<int>& F);
	void setSizeGamma(const uint16_t& sizeGamma);
	void setGamma(const std::vector<char>& Gamma);
	void setZ0(const char& Z0);

	uint16_t getSizeQ() const;
	uint16_t getSizeSum() const;
	uint16_t getSizeDelta() const;
	uint16_t getSizeF() const;
	uint16_t getSizeGamma() const;
	uint16_t getq0() const;
	char getZ0()const;
	std::vector<int> getQ() const;
	std::vector<char> getSum() const;
	std::vector<Prod> getDelta() const;
	std::vector<int> getF() const;
	std::vector<char> getGamma() const;

	void printQ() const;
	void printSum() const;
	void printDelta() const;
	void printF() const;
	void printGamma() const;
	void printAutomaton() const;

	void addState(int state);
	void addTransition(Prod transition);
	void addSymbol(char symbol);
	void addStackSymbol(char symbol);
	void addFinalState(int state);
	void addInitialState(int state);

	bool checkWord(uint32_t currentState, std::stack<char>& stack, std::string word);
	std::vector<std::pair<int, std::string>> findProdResult(std::tuple<int, char, char> prodInput);
	bool verifyAutomaton();

	void readAutomaton(std::ifstream& file);
	void printAutomaton(std::ofstream& file);
private:
	uint16_t m_sizeQ;
	uint16_t m_sizeSum;
	uint16_t m_sizeDelta;
	uint16_t m_sizeF;
	uint16_t m_sizeGamma;
	std::vector<int> m_Q;
	std::vector<char> m_Sum;
	std::vector<Prod> m_Delta;
	std::vector<char> m_Gamma;
	std::vector<char> m_Stack;
	char m_Z0;
	uint16_t m_q0;
	std::vector<int> m_F;
};

