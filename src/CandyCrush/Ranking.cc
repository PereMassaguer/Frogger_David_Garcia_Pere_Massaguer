#include "Ranking.hh"



Ranking::Ranking()
{
	std::fstream file("register.dat", std::ios::out | std::ios::binary | std::ios::app);
	file.close();
}


Ranking::~Ranking()
{
}


void Ranking::AddRankingInfo(PersonData info)
{
	std::vector<PersonData> data = GetRankingInfo();

	std::ofstream file("register.dat", std::ios::out | std::ios::binary);
	int maxRankSize = (int)(data.size() < 10 ? data.size() + 1 : data.size());

	int i = 0;
	for (i; i < maxRankSize - 1; i++) {
		if (data.at(i).score < info.score) break;
		file.write((char *)(&data.at(i)), sizeof(PersonData));
	}
	file.write((char *)(&info), sizeof(PersonData));
	for (i; i < maxRankSize - 1; i++)
		file.write((char *)(&data.at(i)), sizeof(PersonData));
	file.close();
}


std::vector<PersonData> Ranking::GetRankingInfo()
{
	std::ifstream file("register.dat", std::ios::in | std::ios::binary);
	std::vector<PersonData> ret;
	
	PersonData aux;
	file.seekg(0, std::ios::end);
	int dataSize = (int)(file.tellg() / sizeof(PersonData));
	file.seekg(0, std::ios::beg);

	for (int i = 0; i < dataSize; i++) {
		file.read((char *)(&aux), sizeof(PersonData));
		ret.push_back(aux);
	}

	file.close();
	return ret;
}