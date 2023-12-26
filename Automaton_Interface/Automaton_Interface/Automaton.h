#pragma once
#include <vector>
#include <iostream>
#include <unordered_set>

//Automaton Interface
class Automaton
{
public:
	virtual ~Automaton() = default;
	virtual void printAutomaton(std::ofstream& file) = 0;
	virtual void readAutomaton(std::ifstream& file) = 0;
	virtual bool verifyAutomaton() = 0;
	virtual bool checkWord(const std::unordered_set<int>& currentStates, const std::string& word, int currentIndex) = 0;
	virtual bool checkWordLambda(const std::string& word)=0;
	virtual void addState(int stateValue) = 0; 
	virtual void addTransition(int firstState, char symbol, int secondState) = 0;
	virtual void addSymbolToAlphabet(char symbol) = 0;
	virtual void addFinalState(int state) = 0;

	virtual void setSizeQ(const uint16_t& size) = 0;
	virtual std::vector<int> getQ() const = 0;
	//adauga toti get/set (orice functie pe care trebuie sa o accesez in mainWindow)

	uint16_t getq0() const 
	{
		return m_q0;
	}
protected:
	std::vector<uint32_t> m_Q;
	std::vector<char> m_Sum;
	uint32_t m_q0;
	std::vector<std::tuple<char,int,char>> m_Delta; //maybe class for prods
	std::vector<uint32_t> m_F;
};

