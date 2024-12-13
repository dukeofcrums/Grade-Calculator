#pragma once
#include <iostream>
#include <iomanip>
#include <vector>
#include <stdlib.h>
#include <string>
#include <fstream>

using namespace std;

struct assignment {
	string name;
	double grade;
	double weight;
	bool graded;
	double norm_grade;
	double norm_weight;
	bool taken = false;
	int index_least;
};