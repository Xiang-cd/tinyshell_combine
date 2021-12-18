#include "Terminal.h"
Terminal gTerm;
int Argc;
char *Argv[MAXARG] = {};




void doTee(int argc, char *argv[]) {
    TestArg(argc, argv);
}

void doCat(int argc, char *argv[]) {
    TestArg(argc, argv);
}

void doCp(int argc, char *argv[]) {
    TestArg(argc, argv);
}

void doCd(int argc, char *argv[]) {
    TestArg(argc, argv);
}

void doPwd(int argc, char *argv[]) {
    TestArg(argc, argv);
    string tmp = "pwd output";
    memcpy(gTerm.strout, tmp.c_str(), strlen(tmp.c_str()));
    cerr << "after pwd strout:" << gTerm.strout << endl;
}

void doLs(int argc, char *argv[]) {
    TestArg(argc, argv);
}

void doCls(int argc, char *argv[]) {
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
//以上仅供调试,到合起来的时候得改







inline void printColor(const string &s, int front, int color, bool light = false) {
    if (light) printf("\033[%d%dm%s\033[0m", front, color, s.c_str());
    else printf("\033[1;%d%dm%s\033[0m", front, color, s.c_str());
}

inline void printTheme(const string &a, int front, int color, const string &b, int front2, int color2,bool light = true) {
    printColor(a, front, color,light);
    cout << ":";
    printColor(b, front2, color2,light);
}

void printHeading(bool right) {
    string a;
    a.append(gTerm.mach);
    a.append("@");
    a.append(gTerm.user);
    string b = gTerm.wdir;
    if (gTerm.theme == 2) {
        printTheme(a, 3, 5, b, 3, 3);
    } else if (gTerm.theme == 1) {
        printTheme(a, 4, 6, b, 4, 2);
    } else {
        printTheme(a, 3, 2, b, 3, 4);
    }
    if (right)cout<<"$";
    else printColor("$",3,1);
}

inline string BasicInit(const string &input, const regex &ex) {
    cout << input;
    bool flag = false;
    string temp;
    while (!flag) {
        getline(cin, temp);
        if (strlen(temp.c_str()) > MAXLINE) {
            cerr << "too long input name, please input again!\n";
            cout<<input;
            continue;
        }
        flag = regex_match(temp, ex);
        if (!flag){
            cerr << "invalid name, please input again!\n";
            cout<<input;
        }
        else break;
    }
    return temp;
}

inline void TestArg(int argc, char *argv[]) {
    for (int i = 0; i < argc; ++i) {
        cerr << "arg" << i << ":" << argv[i] << endl;
    }
    cerr << "stdin:" << gTerm.strin << endl;
    cerr << "stdout:" << gTerm.strout << endl;
}

void GetAccountInit() {
    string tmp = BasicInit("Machine Name:", regex("(_|[a-z]|[A-Z])\\w*"));
    memcpy(gTerm.mach, tmp.c_str(), strlen(tmp.c_str()));
    tmp = BasicInit("Root Dir:", regex("/((\\w|-|.)*/)*(\\w|-|.)*/?"));
    memcpy(gTerm.root, tmp.c_str(), strlen(tmp.c_str()));
    tmp = BasicInit("Login:", regex("(_|[a-z]|[A-Z])\\w*"));
    memcpy(gTerm.user, tmp.c_str(), strlen(tmp.c_str()));
    memcpy(gTerm.wdir, gTerm.root, strlen(gTerm.root));
    gTerm.theme = 0;
}

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

bool selectInstr() {
    if (regex_match(Argv[0], regex("echo"))) {
        doEcho(Argc, Argv);
    } else if (regex_match(Argv[0], regex("cd"))) {
        doCd(Argc, Argv);
    } else if (regex_match(Argv[0], regex("pwd"))) {
        doPwd(Argc, Argv);
    } else if (regex_match(Argv[0], regex("ls"))) {
        doLs(Argc, Argv);
    } else if (regex_match(Argv[0], regex("cat"))) {
        doCat(Argc, Argv);
    } else if (regex_match(Argv[0], regex("tee"))) {
        doTee(Argc, Argv);
    } else if (regex_match(Argv[0], regex("cp"))) {
        doCp(Argc, Argv);
    } else if (regex_match(Argv[0], regex("diff"))) {
        doDiff(Argc, Argv);
    } else if (regex_match(Argv[0], regex("grep"))) {
        doGrep(Argc, Argv);
    } else if (regex_match(Argv[0], regex("clear|cls"))) {
        doCls(Argc, Argv);
    } else if (regex_match(Argv[0], regex("vim"))) {
        doVim(Argc, Argv);
    } else if (regex_match(Argv[0], regex("change"))) {
        doChange(Argc, Argv);
    } else if (regex_match(Argv[0], regex("exit"))) {
        return true;
    } else {
        if (strlen(Argv[0]) > 0)return false;
    }
    return true;
}

void proceseInstr(string tmp) {
    Argc = 0;
    regex parttern("\\s+");
    tmp.erase(0, tmp.find_first_not_of(' '));
    sregex_token_iterator pos(tmp.begin(), tmp.end(), parttern, -1);
    decltype(pos) end;
    for (; pos != end; ++pos) {
        memset(Argv[Argc], 0, sizeof(char) * MAXLINE);
        memcpy(Argv[Argc], pos->str().c_str(), strlen(pos->str().c_str()));
        Argc++;
    }
}

bool splitInstr(string tmp) {
    bool flag = true;
    regex parttern("\\|");
    sregex_token_iterator pos(tmp.begin(), tmp.end(), parttern, -1);
    decltype(pos) end;
    for (; pos != end; ++pos) {
        proceseInstr(pos->str());
        memcpy(gTerm.strin, gTerm.strout, MAXFILE);
        memset(gTerm.strout, 0, MAXFILE);
        bool tmpf = selectInstr();
        flag = flag and tmpf; //如果遇到指令选择错误，则跳出指令的执行
        if (not tmpf)cerr << "command \"" << Argv[0] << "\" not found!" << endl;
    }
    return flag;
}