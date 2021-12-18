
#pragma once

#include <iostream>
#include <cstring>
#include <fstream>
#include "Terminal.h"
#define  Debug false

extern Terminal gTerm;

bool test = false;
void doDiff(int argc,char* argv[]);

int choose(int a, int b,int maxlinea,int maxlineb,bool sameline[20][20],bool aim[20][20]);
