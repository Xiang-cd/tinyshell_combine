//
// Created by 项小羽 on 2022/1/1.
//

#include "additionalFuns.h"
#include "Terminal.h"
#include <iostream>
#include <regex>
#include <string>
extern Terminal gTerm;

using namespace std;

void doVim(int argc, char *argv[]) {
    if (argc == 1) { system("vim"); }
    if (argc == 2) {
        if (regex_match(argv[1], regex("([0-9]|[a-z]|[A-Z]|-|_)+(.[a-z]+)?"))) {
            string a = "vim ";
            a.append(argv[1]);
            system(a.c_str());
        } else {
            cerr << "invalid file name!" << endl;
        }
    }
}

void doLs() {
    string root = gTerm.root;
    string work = gTerm.wdir;
    string ans = "ls " + root + work;
    system(ans.c_str());
}

void doCls() {
    system("clear");
}

void doChange(int argc, char *argv[]) {
    if (argc <= 1) {
        cerr << "select a theme please~" << endl;
    } else if (strlen(argv[1]) > 1 or argv[1][0] > '2' or argv[1][0] < '0') {
        cerr << "select theme from number 0 to 2" << endl;
    } else {
        gTerm.theme = argv[1][0] - '0';
    }
}