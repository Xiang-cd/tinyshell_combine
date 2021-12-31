#include "Terminal.h"
#include <stack>
#include <vector>
#include <fstream>

Terminal gTerm;
int Argc;
char *Argv[MAXARG] = {};
char tmp_for_p[MAXLINE] = {};

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

static inline void p(string &a, bool last, bool line, int &num) {
    memset(tmp_for_p, 0, MAXLINE);
    if (line) {
        if (last)sprintf(tmp_for_p, "%6d  %s$\n", num++, a.c_str());
        else sprintf(tmp_for_p, "%6d  %s\n", num++, a.c_str());
    } else {
        if (last)sprintf(tmp_for_p, "%s$\n", a.c_str());
        else sprintf(tmp_for_p, "%s\n", a.c_str());
    }
    strcat(gTerm.strout, tmp_for_p);
}

static inline void select_print_mode(bool *opts, string line, bool &last_empty, int &num) {
    if ((opts[1] or opts[2]) and (line.empty())) {
        if (opts[2]) {
            if (!last_empty) {
                last_empty = true;
                if (opts[1]) p(line, opts[3], false, num);
                else p(line, opts[3], opts[0], num);
                return;
            } else return;
        } else {
            if (opts[1]) p(line, opts[3], false, num);
            else p(line, opts[3], opts[0], num);
            return;
        }
    }
    last_empty = false;
    p(line, opts[3], opts[0] or opts[1], num);
}

void doCat(int argc, char *argv[]) {
    memset(gTerm.strout, 0, MAXFILE);
    bool opts[4] = {false};
    if (argc < 2) {   //这里是参数数量判断
        cerr << "lack of arguments" << endl;
        return;
    }
    vector<string> arguments;
    for (int i = 1; i < argc; ++i) {
        string tmp = argv[i];
        arguments.push_back(tmp);
    }
    vector<string> filelist;
    bool file_start = false;
    for (int i = 0; i < arguments.size(); ++i) {
        if (regex_match(arguments[i], regex("-\\w+")) and !file_start) {
            if (regex_match(arguments[i], regex("-n"))) {
                opts[0] = true;
            } else if (regex_match(arguments[i], regex("-b"))) {
                opts[1] = true;
            } else if (regex_match(arguments[i], regex("-s"))) {
                opts[2] = true;
            } else if (regex_match(arguments[i], regex("-E"))) {
                opts[3] = true;
            } else {
                cerr << "invalid argument!" << endl;
                return;
            }
        } else if (regex_match(arguments[i], regex("--help"))) {
            cout << "cat - concatenate files and print on the standard output\n";
            cout << "With no FILE, or when FILE is -, read standard input.\n";
            cout << "-n with line number\n";
            cout << "-b with line number, but no number for empty line\n";
            cout << "-s only print one empty line if there are continues empty lines\n";
            cout << "-E each line end with '$'\n";
            return;
        } else {
            filelist.push_back(arguments[i]);
            file_start = true;
        }
    }
    if (filelist.empty()) {
        cerr << "lack of file!\n";
        return;
    }
    int num = 1;
    for (int i = 0; i < filelist.size(); ++i) {
        string P;
        P = filelist[i];
        stack<string> final_path;
        string tmp;
        if (regex_match(P, regex("-"))) {
            tmp = gTerm.strin;
            vector<string> lines;
            string buffer;
            for (int j = 0; j < tmp.size(); ++j) {
                if (tmp[j] == '\n') {
                    lines.push_back(buffer);
                    buffer.clear();
                } else {
                    buffer += tmp[j];
                }
            }
            if (!buffer.empty())lines.push_back(buffer);
            bool last_empty = false;
            for (int j = 0; j < lines.size(); ++j) {
                select_print_mode(opts, lines[j], last_empty, num);
            }
            continue;
        } else if (regex_match(P, regex("/((\\w|-|.)*/?)*(\\w|-|.)+"))) {
            string abs = gTerm.root;
            abs += P;
            vector<string> lst = splitpath(abs);
            if (!processpath(lst, final_path)) return;
            tmp = combinepath(final_path);
        } else if (regex_match(P, regex("((\\w|-|.)*/?)*(\\w|-|.)+"))) {
            string abs = gTerm.root;
            abs += "/";
            abs += gTerm.wdir;
            abs += "/";
            abs += P;
            vector<string> lst = splitpath(abs);
            if (!processpath(lst, final_path)) return;
            tmp = combinepath(final_path);
        } else {
            cerr << "invalid path or file!" << endl;
        }
        tmp = tmp.substr(0, tmp.length() - 1);
        ifstream fin(tmp);
        if (!fin) {
            cerr << tmp << ": No such file or directory\n";
        } else {
            string line;
            bool last_empty = false;
            while (!fin.eof()) {
                getline(fin, line);
                select_print_mode(opts, line, last_empty, num);
            }
        }
    }
}

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
        cout << "cd instruction is to change the working directory" << endl;
        cout << "The cd utility shall change the working directory of the current shell execution environment" << endl;
        cout << "use it like:\ncd /home/ect\n";
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

//以上仅供调试,到合起来的时候得改







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