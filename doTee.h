#pragma once

#include "Terminal.h"
#include <iostream>
#include <cstdio>
#include <fstream>
using namespace std;

extern Terminal gTerm;
extern vector<string> splitpath(string tmp);
extern bool processpath(vector<string> lst, stack<string>& final_path);
extern string combinepath(stack<string> final);
void doTee(int argc, char* argv[]);