#include "doCd.h"

void doCd(int argc, char *argv[]) {
    if (argc >= 3) {
        cerr << "too many arguments!" << endl;
        return;
    } else if (argc < 2) {
        cerr << "lack of arguments!" << endl;
        return;
    }
    string P = argv[1];
    stack<string> final_path;
    if (regex_match(P, regex("--help"))) {
        string outs =
                "cd instruction is to change the working directory"
                "The cd utility shall change the working directory of the current shell execution environment"
                "use it like:\ncd /home/ect\n";
        memcpy(gTerm.strout,outs.c_str(),outs.size());
    } else if (regex_match(P, regex("/((\\w|-|.)*/?)*"))) {
        vector<string> lst = splitpath(P);
        if (!processpath(lst, final_path)) return;
        string tmp = combinepath(final_path);
        strcpy(gTerm.wdir, tmp.c_str());
        if (gTerm.wdir[strlen(gTerm.wdir) - 1] == '/' and strlen(gTerm.wdir) > 1)
            gTerm.wdir[strlen(gTerm.wdir) - 1] = '\0';
    } else if (regex_match(P, regex("((\\w|-|.)*/?)*"))) {
        string abs = gTerm.wdir;
        abs += "/";
        abs += P;
        vector<string> lst = splitpath(abs);
        if (!processpath(lst, final_path)) return;
        string tmp = combinepath(final_path);
        strcpy(gTerm.wdir, tmp.c_str());
        if (gTerm.wdir[strlen(gTerm.wdir) - 1] == '/' and strlen(gTerm.wdir) > 1)
            gTerm.wdir[strlen(gTerm.wdir) - 1] = '\0';
    } else {
        cerr << "invalid path!" << endl;
    }
}
