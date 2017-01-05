#include <iostream>
#include <fstream>
#include <string.h>
#include <stdlib.h>



using namespace std;

int  main() {

	string Name = "Pere";
	int Score = 128;

	ofstream fileOut("register.dat", ios::out | ios::binary);

	fileOut.write(reinterpret_cast<char *>(&Name), Name.size());
	fileOut.write(reinterpret_cast<char *>(&Score), sizeof(Score));


	fileOut.close();

	ifstream fileIn("register.dat", ios::in | ios::binary);

	fileIn.read(reinterpret_cast<char *>(&Name), Name.size());
	fileIn.read(reinterpret_cast<char *>(&Score), sizeof(Score));

	cout << "Score: " << Score << endl;
	system("pause");
}