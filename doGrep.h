#pragma once
#include "fileProcess.h"
#include "Terminal.h"
#include <cstring>
#include <fstream>
#include <vector>
using namespace std;


static int c_flag; //1代表命令中含有-c,应输出计算符合样式的行数
static int h_flag; //0和-1代表不显示所属的文件名称（-1为默认值），1代表输出所属的文件名称
static int n_flag; //0代表不用输出对应的行号，1代表要输出对应的行号
static int i_flag; //0代表区分大小写，1代表不区分大小写
static int A_num; //代表输出符合范本样式后的行数
static int B_num; //代表输出符合范本样式前的行数

void doHelp(); //grep --help 指令的实现，用于显示帮助

void init(); //在每次解析命令行之前，先将c_flag,h_flag,i_flag,A_num,B_num初始化

int commandLineParse(int argc,char * argv[], vector<File>& file, string& p); //对命令行参数进行解析.若为非法输入则返回0;若为help指令,返回2；其余返回1

void doGrep(int argc, char * argv[]); //grep指令函数，将按照传入的参数进行指令处理，并将结果输出到strout

