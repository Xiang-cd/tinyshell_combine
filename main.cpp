#include "Terminal.h"
#include "doEcho.h"
#include <iostream>
#include <regex>
#include <string>

#define  MAXARG 20
#define Develop false

using namespace std;

extern Terminal gTerm;
extern int Argc;
extern char *Argv[MAXARG];

int main(int argc, char *argv[]) {
    system("clear");
    if (Develop) {
        string tmp = argv[0];
        const regex parten("/((\\w|-|.)*/)*");
        smatch path;
        if (regex_search(tmp, path, parten)) {
            memcpy(gTerm.root,path[0].str().c_str(),strlen(path[0].str().c_str()));
//            memcpy(gTerm.wdir,path[0].str().c_str(),strlen(path[0].str().c_str()));
            gTerm.root[strlen(gTerm.root)-1] = '\0';
            gTerm.wdir[0] = '/';
            gTerm.wdir[1] = '\0';
        }
    } else {
        GetAccountInit();
    }
    string tmp;
    smatch args;
    for (int i = 0; i < MAXARG; ++i) {
        Argv[i] = new char[MAXLINE];
    }

    bool right = true;
    while (true) {
        printHeading(right);
        getline(cin, tmp);
        right = splitInstr(tmp);
        if (regex_match(Argv[0], regex("exit")))break;
        else cout << gTerm.strout;
        memset(gTerm.strout, 0, MAXFILE); //每次输出后,都将输出流清空
    }
    for (int i = 0; i < MAXARG; ++i) {
        delete Argv[i];
    }
    return 0;
}

