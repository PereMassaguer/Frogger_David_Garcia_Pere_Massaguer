#include <iostream>
#include <fstream>
#include <string.h>

using namespace std;

struct Register
{
	char Name[3];
	char Score[15];
};

int main() {
	Register NewScore, PastScore;

	ofstream file("register.dat", ios::binary);

	//Marca error al strcpy per lo que he fet un cpp nou es diu. RankingBinaryFile2.cpp (On ho he fet amb el mètode explicat a classe). 
	strcpy(NewScore.Name, "AAA");
	strcpy(NewScore.Score, "125");

	file.write((char*)&NewScore, sizeof(Register));
	file.close();

	//Per llegir els scores i els noms. 

	ifstream readFile("register.dat", ios::binary);
	readFile.read((char*)&PastScore, sizeof(Register)); //Proces invers a file.write((char*)&NewScore, sizeof(Register));

	//Per que surti per pantalla. 

	cout << "Name: " << PastScore.Name << endl;
	cout << "Score: " << PastScore.Score << endl;
}