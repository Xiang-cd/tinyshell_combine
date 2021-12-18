#pragma once
#include <iostream>
#include <vector>
using namespace std;

void splitByStar(string s,vector<string>& v); //将s通过*号分开

int findFirstOf(string P,string T, int idx); //在文本串T中找到模式串P第一次出现的位置，(.可以匹配任意字符)

int findLastOF(string P,string T); //在文本串T中找到模式串P最后一次出现的位置（.）可以匹配任意元素

bool match(string P,string T,int &front, int& back, int i_flag); //检测模式串P与字符串T是否匹配，并以引用的形式给出匹配的区间段[front,back]

string colorLine(string s, int l, int r); //在[l,r]区间内的字符标红处理,如遇到l=r=-1则不用标红直接返回即可

string colorFile(string fName, bool match); //输出带颜色的文件名，如果是与子串匹配的要输出filename: 如果不是匹配的输出filename-

string colorLineNum(int lineNum, bool match); //输出带颜色的行号，如果是匹配的，则输出lineNum： 如果不是匹配的则输出lineNum-