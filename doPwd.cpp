#include "doPwd.h"

void doPwd(int argc, char *argv[]) {
    if (strcmp(argv[1], "--help") == 0) {
        cerr << "pwd: Print the name of the current working dirctory." << endl;
        return;
    }
    if (argc>1){
        cerr<<"too many argument!"<<endl;
    }
    strcat(gTerm.strout, gTerm.wdir);
    strcat(gTerm.strout, "\n");
}
