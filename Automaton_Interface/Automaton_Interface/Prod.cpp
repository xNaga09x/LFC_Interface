#include "Prod.h"

Prod::Prod()
{

}

Prod::Prod(const int& overStackSize, const int& initState, const int& finalState, const char& alphabet, const char& topStack, const std::string& overStack)
	:m_overStackSize(overStackSize), m_initState(initState), m_finalState(finalState), m_alphabet(alphabet), m_topStack(topStack), m_overStack(overStack)
{
}

void Prod::setInitialState(const int& state)
{
	m_initState = state;
}

void Prod::setFinalState(const int& state)
{
	m_finalState = state;
}

void Prod::setAlphabet(const char& letter)
{
	m_alphabet = letter;
}

void Prod::setTopStack(const char& letter)
{
	m_topStack = letter;
}

void Prod::setOverStack(const std::string& characters)
{
	m_overStack = characters;
}

void Prod::setOverStackSize(const int& size)
{
	m_overStackSize = size;
}

int Prod::getOverStackSize() const
{
	return m_overStackSize;
}

int Prod::getInitialState() const
{
	return m_initState;
}

int Prod::getFinalState() const
{
	return m_finalState;
}

char Prod::getAlphabet() const
{
	return m_alphabet;
}

char Prod::getTopStack() const
{
	return m_topStack;
}

std::string Prod::getOverStack() const
{
	return m_overStack;
}

std::ostream& operator<<(std::ostream& os, const Prod& prod)
{
	os << "delta(" << prod.m_initState << ", " << prod.m_alphabet << ", " << prod.m_topStack << ") = ("
		<< prod.m_finalState << ", " << prod.m_overStack << ")";
	return os;
}
