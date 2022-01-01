#pragma once
#include "Terminal.h"
#include <iostream>
#include <regex>
#include <string>
using namespace std;

extern Terminal gTerm;
extern const int MAXLINE;
extern const int MAXFILE;

void doEcho(int argc, char *argv[]);