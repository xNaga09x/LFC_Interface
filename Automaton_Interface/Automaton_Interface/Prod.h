#pragma once
#include <vector>

class Prod
{
public:
	Prod();
	Prod(const int& overStackSize, const int& initState, const int& finalState, const char& alphabet, const char& topStack, const std::vector<char>& overStack);

	void setInitialState(const int& state);
	void setFinalState(const int& state);
	void setAlphabet(const char& letter);
	void setTopStack(const char& letter);
	void setOverStack(const std::vector<char>& characters);
	void setOverStackSize(const int& size);

	int getOverStackSize() const;
	int getInitialState() const;
	int getFinalState() const;
	char getAlphabet() const;
	char getTopStack() const;
	std::vector<char> getOverStack() const;
private:
	int m_overStackSize;
	int m_initState;
	int m_finalState;
	char m_alphabet;
	char m_topStack;
	std::vector<char> m_overStack;
};

