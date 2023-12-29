#pragma once
#include <iostream>
#include <vector>
#include <string>

class Prod
{
public:
	Prod();
	Prod(const int& overStackSize, const int& initState, const int& finalState, const char& alphabet, const char& topStack, const std::string& overStack);

	void setInitialState(const int& state);
	void setFinalState(const int& state);
	void setAlphabet(const char& letter);
	void setTopStack(const char& letter);
	void setOverStack(const std::string& characters);
	void setOverStackSize(const int& size);

	int getOverStackSize() const;
	int getInitialState() const;
	int getFinalState() const;
	char getAlphabet() const;
	char getTopStack() const;
	std::string getOverStack() const;

	friend std::ostream& operator<<(std::ostream& os, const Prod& prod);
private:
	int m_overStackSize;
	int m_initState;
	int m_finalState;
	char m_alphabet;
	char m_topStack;
	std::string m_overStack;
};

