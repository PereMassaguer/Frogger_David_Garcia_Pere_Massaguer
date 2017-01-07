#pragma once
#include <iostream>
#include <fstream>
#include <string.h>
#include <stdlib.h>
#include <vector>

typedef struct {
	char name[10];
	int score;
}PersonData;


#define Rank Ranking::Instance()
class Ranking
{

public:

	Ranking();
	~Ranking();

	inline static Ranking &Instance(void) {
		static Ranking rank;
		return rank;
	}

	void AddRankingInfo(PersonData info);
	std::vector<PersonData> GetRankingInfo();
};