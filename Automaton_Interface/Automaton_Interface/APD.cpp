#include "APD.h"

APD::APD()
	:m_sizeQ(0)
	, m_sizeSum(0)
	, m_sizeDelta(0)
	, m_sizeF(0)
	, m_q0(0)
{
	m_Z0 = 'Z';
}

APD::APD(const uint16_t& sizeQ, const uint16_t& sizeSum, const uint16_t& sizeDelta, const uint16_t& sizeF
	, const std::vector<int>& Q, const std::vector<char>& Sum, const std::vector<Prod>& Delta, uint16_t q0
	, const std::vector<int>& F)
	:m_sizeQ(sizeQ)
	, m_sizeSum(sizeSum)
	, m_sizeDelta(sizeDelta)
	, m_sizeF(sizeF)
	, m_Q(Q)
	, m_Sum(Sum)
	, m_Delta(Delta)
	, m_q0(q0)
	, m_F(F)
{
	m_Z0 = 'Z';
}

void APD::setSizeQ(const uint16_t& sizeQ)
{
	m_sizeQ = sizeQ;
}

void APD::setSizeSum(const uint16_t& sizeSum)
{
	m_sizeSum = sizeSum;
}

void APD::setSizeDelta(const uint16_t& sizeDelta)
{
	m_sizeDelta = sizeDelta;
}

void APD::setSizeF(const uint16_t& sizeF)
{
	m_sizeF = sizeF;
}

void APD::setQ(const std::vector<int>& Q)
{
	m_Q = Q;
}

void APD::setSum(const std::vector<char>& Sum)
{
	m_Sum = Sum;
}

void APD::setDelta(const std::vector<Prod>& Delta)
{
	m_Delta = Delta;
}

void APD::setq0(uint16_t q0)
{
	m_q0 = q0;
}

void APD::setF(const std::vector<int>& F)
{
	m_F = F;
}

void APD::setSizeGamma(const uint16_t& sizeGamma)
{
	m_sizeGamma = sizeGamma;
}

void APD::setGamma(const std::vector<char>& Gamma)
{
	m_Gamma = Gamma;
}

void APD::setZ0(const char& Z0)
{
	m_Z0 = Z0;
}

uint16_t APD::getSizeQ() const
{
	return m_sizeQ;
}

uint16_t APD::getSizeSum() const
{
	return m_sizeSum;
}

uint16_t APD::getSizeDelta() const
{
	return m_sizeDelta;
}

uint16_t APD::getSizeF() const
{
	return m_sizeF;
}

uint16_t APD::getSizeGamma() const
{
	return m_sizeGamma;
}

std::vector<int> APD::getQ() const
{
	return m_Q;
}

std::vector<char> APD::getSum() const
{
	return m_Sum;
}

std::vector<Prod> APD::getDelta() const
{
	return m_Delta;
}

uint16_t APD::getq0() const
{
	return m_q0;
}

char APD::getZ0() const
{
	return m_Z0;
}

std::vector<int> APD::getF() const
{
	return m_F;
}

std::vector<char> APD::getGamma() const
{
	return m_Gamma;
}

void APD::printQ() const
{
	for (int i = 0; i < m_sizeQ; i++)
		std::cout << "q" << m_Q[i] << " ";
	std::cout << std::endl;
}

void APD::printSum() const
{
	for (int i = 0; i < m_sizeSum; i++)
		std::cout << m_Sum[i] << " ";
	std::cout << std::endl;
}

void APD::printDelta() const
{
	for (int i = 0; i < m_sizeDelta; i++)
	{
		std::cout << "\u03B4(" << m_Delta[i].getInitialState() << "," << m_Delta[i].getAlphabet() << "," << m_Delta[i].getTopStack() << ")={(" << m_Delta[i].getFinalState() << ",";
		for (int j = 0; j < m_Delta[i].getOverStackSize(); j++)
			std::cout << m_Delta[i].getOverStack()[j];
		std::cout << ")}\n";
	}
}

void APD::printF() const
{
	for (int i = 0; i < m_sizeF; i++)
		std::cout << 'q' << m_F[i] << ' ';
	std::cout << std::endl;
}

void APD::printAutomaton() const
{
	std::cout << "Dimensiune Q: ";
	std::cout << m_sizeQ << std::endl;
	std::cout << "Elementele lui Q: ";
	this->printQ();
	std::cout << "Dimensiune alfabet: ";
	std::cout << m_sizeSum << std::endl;
	std::cout << "Elementele alfabetului: ";
	this->printSum();
	std::cout << "Dimensiune gamma: ";
	std::cout << m_sizeGamma << std::endl;
	std::cout << "Elementele gamma: ";
	this->printGamma();
	std::cout << "Numar de tranzitii: ";
	std::cout << m_sizeDelta << std::endl;
	std::cout << "Tranzitiile sunt: ";
	this->printDelta();
	std::cout << "Stare initiala: ";
	std::cout << 'q' << m_q0 << std::endl;
	std::cout << "Dimensiune F: ";
	std::cout << m_sizeF << std::endl;
	std::cout << "Capul stivei: ";
	std::cout << m_Z0 << std::endl;
	std::cout << "Elementele lui F: ";
	this->printF();
}

void APD::addState(int state)
{
	m_Q.emplace_back(state);
}

void APD::addTransition(Prod transition)
{
	m_Delta.emplace_back(transition);
	m_sizeDelta = m_Delta.size();
}

void APD::addSymbol(char symbol)
{
	if (std::find(m_Sum.begin(), m_Sum.end(), symbol) == m_Sum.end())
	{
		m_Sum.emplace_back(symbol);
		m_sizeSum = m_Sum.size();
	}
}

void APD::addStackSymbol(char symbol)
{
	if (std::find(m_Gamma.begin(), m_Gamma.end(), symbol) == m_Gamma.end())
	{
		m_Gamma.emplace_back(symbol);
		m_sizeGamma = m_Gamma.size();
	}
}

void APD::addFinalState(int state)
{
	m_F.emplace_back(state);
	m_sizeF = m_F.size();
}

void APD::addInitialState(int state)
{
	m_q0 = state;
}

bool APD::checkWord(const std::vector<uint32_t> currentStates, const std::stack<char> currentStack, const std::string& word, const uint32_t& currentIndex)
{
	if (currentIndex == word.size()
		&& currentStack.size() == 1
		&& currentStack.top() == m_Z0)
		return true;
	else if (currentStack.size() == 1 && currentStack.top() != m_Z0) return false;
	else if (currentStack.size() < 1) return false;
	std::vector<uint32_t> nextState;
	std::stack<char> nextStack = currentStack;
	bool transitionFound = false;
	for (int currentState : currentStates)
	{
		for (auto transition : m_Delta)
		{
			if (transition.getInitialState() == currentState &&
				transition.getAlphabet() == word[currentIndex] &&
				transition.getTopStack() == currentStack.top())
			{
				transitionFound = true;
				//pasi intermediari
				nextState.emplace_back(transition.getFinalState());
				if (!nextStack.empty())
					nextStack.pop();
				std::string finalStack = transition.getOverStack();
				if (finalStack[0] == '~')
					continue;
				//era facuta original cu string, nu cu vector de char
				std::reverse(finalStack.begin(), finalStack.end());
				for (auto symbol : finalStack)
				{
					nextStack.push(symbol);
				}
			}
		}
	}
	if (!transitionFound)
		return false;
	return checkWord(nextState, nextStack, word, currentIndex + 1);
	//s ar putea sa crape la verificarea ultimei productii care verifica daca stiva e goala
}

bool APD::verifyAutomaton()
{
	if (std::find(m_Q.begin(), m_Q.end(), m_q0) == m_Q.end())
	{
		//std::cout << "1";
		return false;
	}

	if (std::find(m_Gamma.begin(), m_Gamma.end(), m_Z0) == m_Gamma.end())
	{
		//std::cout << "2";
		return false;
	}

	if (m_Q.size() == 0 || m_Sum.size() == 0 || m_Delta.size() == 0 || m_F.size() == 0)
	{
		//std::cout << "3";
		return false;
	}

	auto it = std::find(m_Q.begin(), m_Q.end(), m_q0);
	if (it == m_Q.end())
	{
		//std::cout << "4";
		return false;
	}

	for (int i = 0; i < m_F.size(); i++)
		if (std::find(m_Q.begin(), m_Q.end(), m_F[i]) == m_Q.end())
		{
			//std::cout << "5";
			return false;
		}

	for (int i = 0; i < m_Delta.size(); i++)
		if ((std::find(m_Q.begin(), m_Q.end(), m_Delta[i].getInitialState()) == m_Q.end()
			|| std::find(m_Sum.begin(), m_Sum.end(), m_Delta[i].getAlphabet()) == m_Sum.end()
			|| std::find(m_Q.begin(), m_Q.end(), m_Delta[i].getFinalState()) == m_Q.end()
			|| std::find(m_Gamma.begin(), m_Gamma.end(), m_Delta[i].getAlphabet()) == m_Gamma.end())
			&& m_Delta[i].getAlphabet() != '~')
		{
			//std::cout << "6";
			return false;
		}
	for (int i = 0; i < m_Delta.size(); i++)
	{
		std::string check = m_Delta[i].getOverStack();
		for (int j = 0; j < check.size(); j++)
		{
			if (check[j] == '~')
				continue;
			if (std::find(m_Gamma.begin(), m_Gamma.end(), check[j]) == m_Gamma.end())
			{
				//std::cout << "7";
				return false;
			}
		}

	}
	return true;
}

void APD::readAutomaton(std::ifstream& file)
{
	uint16_t value;
	int v;
	int v1;
	char character;
	char characterStack;
	std::string stackFinal;
	std::vector<int> int_vector;
	std::vector<char> char_vector;
	std::vector<Prod> Delta_vector;
	//citire dimensiune si elemente Q
	file >> value;
	m_sizeQ = value;
	for (int i = 0; i < m_sizeQ; i++)
	{
		file >> v;
		int_vector.push_back(v);
	}
	m_Q = int_vector;
	int_vector.clear();
	//citire dimensiune si elemente Sum
	file >> value;
	m_sizeSum = value;
	for (int i = 0; i < m_sizeSum; i++)
	{
		file >> character;
		char_vector.emplace_back(character);
	}
	m_Sum = char_vector;
	char_vector.clear();
	//citire dimensiune si elemente Gamma
	file >> value;
	m_sizeGamma = value;
	for (int i = 0; i < m_sizeGamma; i++)
	{
		file >> character;
		char_vector.emplace_back(character);
	}
	m_Gamma = char_vector;
	char_vector.clear();
	//citire dimensiune si elemente Delta
	file >> value;
	m_sizeDelta = value;
	Prod p;
	for (int i = 0; i < m_sizeDelta; i++)
	{
		file >> v >> character >> characterStack >> v1 >> stackFinal;
		p.setInitialState(v);
		p.setAlphabet(character);
		p.setTopStack(characterStack);
		p.setFinalState(v1);
		p.setOverStack(stackFinal);
		p.setOverStackSize(stackFinal.size());
		Delta_vector.push_back(p);
	}
	m_Delta = Delta_vector;
	Delta_vector.clear();
	//citire stare initiala q0
	file >> value;
	m_q0 = value;
	//citire stare initiala Z0
	file >> character;
	m_Z0 = character;
	//citire dimensiune si elemente F
	file >> value;
	m_sizeF = value;
	for (int i = 0; i < m_sizeQ; i++)
	{
		file >> v;
		int_vector.push_back(v);
	}
	m_F = int_vector;
	int_vector.clear();
}

void APD::printAutomaton(std::ofstream& file)
{
	file << "Dimensiune Q: ";
	file << m_sizeQ << "\n";
	file << "Elementele lui Q: ";
	for (int i = 0; i < m_sizeQ; i++)
		file << "q" << m_Q[i] << " ";
	file << "\n";
	file << "Dimensiune alfabet: ";
	file << m_sizeSum << "\n";
	file << "Elementele alfabetului: ";
	for (int i = 0; i < m_sizeSum; i++)
		file << m_Sum[i] << " ";
	file << "\n";
	file << "Dimensiune gamma: ";
	file << m_sizeGamma << "\n";
	file << "Elementele gamma: ";
	for (int i = 0; i < m_sizeGamma; i++)
		file << m_Gamma[i] << " ";
	file << "\n";
	file << "Numar de tranzitii: ";
	file << m_sizeDelta << "\n";
	file << "Tranzitiile sunt: ";
	for (int i = 0; i < m_sizeDelta; i++)
		file << m_Delta[i] << "\n";
	file << "\n";
	file << "Stare initiala: ";
	file << 'q' << m_q0 << "\n";
	file << "Stare initiala stiva: ";
	file << m_Z0 << "\n";
	file << "Dimensiune F: ";
	file << m_sizeF << "\n";
	file << "Elementele lui F: ";
	for (int i = 0; i < m_sizeF; i++)
		file << m_F[i] << " ";
}

void APD::printGamma() const
{
	for (int i = 0; i < m_sizeGamma; i++)
		std::cout << m_Gamma[i] << " ";
	std::cout << std::endl;
}
