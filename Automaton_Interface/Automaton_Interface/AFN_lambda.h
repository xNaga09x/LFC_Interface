#pragma once
#include "Automaton.h"
#include<iostream>
#include<string>
#include<vector>
#include<tuple>
#include<algorithm>
#include<unordered_set>
#include<fstream>
class AFN_lambda : public Automaton
{
public:
	AFN_lambda();
	AFN_lambda(const uint16_t& sizeQ, const uint16_t& sizeSum, const uint16_t& sizeDelta, const uint16_t& sizeF, const std::vector<int>& Q,
		const std::vector<char>& Sum, const std::vector<std::tuple<int, char, int>>& Delta, uint16_t q0, const std::vector<int>& F);

	void setSizeQ(const uint16_t& sizeQ);
	void setSizeSum(const uint16_t& sizeSum);
	void setSizeDelta(const uint16_t& sizeDelta);
	void setSizeF(const uint16_t& sizeF);
	void setQ(const std::vector<int>& Q);
	void setSum(const std::vector<char>& Sum);
	void setDelta(const std::vector<std::tuple<int, char, int>>& Delta);
	void setq0(uint16_t q0);
	void setF(const std::vector<int>& F);

	uint16_t getSizeQ() const;
	uint16_t getSizeSum() const;
	uint16_t getSizeDelta() const;
	uint16_t getSizeF() const;
	std::vector<int> getQ() const;
	std::vector<char> getSum() const;
	std::vector<std::tuple<int, char, int>> getDelta() const;
	uint16_t getq0() const;
	std::vector<int> getF() const;

	bool verifyAutomaton();

	void printQ() const;
	void printSum() const;
	void printDelta() const;
	void printF() const;
	void printAutomaton() const;

	void addState(int state);
	void addTransition(int firstState, char symbol, int secondState);
	void addSymbolToAlphabet(char symbol);
	void addFinalState(int state);

	std::unordered_set<int> lambdaClosure(int state) const;

	bool checkWord(const std::unordered_set<int>& currentStates, const std::string& word, int currentIndex) override
	{
		return false;
	}

	bool checkWordLambda(const std::string& word);

	void readAutomaton(std::ifstream& file);

	void printAutomaton(std::ofstream& file);

private:
	uint16_t m_sizeQ;
	uint16_t m_sizeSum;
	uint16_t m_sizeDelta;
	uint16_t m_sizeF;
	std::vector<int> m_Q;
	std::vector<char> m_Sum;
	std::vector<std::tuple<int, char, int>> m_Delta;
	uint16_t m_q0;
	std::vector<int> m_F;
};

