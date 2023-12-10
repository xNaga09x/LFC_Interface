#include "AFD.h"

AFD::AFD()
	:m_sizeQ(0)
	, m_sizeSum(0)
	, m_sizeDelta(0)
	, m_sizeF(0)
	, m_q0(0)
{
}

AFD::AFD(const uint16_t& sizeQ, const uint16_t& sizeSum, const uint16_t& sizeDelta, const uint16_t& sizeF
	, const std::vector<int>& Q, const std::vector<char>& Sum, const std::vector<std::tuple<int, char, int>>& Delta, uint16_t q0
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
}

void AFD::setSizeQ(const uint16_t& sizeQ)
{
	m_sizeQ = sizeQ;
}

void AFD::setSizeSum(const uint16_t& sizeSum)
{
	m_sizeSum = sizeSum;
}

void AFD::setSizeDelta(const uint16_t& sizeDelta)
{
	m_sizeDelta = sizeDelta;
}

void AFD::setSizeF(const uint16_t& sizeF)
{
	m_sizeF = sizeF;
}

void AFD::setQ(const std::vector<int>& Q)
{
	m_Q = Q;
}

void AFD::setSum(const std::vector<char>& Sum)
{
	m_Sum = Sum;
}

void AFD::setDelta(const std::vector<std::tuple<int, char, int>>& Delta)
{
	m_Delta = Delta;
}

void AFD::setq0(uint16_t q0)
{
	m_q0 = q0;
}

void AFD::setF(const std::vector<int>& F)
{
	m_F = F;
}

uint16_t AFD::getSizeQ() const
{
	return m_sizeQ;
}

uint16_t AFD::getSizeSum() const
{
	return m_sizeSum;
}

uint16_t AFD::getSizeDelta() const
{
	return m_sizeDelta;
}

uint16_t AFD::getSizeF() const
{
	return m_sizeF;
}

std::vector<int> AFD::getQ() const
{
	return m_Q;
}

std::vector<char> AFD::getSum() const
{
	return m_Sum;
}

std::vector<std::tuple<int, char, int>> AFD::getDelta() const
{
	return m_Delta;
}

uint16_t AFD::getq0() const
{
	return m_q0;
}

std::vector<int> AFD::getF() const
{
	return m_F;
}

bool AFD::verifyAutomaton()
{
	//Verific ca m_sizeQ si m_Sum sa fie nevide
	if (m_sizeQ == 0 || m_sizeSum == 0 || m_sizeDelta == 0 || m_sizeF == 0)
	{
		return false;
	}
	//Verific daca m_q0 se afla in m_Q
	auto it = std::find(m_Q.begin(), m_Q.end(), m_q0);
	if (it == m_Q.end())
	{
		return false;
	}
	//Verifica daca elementele din m_F apartin lui m_Q
	for (int i = 0; i < m_F.size(); i++)
		if (std::find(m_Q.begin(), m_Q.end(), m_F[i]) == m_Q.end())
		{
			return false;
		}
	//Verifica daca elem lui m_Delta apartin lui m_Q,m_Sum,m_Q
	for (int i = 0; i < m_sizeDelta; i++)
		if (std::find(m_Q.begin(), m_Q.end(), std::get<0>(m_Delta[i])) == m_Q.end()
			|| std::find(m_Sum.begin(), m_Sum.end(), std::get<1>(m_Delta[i])) == m_Sum.end()
			|| std::find(m_Q.begin(), m_Q.end(), std::get<2>(m_Delta[i])) == m_Q.end())
		{
			return false;
		}

	return true;
}

void AFD::printQ() const
{
	for (int i = 0; i < m_sizeQ; i++)
		std::cout << "q" << m_Q[i] << " ";
	std::cout << std::endl;
}

void AFD::printSum() const
{
	for (int i = 0; i < m_sizeSum; i++)
		std::cout << m_Sum[i] << " ";
	std::cout << std::endl;
}

void AFD::printDelta() const
{
	for (int i = 0; i < m_sizeDelta; i++)
		std::cout << "(q" << std::get<0>(m_Delta[i]) << ',' << std::get<1>(m_Delta[i]) << ")=q" << std::get<2>(m_Delta[i]) << std::endl;

}

void AFD::printF() const
{
	for (int i = 0; i < m_sizeF; i++)
		std::cout << 'q' << m_F[i] << ' ';
	std::cout << std::endl;
}

void AFD::printAutomaton() const
{
	std::cout << "Dimensiune Q: ";
	std::cout << m_sizeQ << std::endl;
	std::cout << "Elementele lui Q: ";
	this->printQ();
	std::cout << "Dimensiune alfabet: ";
	std::cout << m_sizeSum << std::endl;
	std::cout << "Elementele alfabetului: ";
	this->printSum();
	std::cout << "Numar de tranzitii: ";
	std::cout << m_sizeDelta << std::endl;
	std::cout << "Tranzitiile sunt: ";
	this->printDelta();
	std::cout << "Stare initiala: ";
	std::cout << 'q' << m_q0 << std::endl;
	std::cout << "Dimensiune F: ";
	std::cout << m_sizeF << std::endl;
	std::cout << "Elementele lui F: ";
	this->printF();
}

bool AFD::isDeterministic()
{
	for (int i = 0; i < m_sizeDelta - 1; i++)
		for (int j = i + 1; j < m_sizeDelta; j++)
			if (std::get<0>(m_Delta[i]) == std::get<0>(m_Delta[j]) && std::get<1>(m_Delta[i]) == std::get<1>(m_Delta[j]))
				return false;
	return true;
}

bool AFD::checkWord(const std::unordered_set<int>& currentStates, const std::string& word, int currentIndex)
{
	//verific daca am parcurs cuvantul
	if (currentIndex == word.size())
	{
		//verific daca e cel putin o stare finala in starile curente
		for (int state : currentStates)

			if (std::find(m_F.begin(), m_F.end(), state) != m_F.end())
				return true;
		return false;
	}
	//verific tranzitiile pentru caracterul curent,verific fiecare drum pe care are
	std::unordered_set<int> nextState;
	for (int currentState : currentStates)
		for (auto transition : m_Delta)
		{
			if (std::get<0>(transition) == currentState && (std::get<1>(transition) == word[currentIndex] ||
				std::get<1>(transition) == '\0'))
				nextState.insert(std::get<2>(transition));

		}
	//apeleare a urmatorului caracter
	return checkWord(nextState, word, currentIndex + 1);
}

void AFD::Read(std::ifstream& file)
{
	uint16_t value;
	int v;
	int v1;
	char character;
	std::vector<int> int_vector;
	std::vector<char> char_vector;
	std::vector<std::tuple<int, char, int>> Delta_vector;
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
	//citire dimensiune si elemente Delta
	file >> value;
	m_sizeDelta = value;
	for (int i = 0; i < m_sizeDelta; i++)
	{
		file >> v >> character >> v1;
		Delta_vector.push_back(std::make_tuple(v, character, v1));
	}
	m_Delta = Delta_vector;
	Delta_vector.clear();
	//citire stare initiala q0
	file >> value;
	m_q0 = value;
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
	//return AFD();
}

void AFD::printToFile(std::ofstream& file)
{
	if (isDeterministic())
	{
		file << "M=({";
		for (uint32_t index = 0; index < m_sizeQ; index++)
		{
			if (index < m_sizeQ - 1)
				file << "q" << index << ", ";
			else file << "q" << index << "}, {";

		}
		for (uint32_t index = 0; index < m_sizeSum; index++)
		{
			if (index < m_sizeSum - 1)
				file << m_Sum[index] << ", ";
			else file << m_Sum[index] << "}, ";
		}
		file << "delta, q" << m_q0 << ", {";
		for (uint32_t index = 0; index < m_sizeF; index++)
		{
			if (index < m_sizeF - 1)
				file << "q" << m_F[index] << ", ";
			else file << "q" << m_F[index] << "})";
		}
		file << "\nDelta:\n";
		for (auto& transition : m_Delta)
		{
			file << "delta(q" << std::get<0>(transition) << ","
				<< std::get<1>(transition) << ")=q" << std::get<2>(transition) << "\n";
		}
	}
	else file << "Automatul este salvat cu tipul gresit.";
}

