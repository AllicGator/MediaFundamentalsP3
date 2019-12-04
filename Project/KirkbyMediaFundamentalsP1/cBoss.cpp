/*	Project:	KirkbyMediaFundamentalsP1
 *	File:		cBoss.cpp
 *	Author:		Dylan Kirkby
 *	Purpose:	Implementation for cBoss.h
 */

#include "cBoss.h"
#include <fstream>

Boss::Boss(std::string path)
{
	srand(time(NULL));
	maxhp = 100;
	curhp = maxhp;
	minDmg = rand() % 10 + 7;
	maxDmg = rand() % 25 + 13;
	hitRate = rand() % 95 + 80;

	//read ascii art into vector
	std::ifstream input_file;
	input_file.open(path);
	if (!input_file) {
		printf("Unable to open %s\n\tContinuing without art...\n", path.c_str());
		system("pause");
	}

	std::string line;
	while (std::getline(input_file, line))
	{
		if (line.length() > 0) {
			asciiArt.push_back(line);
		}
	}
}
