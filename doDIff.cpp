#include "doDiff.h"

void doDiff(int argc, char *argv[]) {
    if (argc < 3 && strcmp(argv[1], "--help") != 0) {
        cerr << "diff: 不符合要求的参数" << endl;
        return;
    }
    //准备工作：
    bool command[5] = {false};        //创建布尔数组储存用户是否有-b,-B等特殊要求
    struct zifu {                           //储存用户是否有-I的要求
        bool I;                             //是否有-I
        char zifu[20];                      //-I是什么
    };
    zifu command_I;
    command_I.I = false;                             //初始化-I

    if (argc == 2 && strcmp(argv[1], "--help") == 0)         //用户执行了“帮助”指令，输出帮助直接返回。
    {
        strcpy(gTerm.strout,
               "帮助：\n本指令可逐行比较两个文件，特殊的比较规则为：\n-b：不检查空格字符的不同。\n-B：不检查空白行。\n- i：不检查大小写的不同。\n- q：仅显示有无差异，不显示详细的信息。无差异时不显示，有差异时显示“File[文件 1] and [文件 2] differ”。\n- w：忽略全部的空格字符。\n- I[字符串]：若两个文件在某几行有所不同，但这几行同时都包含了选项中指定字符串，则不显示这两个文件的差异。\n");
        return;
    }
    for (int i = 1; i < argc - 2; i++)           //在布尔数组里储存特殊要求
    {
        if (strcmp(argv[i], "-b") == 0) command[0] = true;         //[0]储存-b
        else if (strcmp(argv[i], "-B") == 0) command[1] = true;         //[1]储存-B
        else if (strcmp(argv[i], "-i") == 0) command[2] = true;         //[2]储存-i
        else if (strcmp(argv[i], "-q") == 0) command[3] = true;         //[3]储存-q
        else if (strcmp(argv[i], "-w") == 0) command[4] = true;         //[4]储存-w
        else if (argv[i][0] == '-' && argv[i][1] == 'I') {
            command_I.I = true;
            strcpy(command_I.zifu, argv[i] + 2);
        } else {
            cerr << "diff: " << argv[i] << ": No such command" << endl;
            return;
        }
    }
    if (test) {
        //输出记录结果测试：
        for (int i = 0; i <= 4; i++) {
            cout << command[i] << endl;
        }
        cout << command_I.I << " " << command_I.zifu << endl;
        //system("pause");
    }

    const int hangshu = 20, lieshu = 20;  //可根据需要修改
    char file1[hangshu][lieshu] = {};
    char file2[hangshu][lieshu] = {};
    int maxlinea = 0;    //统计test1的行数
    int maxlineb = 0;    //统计test2的行数
    //char patha[200], pathb[200];
    char r[100] = {0}, w[100] = {0};
    //strcpy(r, gTerm.root);
    strcpy(w, gTerm.wdir);
    if (argv[argc - 2][0] == '/') {//根目录
        strcpy(r, gTerm.root);
        strcat(r, argv[argc - 2]);
        strcat(r, "txt");
    } else {//相对路径
        //strcat(r, "/");
        strcat(r, w);
        strcat(r, "/");
        strcat(r, argv[argc - 2]);
        //jdstrcat(r, ".txt");
    }
    if (Debug)cout << r << endl;
    if (strcmp(argv[argc - 2],"-") !=0) {
        ifstream test1(r);
        if (!test1) {
            cerr << "diff: " << argv[argc - 2] << ": No such file or directoty" << endl;
            return;
        }
        int hang = 1;
        while (test1.getline(file1[hang], 20)) {
            hang++;
        }
        maxlinea = hang - 1;
    } else {
        int hang = 1;
        int y = 0;
        while (true) {
            int z = 0;
            while (gTerm.strin[y] != '\n') {//见到\n换行，见到\0结束
                *(file1[hang] + z) = gTerm.strin[y];
                y++;
                z++;
            }
            hang++;
            y++;
            if (gTerm.strin[y] == '\0') {
                break;
            }
        }
        maxlinea = hang - 1;
    }
    char r1[100] = {0}, w1[100] = {0};
    //memcpy(r1, gTerm.root,strlen(gTerm.root));
    // strcpy(r1, gTerm.root);
    strcpy(w1, gTerm.wdir);
    if (argv[argc - 1][0] == '/') {//根目录
        strcpy(r1, gTerm.root);
        strcat(r1, argv[argc - 1]);
        strcat(r1, "txt");
    } else {//相对路径
        //strcat(r1, "/");
        strcat(r1, w1);
        strcat(r1, "/");
        strcat(r1, argv[argc - 1]);
        //strcat(r1, ".txt");
    }
    if (Debug)cout << r1 << endl;
    if (strcmp(argv[argc - 1] ,"-")!=0) {
        ifstream test2(r1);
        if (!test2) {
            cerr << "diff: " << argv[argc - 1] << ": No such file or directoty" << endl;
            return;
        }
        int hang = 1;
        while (test2.getline(file2[hang], 20)) {
            hang++;
        }
        maxlineb = hang - 1;
    } else if (strcmp(argv[argc - 2],"-") !=0) {//文件二需要从标准输入读取
        int hang = 1;
        int y = 0;
        while (true) {
            int z = 0;
            while (gTerm.strin[y] != '\n') {
                *(file2[hang] + z) = gTerm.strin[y];
                y++;
                z++;
            }
            hang++;
            y++;
            if (gTerm.strin[y] == '\0') {
                break;
            }
        }
        maxlineb = hang - 1;
    } else {//两个文件均需要从标准输入读取
        int hang = 1;
        int y = 0;
        while (gTerm.strin[y] != '\0') {
            y++;
        }
        y++;
        while (true) {
            int z = 0;
            while (gTerm.strin[y] != '\n') {
                *(file2[hang] + z) = gTerm.strin[y];
                y++;
                z++;
            }
            hang++;
            y++;
            if (gTerm.strin[y] == '\0') {
                break;
            }
        }
        maxlineb = hang - 1;
    }


    //在每一行末尾补充回车：
    for (int i = 1; i <= maxlinea; i++) {
        strcat(file1[i], "\n");
    }
    for (int i = 1; i <= maxlineb; i++) {
        strcat(file2[i], "\n");
    }


    //文件读取测试
    if (test) {
        for (int i = 1; i <= maxlinea; i++) {
            cout << file1[i];
        }
        cout << endl;
        for (int i = 1; i <= maxlineb; i++) {
            cout << file2[i];
        }
        cout << endl;
    }


    //开始预处理文件，第一步：复制一份
    char file1_c[hangshu][lieshu];
    char file2_c[hangshu][lieshu];
    for (int i = 0; i < hangshu; i++) {
        for (int j = 0; j < lieshu; j++) {
            file1_c[i][j] = file1[i][j];
            file2_c[i][j] = file2[i][j];
        }
    }

    //文件复制测试
    if (test) {
        cout << "文件复制测试" << endl;
        for (int i = 1; i <= maxlinea; i++) {
            cout << file1_c[i];
        }
        cout << endl;
        for (int i = 1; i <= maxlineb; i++) {
            cout << file2_c[i];
        }
        cout << endl;
        //system("pause");
    }

    if (command[4] == 1) {    //忽略全部空格字符
        for (int i = 1; i <= maxlinea; i++) {
            int j = 0;
            while (file1_c[i][j] != '\n') {
                if (file1_c[i][j] == ' ') {//见到空格，把它后面的东西全都前提一位，覆盖掉空格
                    int k = j;
                    while (file1_c[i][k] != '\n') {
                        file1_c[i][k] = file1_c[i][k + 1];
                        k++;
                    }
                    file1_c[i][k] = 0;
                }
                if (file1_c[i][j] != ' ' && file1_c[i][j] != '\n')j++;
            }
        }
        for (int i = 1; i <= maxlineb; i++) {//操作文件二，与一同理
            int j = 0;
            while (file2_c[i][j] != '\n') {
                if (file2_c[i][j] == ' ') {
                    int k = j;
                    while (file2_c[i][k] != '\n') {
                        file2_c[i][k] = file2_c[i][k + 1];
                        k++;
                    }
                    file2_c[i][k] = 0;
                }
                if (file2_c[i][j] != ' ' && file2_c[i][j] != '\n')j++;
            }
        }
    }

    if (test) {
        //文件预处理测试
        cout << "忽略所有空格" << endl;
        for (int i = 1; i <= maxlinea; i++) {
            cout << file1_c[i];
        }
        cout << endl;
        for (int i = 1; i <= maxlineb; i++) {
            cout << file2_c[i];
        }
        cout << endl;
        //system("pause");
    }

    if (command[0] == 1) {    //不检查空格字符的不同
        for (int i = 1; i <= maxlinea; i++) {
            int j = 0;
            while (file1_c[i][j] != '\n') {
                if (file1_c[i][j] == ' ') {//见到空格字符，把它后面紧挨着的空格字符全都去掉
                    while (file1_c[i][j + 1] == ' ') {
                        int k = j;
                        while (file1_c[i][k] != '\n') {
                            file1_c[i][k] = file1_c[i][k + 1];
                            k++;
                        }
                        file1_c[i][k] = 0;
                    }
                }
                j++;
            }
        }
        for (int i = 1; i <= maxlineb; i++) {//操作文件二
            int j = 0;
            while (file2_c[i][j] != '\n') {
                if (file2_c[i][j] == ' ') {
                    while (file2_c[i][j + 1] == ' ') {
                        int k = j;
                        while (file2_c[i][k] != '\n') {
                            file2_c[i][k] = file2_c[i][k + 1];
                            k++;
                        }
                        file2_c[i][k] = 0;
                    }
                }
                j++;
            }
        }
    }

    if (test) {
        //文件预处理测试
        cout << "不检查空格字符的不同" << endl;
        for (int i = 1; i <= maxlinea; i++) {
            cout << file1_c[i];
        }
        cout << endl;
        for (int i = 1; i <= maxlineb; i++) {
            cout << file2_c[i];
        }
        cout << endl;
        //system("pause");
    }


    if (command[2] == 1) {    //不检查大小写
        for (int i = 0; i < hangshu; i++) {
            for (int j = 0; j < lieshu; j++) {//利用ASKII码减去32把小写全都变大写
                if (file1_c[i][j] >= 97 && file1_c[i][j] <= 122) {
                    file1_c[i][j] -= 32;
                }
                if (file2_c[i][j] >= 97 && file2_c[i][j] <= 122) {
                    file2_c[i][j] -= 32;
                }
            }
        }
        for (int j = 0; j < 20; j++) {  //将-I中的字符也全变成大写
            if (command_I.zifu[j] >= 97 && command_I.zifu[j] <= 122) {
                command_I.zifu[j] -= 32;
            }
        }
    }

    if (test) {
        //文件预处理测试
        cout << "不检查大小写" << endl;
        for (int i = 1; i <= maxlinea; i++) {
            cout << file1_c[i];
        }
        cout << endl;
        for (int i = 1; i <= maxlineb; i++) {
            cout << file2_c[i];
        }
        cout << endl;
        //system("pause");
    }

    if (command_I.I == 1) {    //含有指定字符串视为相同。
        int length = 0;
        for (int i = 0; i <= 20; i++) {//先测出指定字符串的长度。
            if (command_I.zifu[i] != '\0') {
                length++;
            } else break;
        }
        for (int i = 1; i <= maxlinea; i++) {
            int j = 0;
            while (file1_c[i][j] != '\n') {   //可能引发越界，但不影响结果，因为目标字符串不会含有\n
                int tong = true;
                for (int k = 0; k < length; k++) {
                    if (file1_c[i][j + k] != command_I.zifu[k]) {
                        tong = false;
                        break;
                    }
                }
                if (tong) {//从j开始的length长度字符串恰好为目标
                    strcpy(file1_c[i], command_I.zifu);
                    strcat(file1_c[i], "\n\0");
                    break;
                }
                j++;
            }
        }
        for (int i = 1; i <= maxlineb; i++) {//与文件一同理
            int j = 0;
            while (file2_c[i][j] != '\n') {
                int tong = true;
                for (int k = 0; k < length; k++) {
                    if (file2_c[i][j + k] != command_I.zifu[k]) {
                        tong = false;
                        break;
                    }
                }
                if (tong) {
                    strcpy(file2_c[i], command_I.zifu);
                    strcat(file2_c[i], "\n\0");
                }
                j++;
            }
        }
    }

    if (test) {
        //文件预处理测试
        cout << "含有" << command_I.zifu << "视为相同" << endl;
        for (int i = 1; i <= maxlinea; i++) {
            cout << file1_c[i];
        }
        cout << endl;
        for (int i = 1; i <= maxlineb; i++) {
            cout << file2_c[i];
        }
        cout << endl;
        //system("pause");
    }


    int konga = 0, kongb = 0;
    int a[21], b[21];
    for (int i = 1; i <= 20; i++) {
        a[i] = i;
        b[i] = i;
    }
    if (command[1] == 1) {    //不检查空白行
        char tempfile1[hangshu][lieshu] = {};
        char tempfile2[hangshu][lieshu] = {};
        for (int i = 1; i <= maxlinea; i++) {
            if (strcmp(file1_c[i], "\n") != 0) {//不是空白行，放到tempfile里
                strcpy(tempfile1[i - konga], file1_c[i]);
            } else {//是空白行，不要它，但是a[j]需要发生对应的变化以保证tempfile第i行对应原文件的第a[i]行
                for (int j = i - konga++; j <= maxlinea; j++) {
                    a[j] = j + konga;
                }
            }
        }
        for (int i = 1; i <= maxlinea - konga; i++) {//tempfile回到file_c中
            strcpy(file1_c[i], tempfile1[i]);
        }
        for (int i = 1; i <= maxlineb; i++) {//操作文件二，同理
            if ((strcmp(file2_c[i], "\n") != 0)) {
                strcpy(tempfile2[i - kongb], file2_c[i]);
            } else {

                for (int j = i - kongb++; j <= maxlineb; j++) {
                    b[j] = j + kongb;
                }
            }
        }
        for (int i = 1; i <= maxlineb - kongb; i++) {
            strcpy(file2_c[i], tempfile2[i]);
        }
    }

    if (test) {
        //a[i]与b[i]对应关系测试
        cout << "不检查空白行" << endl;
        for (int i = 1; i <= maxlinea - konga; i++) {
            cout << a[i] << endl;
        }
        cout << endl;
        for (int i = 1; i <= maxlineb - kongb; i++) {
            cout << b[i] << endl;
        }
        cout << endl;
        //system("pause");
    }

    //文件预处理测试
    if (test) {
        for (int i = 1; i <= maxlinea - konga; i++) {
            cout << file1_c[i];
        }
        cout << endl;
        for (int i = 1; i <= maxlineb - kongb; i++) {
            cout << file2_c[i];
        }
        cout << endl;
        //system("pause");
    }

    if (command[3] == 1) {
        for (int i = 1; i <= maxlinea - konga; i++) {
            int spot = 0;   //记录同一行中正在比较的位置
            bool same = true;
            if (maxlinea - konga != maxlineb - kongb)same = false;
            while (file1_c[i][spot] == file2_c[i][spot]) {
                if (file1_c[i][spot] == '\0') {
                    break;
                }
                spot++;
            }
            if (file1_c[i][spot] != file2_c[i][spot]) same = false;//因字符不同而退出循环
            if (same) {//只要有一行不同，就会执行下面的return，只有每一行都相同，才能用continue跳过下面的语句
                continue;
            }
            strcat(gTerm.strout, "File [");
            strcat(gTerm.strout, argv[argc - 2]);
            strcat(gTerm.strout, "] and [");
            strcat(gTerm.strout, argv[argc - 1]);
            strcat(gTerm.strout, "] differ\n");
            return;
        }
        return;
    }

    bool sameline[20][20];
    for (int i = 1; i <= maxlinea; i++) {
        for (int j = 1; j <= maxlineb; j++) {
            sameline[i][j] = 0;
        }
    }
    for (int i = 1; i <= maxlinea - konga; i++) {
        for (int j = 1; j <= maxlineb - kongb; j++) {//测试文件一的第i行与文件二的第j行是否相同（手动实现strcmp)
            bool same = true;
            int spot = 0;
            while (file1_c[i][spot] == file2_c[j][spot]) {
                if (file1_c[i][spot] == '\0') {
                    break;
                }
                spot++;
            }
            if (file1_c[i][spot] != file2_c[j][spot]) same = false;
            if (same) sameline[i][j] = 1;
        }
    }

    if (test) {
        //统计结果测试
        cout << "统计结果测试" << endl;
        for (int i = 1; i <= maxlinea - konga; i++) {
            for (int j = 1; j <= maxlineb - kongb; j++) {
                cout << sameline[i][j] << " ";
            }
            cout << endl;
        }
    }

    //开始递归找最大值
    bool aim[20][20];
    memset(aim,0,20*20);

    int a1 = 0, b1 = 0;
    int choose(int a, int b, int maxlinea, int maxlineb, bool sameline[20][20], bool aim[20][20]);
    choose(1, 1, maxlinea - konga, maxlineb - kongb, sameline, aim);

    if (test) {
        //测试目标结果
        cout << "寻找最优解测试" << endl;
        cout << endl;
        for (int i = 1; i <= maxlinea; i++) {
            for (int j = 1; j <= maxlineb; j++) {
                cout << aim[i][j] << " ";
            }
            cout << endl;
        }
        //system("pause");
    }

    //开始输出：
    maxlinea -= konga;
    maxlineb -= kongb;
    int lineofb = 1;
    for (int i = 1; i <= maxlinea; i++) {//先找有没有相同行
        bool same = false;
        int sameofb = 0;
        for (int j = 1; j <= maxlineb; j++) {
            if (aim[i][j] == 1) {
                same = true;
                sameofb = j;
                break;
            }
        }

        if (!same) {//没有相同行，把它删了
            char out[8];
            char out0[5] = {'d', '\0'};
            char out1[8];
            sprintf(out, "%d", a[i]);
            sprintf(out1, "%d", b[lineofb]);
            strcat(out1, "\n");
            strcat(gTerm.strout, out);
            strcat(gTerm.strout, out0);
            strcat(gTerm.strout, out1);
            char out2[2] = {'<', '\0'};
            char out3[2] = {'\n', '\0'};
            strcat(gTerm.strout, out2);
            strcat(gTerm.strout, file1[a[i]]);
            //strcat(gTerm.strout, out3);
            for (i++; i <= maxlinea; i++) {//一并把它后面紧挨着的没有相同行的东西删了
                bool same = false;
                for (int j = 1; j <= maxlineb; j++) {
                    if (aim[i][j] == 1) {
                        same = true;
                        break;
                    }
                }
                if (!same) {
                    strcat(gTerm.strout, out2);
                    strcat(gTerm.strout, file1[a[i]]);
                    //strcat(gTerm.strout, out3);
                } else {
                    i--;
                    break;
                }
                if (i == maxlinea)break;
            }
        } else if (lineofb == sameofb) {//恰好相同，不输出
            lineofb++;
        } else if (lineofb < sameofb) {//b中多了好几行，全加进来
            char out[8];
            char out0[5] = {'a', '\0'};
            char out1[8];
            sprintf(out, "%d", a[i]);
            sprintf(out1, "%d", b[lineofb]);
            strcat(out1, "\n");
            strcat(gTerm.strout, out);
            strcat(gTerm.strout, out0);
            strcat(gTerm.strout, out1);
            char out2[2] = {'\n', '\0'};
            char out3[2] = {'>', '\0'};
            //strcat(gTerm.strout, out);
            for (; lineofb < sameofb; lineofb++) {//全部加入
                strcat(gTerm.strout, out3);
                strcat(gTerm.strout, file2[b[lineofb]]);
                //strcat(gTerm.strout, out2);
            }
            lineofb++;
        }
        if (i == maxlinea && lineofb != maxlineb + 1) {//文件一已经结束，但文件二还有空余行，加进来
            char out[8];
            char out0[5] = {'a', '\0'};
            char out1[8];
            sprintf(out, "%d", a[i]);
            sprintf(out1, "%d", b[lineofb]);
            strcat(out1, "\n");
            char out2[2] = {'\n', '\0'};
            char out3[2] = {'>', '\0'};
            strcat(gTerm.strout, out);
            strcat(gTerm.strout, out0);
            strcat(gTerm.strout, out1);
            for (; lineofb <= maxlineb; lineofb++) {
                strcat(gTerm.strout, out3);
                strcat(gTerm.strout, file2[b[lineofb]]);
            }
        }
    }
}

int choose(int a, int b, int maxlinea, int maxlineb, bool sameline[20][20], bool aim[20][20]) {//递归思路见说明文档
    bool tong = false;
    int b2 = 0;
    for (int i = b; i <= maxlineb; i++) {
        if (sameline[a][i] == 1) {
            tong = true;
            b2 = i;
            break;
        }
    }
    if (a == maxlinea) {
        if (tong) {
            aim[a][b2] = 1;
            return 1;
        } else return 0;
    }
    if (!tong) {
        return choose(a + 1, b, maxlinea, maxlineb, sameline, aim);
    } else {
        if (choose(a + 1, b, maxlinea, maxlineb, sameline, aim) <=
            choose(a + 1, b2 + 1, maxlinea, maxlineb, sameline, aim) + 1) {
            for (int j = a; j <= maxlinea; j++) {//如果上一行中后者比较大，我实际上不应该执行前者，于是我重新赋0以抵消执行它的影响。
                for (int i = 1; i <= maxlineb; i++) {
                    aim[j][i] = false;
                }
            }
            aim[a][b2] = true;
            return choose(a + 1, b2 + 1, maxlinea, maxlineb, sameline, aim) + 1;
        } else {//同理，我不应当执行后者，赋0来抵消
            for (int j = a; j <= maxlinea; j++) {
                for (int i = 1; i <= maxlineb; i++) {
                    aim[j][i] = false;
                }
            }
            return choose(a + 1, b, maxlinea, maxlineb, sameline, aim);
        }
    }
}

