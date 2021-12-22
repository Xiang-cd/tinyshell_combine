#pragma once //防止头文件被多次包含

#include <iostream>
#include <regex>
#include <string>
using namespace std;

const int MAXLINE=2e3;
const int MAXFILE=5e6;
const int MAXARG=20;

extern void doEcho(int argc, char *argv[]);
extern void doGrep(int argc, char *argv[]);
extern void doDiff(int argc,char* argv[]);
extern void doPwd(int argc, char* argv[]);
extern void doCp(int argc,char* argv[]);
extern void doTee(int argc,char* argv[]);

struct Terminal {
    char user[MAXLINE]; // 用户名
    char mach[MAXLINE]; // 计算机名
    char root[MAXLINE]; // 根目录
    char wdir[MAXLINE]; // 工作目录
    char strin[MAXFILE]; // 重定向标准输入
    char strout[MAXFILE]; // 重定向标准输出
    int theme;
};

inline void printColor(const string &s, int front, int color, bool light );

inline void printTheme(const string &a, int front, int color, const string &b, int front2, int color2,bool light);

void printHeading(bool right);

inline string BasicInit(const string &input, const regex &ex);

inline void TestArg(int argc, char *argv[]);

void GetAccountInit();

void doVim(int argc, char *argv[]);

bool selectInstr();

void proceseInstr(string tmp);

bool splitInstr(string tmp);





