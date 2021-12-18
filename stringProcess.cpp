#include "stringProcess.h"

void splitByStar(string s,vector<string>& v)//将s通过*号分开，j指向待分割子串的首端，i指向待分割子串的末端
{
    int i = 0, j = 0; 
    while(i < s.size()) //逐一去扫描字符串中的每一个字符
    {
        if(s[i] != '*') //如果遇到的不是*号，就扩大待分割子串的规模
        {
            i++;
        }
        else //如果遇到的是*号
        {
            v.push_back(s.substr(j, i - j)); //先将该*号和上一个*号中间的子串分割出来
            while(i<s.size() && s[i] == '*') //如果遇到多个*号，则只需要取第一个即可，后续的*号全部跳过
            {
                j = ++i;
            }
        }
    }
    v.push_back(s.substr(j, i - j)); //将最后一个分割串的信息存储
}

int findFirstOf(string P,string T, int idx) //在文本串T中找到模式串P第一次出现的位置，(.可以匹配任意字符)
{
    T = T.substr(idx);
    int n = T.size(), i = 0;
    int m = P.size(), j;
    for(i = 0; i < n - m + 1; i++)
    {
        for(j = 0; j < m; j++)
        {
            if(T[i + j] != P[j] && P[j] != '.') break;
        }
        if(j >= m) break;
    }
    if(j >= m) return i;
    else return -1;
}

int findLastOf(string P, string T)
{
    int n = T.size(), i = 0;
    int m = P.size(), j;
    for(i = 0; i < n - m + 1; i++)
    {
        for(j = 0; j < m; j++)
        {
            if(T[n - 1 - i - j] != P[m - 1 - j] && P[m - 1 - j] != '.') break;
        }
        if(j >= m) break;
    }
    if(j >= m) return n - 1 - i;
    else return -1;
}

bool match(string P,string T,int &front, int& back, int i_flag) //检测模式串P与字符串T是否匹配，并以引用的形式给出匹配的区间段[front,back]
{
    if(i_flag == 1)
    {
        for(int i = 0; i < P.size(); i++) P[i] = tolower(P[i]);
        for(int j = 0; j < T.size(); j++) T[j] = tolower(T[j]);
    }
    vector<string> v;
    splitByStar(P,v);
    front = findFirstOf(v[0], T, 0);
    if(front == -1) return false;
    back = front + P.size() -1;
    if(v.size() == 1) return true;
    else
    {
        string last = v[v.size() - 1];
        back = findLastOf(last, T);
        if(last == "") back = T.size() - 1;//若匹配，则字符串的匹配区间位于[l,r]区间
        if(back == -1) return false;
        T = T.substr(front + v[0].size(), back - v[v.size() - 1].size() + 1 - front - v[0].size());
        int idx = 0;
        for(int i = 1; i < v.size() - 1; i++)
        {
            idx = T.find(v[i], idx);
            if(idx == -1) return false;
        }
        return true;
    }
}

string colorLine(string s, int l, int r) //在[l,r]区间内的字符标红处理,如遇到-1则不用标红直接返回即可
{
    string res="";
    if(l == -1 && r == -1) return s;
    res = s.substr(0, l);
    res.append("\e[31;1m");
    res.append(s.substr(l, r - l + 1));
    res.append("\e[0m");
    res.append(s.substr(r + 1, s.size() - r - 1));
    return res;
}

string colorFile(string fName, bool match) //输出带颜色的文件名，如果是与子串匹配的要输出filename: 如果不是匹配的输出filename-
{
    string res = "";
    res.append("\033[35m");
    res.append(fName);
    res.append("\e[92:1m");
    if(match) res.append(":");
    else res.append("-");
    res.append("\e[0m");
    return res;
}


string colorLineNum(int lineNum, bool match) //输出带颜色的行号，如果是匹配的，则输出lineNum： 如果不是匹配的则输出lineNum-
{
    string res = "";
    res.append("\e[92:1m");
    res.append(to_string(lineNum));
    if(match) res.append(":");
    else res.append("-");
    res.append("\e[0m");
    return res;
}