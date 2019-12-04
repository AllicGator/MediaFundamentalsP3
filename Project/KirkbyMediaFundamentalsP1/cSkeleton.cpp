/*	Project:	KirkbyMediaFundamentalsP1
 *	File:		cSkeleton.cpp
 *	Author:		Dylan Kirkby
 *	Purpose:	Implementation for cSkeleton.h
 */

#include "cSkeleton.h"
#include <fstream>

Skeleton::Skeleton(std::string path)
{
	maxhp = rand() % 20 + 10;
	curhp = maxhp;
	minDmg = rand() % 4 + 0;
	maxDmg = rand() % 10 + 4;
	hitRate = rand() % 80 + 50;

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
