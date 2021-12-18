#pragma once
#include "stringProcess.h"
#include "Terminal.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <utility>
using namespace std;

class File
{
    public:
    string fileName; //当前处理文件的文件名
    string wdir;
    string colorFileName; //带颜色的fileName
    int matchedNum;
    vector< pair<int,int> > matchPositions; //对于匹配行，存放其匹配子串的起始位置和终止位置，对于非匹配行，起始位置和终止位置都设置为-1
    vector<int> output; //判断该行是否要输出，如果为1代表匹配而且要输出，如果为0代表不用输出，如果为2代表不用匹配但是要输出
    vector<string> contents; //当前文件的内容，每行内容为一个字符串

    File(string Name, string Wdir); //构造函数，记录文件名，若输入的文件名是- 则将文件名置为(standard input)
    bool getContents(); //获取当前文件的内容,若成功读取则返回true，若该文件不存在则返回false
    void checkMatch(string patterns, int i_flag); //检测模式串与该文件内每一行是否匹配，并记录下匹配的位置
};