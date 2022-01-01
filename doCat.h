#ifndef TOTAL_DOCAT_H
#define TOTAL_DOCAT_H

#include "Terminal.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;
extern Terminal gTerm;

extern vector<string> splitpath(string tmp);

extern bool processpath(vector<string> lst, stack<string> &final_path);

extern string combinepath(stack<string> final);


#endif //TOTAL_DOCAT_H
