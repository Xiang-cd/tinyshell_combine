#include "doGrep.h"

extern Terminal gTerm;

void doHelp() //grep --help 指令的实现，用于显示帮助
{
    ifstream fin;
    fin.open("help.txt");
    fin.seekg(0, fin.end);
    int len = fin.tellg();
    fin.seekg(0, fin.beg);
    fin.read(gTerm.strout, len);
}

void init() //在每次解析命令行之前，先将c_flag,h_flag,i_flag,A_num,B_num初始化
{
    c_flag = 0;
    h_flag = -1;
    i_flag = 0;
    n_flag = 0;
    A_num = 0;
    B_num = 0;
}

int commandLineParse(int argc, char *argv[], vector<File> &file, string &p) //对命令行参数进行解析.若为非法输入则返回0;若为help指令,返回2；其余返回1
{
    int flag = 0;
    if (strcmp(argv[1], "--help") == 0) {
        return 2;
    }
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-' && flag == 0) {
            switch (argv[i][1]) {
                case 'c': {
                    c_flag = 1;
                    break;
                }
                case 'h': {
                    h_flag = 0;
                    break;
                }
                case 'H': {
                    h_flag = 1;
                    break;
                }
                case 'n': {
                    n_flag = 1;
                    break;
                }
                case 'i': {
                    i_flag = 1;
                    break;
                }
                case 'A': {
                    A_num = 0; //若多次出现-A指令，则以后出现的为准
                    if (strlen(argv[i]) <= 2) //非法输入
                    {
                        cerr << "grep: " << argv[i + 1] << ": ivalid context length argument" << endl;
                        return false;
                    }
                    for (int j = 2; j < strlen(argv[i]); j++) {
                        if (argv[i][j] >= '0' && argv[i][j] <= '9') //判断-A后所接的是否是合法的行数输入
                        {
                            A_num *= 10;
                            A_num += argv[i][j] - '0';
                        } else //若不是合法的行数输入，直接返回false
                        {
                            cerr << "grep: " << argv[i] + 2 << ": ivalid context length argument" << endl;
                            return false;
                        }
                    }
                    break;
                }
                case 'B': {
                    B_num = 0; //若出现多次-B指令，则以后出现的为准
                    if (strlen(argv[i]) <= 2) //非法输入
                    {
                        cerr << "grep: " << argv[i + 1] << ": ivalid context length argument" << endl;
                        return false;
                    }
                    for (int j = 2; j < strlen(argv[i]); j++) {
                        if (argv[i][j] >= '0' && argv[i][j] <= '9') //判断-A后所接的是否是合法的行数输入
                        {
                            B_num *= 10;
                            B_num += argv[i][j] - '0';
                        } else //若不是合法的行数输入，直接返回false
                        {
                            cerr << "grep: " << argv[i] + 2 << ": ivalid context length argument" << endl;
                            return false;
                        }
                    }
                    break;
                }
                default: {
                    cerr << "grep: invalid option -" << argv[i][1]
                         << endl << "Usage: grep [OPTION]... PATTERNS [FILE]"
                         << endl << "Try \'grep --help\' for more information."
                         << endl;
                    return false;
                }
            }

        } else {
            if (flag == 1) //当flag为1的时候表示开始输入文件
            {
                string fName;
                fName = argv[i];
                string Dir;
                if (argv[i][0] == '/') //表示从根目录开始
                {
                    Dir = gTerm.root;
                    Dir += argv[i];
                } else if (argv[i][0] == '-') {} //表示从标准输入读取
                else //表示从相对路径读取
                {
                    Dir = gTerm.root;
                    Dir += gTerm.wdir;
                    Dir += "/";
                    Dir += argv[i];
                }
                File f(fName, Dir);
                if (!f.getContents()) //表明文件不能正确打开，则直接输出错误并return 0
                {
                    cerr << "grep: " << fName << ": " << "No such file or directory" << endl;
                } else {
                    file.push_back(f);
                }
            }
            if (flag == 0) //当flag为0的时候，表示开始输入模式串
            {
                p = argv[i];
                flag = 1;
            }
        }
    }
    return 1;
}

void doGrep(int argc, char *argv[]) //grep指令的主函数
{
    init();
    vector<File> fileVec;
    string patterns;
    string output = "";

    int flag = commandLineParse(argc, argv, fileVec, patterns);
    if (flag == 0) return; //当输入非法，不用进行后续操作
    else if (flag == 2) doHelp(); //commandLinePasrse返回2时，表明处理grep --help指令
    else {
        for (int i = 0; i < fileVec.size(); i++) fileVec[i].checkMatch(patterns, i_flag);
        if (c_flag == 1) //只需要输出每个文件成功匹配的行数
        {
            for (int i = 0; i < fileVec.size(); i++) {
                string num;
                num = to_string(fileVec[i].matchedNum);
                if ((fileVec.size() == 1 && h_flag == 1) ||
                    (fileVec.size() > 1 && h_flag != 0)) //需要输出文件名的情况：单文件且带-H指令、多文件且不带-h指令
                {
                    strcat(gTerm.strout, colorFile(fileVec[i].fileName, true).c_str());
                    // output.append(colorFile(fileVec[i].fileName,true));
                }
                num.push_back('\n');
                strcat(gTerm.strout, num.c_str());

            }
        } else //需要输出对应的行具体是什么
        {
            //先处理要输出哪些行
            for (int i = 0; i < fileVec.size(); i++) //处理-A[行数]和-B[行数]
            {
                for (int j = 0; j < fileVec[i].contents.size(); j++) {
                    if (fileVec[i].output[j] == 1) {
                        for (int k = 1; k <= A_num && k + j < fileVec[i].output.size(); k++) //处理-A
                        {
                            if (fileVec[i].output[k + j] == 1) //保证不冗余输出
                            {
                                break;
                            } else {
                                fileVec[i].output[k + j] = 2;
                            }
                        }
                        for (int k = 1; k <= B_num && j - k >= 0; k++) //处理-B
                        {
                            if (fileVec[i].output[j - k] == 1) //保证不冗余输出
                            {
                                break;
                            } else {
                                fileVec[i].output[j - k] = 2;
                            }
                        }
                    }
                }

            }
            for (int i = 0; i < fileVec.size(); i++) {
                for (int j = 0; j < fileVec[i].contents.size(); j++) //查看第j-1行
                {
                    if (fileVec[i].output[j] == 1) //该行是匹配的,要输出
                    {
                        if ((fileVec.size() == 1 && h_flag == 1) ||
                            (fileVec.size() > 1 && h_flag != 0)) //需要输出文件名的情况：单文件且带-H指令、多文件且不带-h指令
                        {
                            strcat(gTerm.strout, colorFile(fileVec[i].fileName, true).c_str());

                        }
                        if (n_flag == 1) //要输出行号的情况
                        {
                            strcat(gTerm.strout, colorLineNum(j + 1, true).c_str());

                        }

                        strcat(gTerm.strout, colorLine(fileVec[i].contents[j], fileVec[i].matchPositions[j].first,
                                                       fileVec[i].matchPositions[j].second).c_str());
                        strcat(gTerm.strout, "\n");


                    } else if (fileVec[i].output[j] == 2) //该行是不匹配的，但是要输出
                    {
                        if ((fileVec.size() == 1 && h_flag == 1) ||
                            (fileVec.size() > 1 && h_flag != 0)) //需要输出文件名的情况：单文件且带-H指令、多文件且不带-h指令
                        {
                            strcat(gTerm.strout, colorFile(fileVec[i].fileName, false).c_str());

                        }
                        if (n_flag == 1) //要输出行号的情况
                        {
                            strcat(gTerm.strout, colorLineNum(j + 1, false).c_str());

                        }
                        strcat(gTerm.strout, fileVec[i].contents[j].c_str());
                        strcat(gTerm.strout, "\n");

                    } else //该行是不匹配的而且不用输出
                    {
                        continue;
                    }
                }
            }

        }

    }

}

