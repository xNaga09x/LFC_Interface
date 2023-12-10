#pragma once
#include<iostream>
#include<string>
#include<vector>
#include<tuple>
#include<algorithm>
#include<unordered_set>
#include<fstream>
class AFD
{
public:
	AFD();
	AFD(const uint16_t& sizeQ, const uint16_t& sizeSum, const uint16_t& sizeDelta, const uint16_t& sizeF, const std::vector<int>& Q,
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

	bool isDeterministic();

	//verificare utilizand BackTracking (currentStates reprezinta starile de cuvinte,deoarece pot exista mai multe
	//stari intr-un moment,word e cuv dat,iar currentIndex e litera curenta din cuvant)
	bool checkWord(const std::unordered_set<int>& currentStates, const std::string& word, int currentIndex);

	void Read(std::ifstream& file);

	void printToFile(std::ofstream& file);

private:
	uint16_t m_sizeQ;
	uint16_t m_sizeSum;
	uint16_t m_sizeDelta;
	uint16_t m_sizeF;
	std::vector<int> m_Q;//se poate face si int sau string sau char(nu are treaba) - q0,q1,q2...
	std::vector<char> m_Sum;//alfabetul - a,b
	std::vector<std::tuple<int, char, int>> m_Delta;//functiile de tranzitie
	uint16_t m_q0;//starea initiala
	std::vector<int> m_F;//stari finale
};
