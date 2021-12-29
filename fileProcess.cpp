#include "fileProcess.h"
extern Terminal gTerm;
extern void splitByStar(string s,vector<string>& v); //将s通过*号分开，j指向待分割子串的首端，i指向待分割子串的末端
extern bool match(string P,string T,int &front, int& back, int i_flag); //检测模式串P与字符串T是否匹配，并以引用的形式给出匹配的区间段[front,back]

File::File(string Name,string Wdir):fileName(Name), wdir(Wdir)//初始化File类，记录其所对应的文件名
{

    if(fileName == "-") fileName = "(standard input)"; 
    matchedNum = 0;
}


bool File::getContents() //获取当前文件的内容
{
    if(fileName == "(standard input)")
    {
        string str = gTerm.strin;
        string tmp;
        for(int i = 0; i < str.size(); i++)
        {
            if(str[i] == '\n')
            {
                contents.push_back(tmp);
                matchPositions.push_back(make_pair(-1,-1));
                output.push_back(0);
                tmp = "";
                continue;
            }
            tmp.push_back(str[i]);
        }
        if(tmp.size()) contents.push_back(tmp);
        return true;
    }
    else
    {
        string str = wdir;
        ifstream fin(str);
        if(!fin.is_open()) return false;
        string line;
        while(getline(fin, line))
        {
            contents.push_back(line);
            matchPositions.push_back(make_pair(-1,-1));
            output.push_back(0);
        }
        fin.close();
        return true;
    }
}

void File::checkMatch(string patterns, int i_flag) //检测模式串与该文件内每一行是否匹配，并记录下匹配的位置
{
    for(int i = 0; i < contents.size(); i++)
    {
        int front, back;
        if(match(patterns, contents[i], front, back, i_flag))
        {
            matchedNum++;
            matchPositions[i].first = front;
            matchPositions[i].second = back;
            output[i] = 1;
        }
    }
}
