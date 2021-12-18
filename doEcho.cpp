#include "doEcho.h"

void doEcho(int argc, char *argv[]) {
    int posi = 0;
    if (regex_match(argv[1], regex("-n"))) posi = 1;
    if (regex_match(argv[1], regex("--help"))) {
        memset(gTerm.strout, 0, MAXFILE);
        string a = "Echo the STRING(s) to standard output.\n-n     do not output the trailing newline\n--help display this help and exit\n--version\noutput version information and exit\n";
        memcpy(gTerm.strout, a.c_str(), strlen(a.c_str()));
        return;
    }
    if (regex_match(argv[1], regex("--version"))) {
        memset(gTerm.strout, 0, MAXFILE);
        string a = "homework for basic programming,version 0.1\n";
        memcpy(gTerm.strout, a.c_str(), strlen(a.c_str()));
        return;
    }
    string temp;
    for (int i = posi ? 2 : 1; i < argc; ++i) {
        temp.append(argv[i]);
        if (i != argc - 1) temp.append(" ");
    }
    if (!posi) { temp.append("\n"); }
    memset(gTerm.strout, 0, MAXFILE);
    memcpy(gTerm.strout, temp.c_str(), strlen(temp.c_str()));
}

