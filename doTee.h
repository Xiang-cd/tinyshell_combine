#pragma once

#include "Terminal.h"
#include <iostream>
#include <cstdio>
#include <fstream>
using namespace std;

extern Terminal gTerm;

void doTee(int argc, char* argv[]);