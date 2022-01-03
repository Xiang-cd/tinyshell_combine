
#pragma once
#include "Terminal.h"
#include <stack>
#include <vector>
#include <regex>
#include <iostream>
#include <cstring>
#include <fstream>

#define  Debug true

extern Terminal gTerm;
extern vector<string> splitpath(string tmp);
extern bool processpath(vector<string> lst, stack<string>& final_path);
extern string combinepath(stack<string> final);
bool test = false;
using namespace std;


