#include "doCp.h"

void init_n()
{
    n_flag = 0;
}

void doCp(int argc, char * argv[])
{
    init_n();
    if(argc < 2) //命令不完整的case
    {
        string info = "cp: missing file operand";
        info += "\n";
        info += "Try 'cp --help' for more information.\n";
        cerr << info << endl;
        return ;
    }
    if(argv[1][0] == '-') 
    {
        if(strlen(argv[1]) >= 3 && argv[1][1] == '-' && argv[1][2] == 'h')// '--help'指令,--h也可以视作--help
        {
            string info = "Usage: cp [OPTION]... SOURCE DEST\n";
            strcpy(gTerm.strout, info.c_str());
            return ;
        }
        if(strlen(argv[1]) < 2 || argv[1][1] != 'n')
        {
            cerr << "cp: illegal option " << argv[1] << endl;
            return ;
        }
        if(argv[1][1] == 'n') 
        {
            n_flag = 1;
        }
    }
    if(argc - n_flag < 3) //没输入俩文件的情况
    {
        cerr << "usage: [-n] source_file target_file" << endl;
        return;
    }
    string file[2]; //file1：source_file,file2:target_file;
    for(int i = 0; i < 2; i++)
    {
        if(argv[i + n_flag + 1][0] == '/') //从根目录开始读
        {
            file[i] = gTerm.root;
            file[i] += argv[i + n_flag + 1];
        }
        else //从相对目录读
        {
            file[i] = gTerm.root;
            file[i] += gTerm.wdir;
            file[i] += "/";
            file[i] += argv[i + n_flag + 1];
        }
    }
    ifstream fin(file[0]); //source_file
    if(!fin.is_open())
    {
        cerr << "cp: " << argv[1 + n_flag] << ": No such file or directory" << endl;
        return;
    }
    string source;
    string line;
    while(getline(fin, line))
    {
        source += line;
        source += "\n";
    }
    fin.close();
    ofstream fout; //target_file
    if(n_flag == 0) fout.open(file[1]);
    else fout.open(file[1],ios::app);
    fout << source;
    fout.close();
}