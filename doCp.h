#pragma once
#include "Terminal.h"
#include <fstream>
#include <iostream>
using namespace std;

extern Terminal gTerm;

void doCp(int argc, char * argv[]);

void init_n();

int n_flag = 0; //0表示不开启-n模式，1表示开启-n模式

