//
// Created by 项小羽 on 2022/1/1.
//

#include "doCat.h"

char tmp_for_p[MAXLINE] = {};

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
