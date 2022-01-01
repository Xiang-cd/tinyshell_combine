//
// Created by 项小羽 on 2022/1/1.
//

#ifndef TOTAL_DOCD_H
#define TOTAL_DOCD_H

#include "Terminal.h"
#include <stack>
using namespace std;

extern Terminal gTerm;

extern vector<string> splitpath(string tmp);

extern bool processpath(vector<string> lst, stack<string> &final_path);

extern string combinepath(stack<string> final);

#endif //TOTAL_DOCD_H
