#include "doTee.h"

bool debug = false;
char file111[1000][MAXLINE] = {0};

void doTee(int argc, char *argv[]) {
    strcpy(gTerm.strout, gTerm.strin);
    if (debug) {
        cout << gTerm.strout;
    }
    if (argc == 1) return;
    bool command = false;
    int i = 1;//循环变量
    if (strcmp(argv[1], "-a") == 0) {
        command = true;
        i++;//第二个输入是指令，从下一个开始循环
    }
    //开始读取信息到file1里
    int hang = 1;
    int y = 0;
    int maxline = 0;
    while (true) {
        int z = 0;
        while (gTerm.strin[y] != '\n' and gTerm.strin[y] != '\0') {
            *(file111[hang] + z) = gTerm.strin[y];
            y++;
            z++;
        }
        hang++;
        y++;
        if (gTerm.strin[y] == '\0') {
            break;
        }
    }
    maxline = hang - 1;

    if (debug) {
        for (int j = 1; j <= maxline; j++) {
            cout << file111[j] << endl;
        }
    }

    char r1[100] = {0}, w1[100] = {0};
    strcpy(w1, gTerm.wdir);
    for (; i < argc; i++) {
        string P1 = argv[i];
        stack<string> final_path2;
        string tmp1;
        if (regex_match(P1, regex("/((\\w|-|.)+/)*(\\w|-|.)+"))) {  // 这里是指根目录的情况
            string abs = gTerm.root;  // 先封装成string
            abs += P1;   // p就是你要处理的参数，在你那里是取filelsit【0】
            vector<string> lst = splitpath(abs); // 调用分割函数，会将路径分割成每一级
            if (!processpath(lst, final_path2)) return;  // 处理路径，合并// . 等选项，如果出现非法路径，会自动报错，所以要保留
            tmp1 = combinepath(final_path2); //合并路径，将路径各级选项合并，赋值给tmp，最后你只要打开tmp这个字符串就可以了
        } else if (regex_match(P1, regex("((\\w|-|.)+/)*(\\w|-|.)+"))) { // 这里是指相对路径的情况
            string abs = gTerm.root;
            abs += "/";
            abs += gTerm.wdir;
            abs += "/";
            abs += P1;
            vector<string> lst = splitpath(abs);
            if (!processpath(lst, final_path2)) return;
            tmp1 = combinepath(final_path2);
        } else {
            cerr << "invalid path or file!" << endl;
        }
        tmp1 = tmp1.substr(0, tmp1.length() - 1);  // 但是要注意这里，合并完之后的最后一个字符是‘/’, 所以我吧它去掉了

        if (debug) {
            cout << "路径：" << endl << tmp1 << endl;
        }
        if (command) {
            ifstream ceshi(tmp1);
            if (!ceshi) {
                cerr << "doTee:" << argv[i] << "不存在的文件" << endl;
                ceshi.close();
            } else {
                ceshi.close();
                ofstream wenjian(tmp1, ios::app);
                for (int j = 1; j <= maxline; j++) {
                    wenjian << file111[j] << endl;
                }
                wenjian.close();
            }
        } else {
            ofstream wenjian(tmp1);
            for (int j = 1; j <= maxline; j++) {
                wenjian << file111[j] << endl;
            }
            wenjian.close();
        }
    }
}