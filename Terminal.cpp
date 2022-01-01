#include "Terminal.h"
#include <stack>
#include <vector>
#include <fstream>

Terminal gTerm;
int Argc;
char *Argv[MAXARG] = {};


vector<string> splitpath(string tmp) {
    vector<string> ans;
    regex parttern("/");
    sregex_token_iterator pos(tmp.begin(), tmp.end(), parttern, -1);
    decltype(pos) end;
    for (; pos != end; ++pos) {
        if (!regex_match(pos->str(), regex("\\s*")))ans.push_back(pos->str());
    }
    return ans;
}

bool processpath(vector<string> lst, stack<string> &final_path) {
    // 只进行.的过滤和..的合并, 其他不做处理
    for (int i = 0; i < lst.size(); ++i) {
        if (regex_match(lst[i], regex("\\."))) continue;
        else if (regex_match(lst[i], regex("\\.\\."))) {
            if (!final_path.empty())final_path.pop();
            else {
                cerr << "invalid path!" << endl;
                return false;
            }
        } else final_path.push(lst[i]);
    }
    return true;
}

string combinepath(stack<string> final) {
    stack<string> tmp;
    string ans = "/";
    while (!final.empty()) {
        tmp.push(final.top());
        final.pop();
    }
    while (!tmp.empty()) {
        ans += tmp.top();
        ans += "/";
        tmp.pop();
    }
    return ans;
}


inline void printColor(const string &s, int front, int color, bool light = false) {
    if (light) printf("\033[%d%dm%s\033[0m", front, color, s.c_str());
    else printf("\033[1;%d%dm%s\033[0m", front, color, s.c_str());
}

inline void
printTheme(const string &a, int front, int color, const string &b, int front2, int color2, bool light = true) {
    printColor(a, front, color, light);
    cout << ":";
    printColor(b, front2, color2, light);
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
    if (right)cout << "$";
    else printColor("$", 3, 1);
}

inline string BasicInit(const string &input, const regex &ex) {
    cout << input;
    bool flag = false;
    string temp;
    while (!flag) {
        getline(cin, temp);
        if (strlen(temp.c_str()) > MAXLINE) {
            cerr << "too long input name, please input again!\n";
            cout << input;
            continue;
        }
        flag = regex_match(temp, ex);
        if (!flag) {
            cerr << "invalid name, please input again!\n";
            cout << input;
        } else break;
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
    tmp = BasicInit("Root Dir:", regex("/((\\w|-|.)*/?)*"));
    memcpy(gTerm.root, tmp.c_str(), strlen(tmp.c_str()));
    if (strlen(gTerm.root) > 1 and gTerm.root[strlen(gTerm.root) - 1] == '/')gTerm.root[strlen(gTerm.root) - 1] = '\0';
    tmp = BasicInit("Login:", regex("(_|[a-z]|[A-Z])\\w*"));
    memcpy(gTerm.user, tmp.c_str(), strlen(tmp.c_str()));
    // memcpy(gTerm.wdir, gTerm.root, strlen(gTerm.root));
    strcat(gTerm.wdir, "/"); //初识化的时候把根目录设置为/user/...，工作目录设置为/
    gTerm.theme = 0;
    string welcome = "    ███████    █████                                                                    \n"
                     "  ███░░░░░███ ░░███                                                                     \n"
                     " ███     ░░███ ░███████      █████████████   █████ ████                                 \n"
                     "░███      ░███ ░███░░███    ░░███░░███░░███ ░░███ ░███                                  \n"
                     "░███      ░███ ░███ ░███     ░███ ░███ ░███  ░███ ░███                                  \n"
                     "░░███     ███  ░███ ░███     ░███ ░███ ░███  ░███ ░███                                  \n"
                     " ░░░███████░   ████ █████    █████░███ █████ ░░███████                                  \n"
                     "   ░░░░░░░    ░░░░ ░░░░░    ░░░░░ ░░░ ░░░░░   ░░░░░███                                  \n"
                     "                                              ███ ░███                                  \n"
                     "                                             ░░██████                                   \n"
                     "                                              ░░░░░░                                    \n"
                     "  █████     ███                                   █████               ████  ████     ███\n"
                     " ░░███     ░░░                                   ░░███               ░░███ ░░███    ░███\n"
                     " ███████   ████  ████████   █████ ████     █████  ░███████    ██████  ░███  ░███    ░███\n"
                     "░░░███░   ░░███ ░░███░░███ ░░███ ░███     ███░░   ░███░░███  ███░░███ ░███  ░███    ░███\n"
                     "  ░███     ░███  ░███ ░███  ░███ ░███    ░░█████  ░███ ░███ ░███████  ░███  ░███    ░███\n"
                     "  ░███ ███ ░███  ░███ ░███  ░███ ░███     ░░░░███ ░███ ░███ ░███░░░   ░███  ░███    ░░░ \n"
                     "  ░░█████  █████ ████ █████ ░░███████     ██████  ████ █████░░██████  █████ █████    ███\n"
                     "   ░░░░░  ░░░░░ ░░░░ ░░░░░   ░░░░░███    ░░░░░░  ░░░░ ░░░░░  ░░░░░░  ░░░░░ ░░░░░    ░░░ \n"
                     "                             ███ ░███                                                   \n"
                     "                            ░░██████                                                    \n"
                     "                             ░░░░░░                                                     \n";
    printColor(welcome, 3, 5);
}

bool selectInstr() {
    if (regex_match(Argv[0], regex("echo"))) {
        doEcho(Argc, Argv);
    } else if (regex_match(Argv[0], regex("cd"))) {
        doCd(Argc, Argv);
    } else if (regex_match(Argv[0], regex("pwd"))) {
        doPwd(Argc, Argv);
    } else if (regex_match(Argv[0], regex("ls"))) {
        doLs();
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
        doCls();
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