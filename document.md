# 程序设计大作业说明文档
 [toc]

## 成员介绍

### 成员列表

冯宁亚

### 分工情况

冯宁亚——diff

## 功能实现

### 基本功能

本项目通过解释器框架将输入的行分解为指令, 指令再依次分解为参数, 辨别调用的函数后, 将参数传给相应的函数, 同时对标准输入输出流进行操作, 达到模拟命令行的效果。具体的指令功能实现如下: 

#### diff指令

- 获取帮助 diff --help
- 指令语法：diff {-b}{-B}{-i}{-p}{-w}{-I[字符串]}[文件1] [文件2]
- 输出：比较文件1和文件2，输出如何通过最少次增删行将文件1变成文件2
- 特殊比较规则如下：
- -b：不检查空格字符的不同（连续的空格和一个空格视为相同）
- -B：不检查空白行
- -i：不检查大小写的不同
- -q：仅显示有无差异，不显示详细信息。无差异时不显示，有差异时输出“File [] and [] differ".
- -w: 忽略全部空格字符
- -I[字符串] :含有[字符串]的行全部视为相同行
- **输入非法处理**
  * **文件不存在**
  * **文件数不是两个**
  * **参数无法识别**

#### grep指令

* **获取帮助 grep --help**
* **计算符合样式的行数 grep -c \[模式串]\[文件]\* **
* **在显示符合样式的那一行之前，不标示该行所属的文件名称 grep -h \[模式串]\[文件]\* **
* **在显示符合样式的那一行之前，标示该行所属的文件名称 grep -H \[模式串]\[文件]\* **
* **忽略字符大小写区别 grep -i \[模式串]\[文件]\* **
* **在显示符合样式的那一行之前，标示该行的编号 grep -n \[模式串]\[文件]\* **
* **除了显示符合范本样式的那一行之外，同时显示出该行之后的\[行数]行内容 grep -A\[行数] \[模式串]\[文件]\* **
* **除了显示符合范本样式的那一行之外，同时显示出该行之前的\[行数]行内容 grep -B\[行数] \[模式串]\[文件]\* **
* **相互排斥的指令处理：以后出现的为准**
* **输入非法处理**
  * **文件不存在**
  * **指令有误**
  * **参数有误**

#### tee指令

#### cat指令

#### cp指令

#### cd指令

#### pwd指令

#### echo指令

### 额外功能

#### ls指令

#### cls/clear指令

#### change指令(主题切换)

#### vim指令(系统调用)

#### exit指令(退出程序)

## 实现细节

### 组织框架和文件说明





接下来我们分模块介绍每个模块的实现细节, 对于算法的应用, 会适当解析算法的细节, 对于部分重要实现, 我们会详细解说相应的代码。

### part1

##### diff指令

第一部分：准备工作

1. 储存用户输入的特殊比较要求

   ```c++
   bool command[5] = {false};        //创建布尔数组储存用户是否有-b,-B等特殊要求
       struct zifu {                           //储存用户是否有-I的要求
           bool I = false;                             //是否有-I
           char zifu[20] = {};                      //-I是什么
       };
       zifu command_I;     //初始化-I
   
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
           } else if (strcmp(argv[i], "--help") == 0);
           else {
               cerr << "diff: " << argv[i] << ": No such command" << endl;
               return;
           }
       }
   ```

   ##### 

2. 加工文件路径并打开文件，读取文件到file数组里

   ```c++
   if (!regex_match(P0, regex("-"))) {
           ifstream test1(tmp0);
           if (!test1) {
               cerr << "diff: " << argv[argc - 2] << ": No such file or directoty" << endl;
               return;
           }
           int hang = 1;
           while (test1.getline(file1[hang], 500)) {
               hang++;
           }
           maxlinea = hang - 1;
       }
   ```

   ##### 

3. 将文件复制一份到file_c数组里，所有的预处理都针对复制的文件进行

第二部分：预处理

1. 依次根据用户输入的特殊比较要求将复制的文件作出对应的操作，具体为：

- -b：把所有连续的空格字符换成一个字符

  ```c++
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
  ```

  ##### 

- -B：删除所有的空白行，同时用a[],b[]两个数组储存变换前后行的对应关系（即a[i]=j表示处理后文件的第i行对应着处理前文件的第j行）（如果没有执行- B，则a[i]=i）

  ```c++
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
  ```

  ##### 

- -i：利用ASKII码将所有小写字母换成大写字母

- -w：删去所有的空格字符

- -I[字符串]:将所有包含指定字符串的行换为指定字符串

  ```c++
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
  ```

  ##### 

- 最后检查“-q"，若执行了它，遍历比较两个文件，遇到不同直接返回不同，否则返回空字符。

第三部分：寻找最优解

思路：找出两个文件中都包含的，且出现顺序相同的，数量尽可能多的一组行（后文中称其为目标行）。这组行保留，其余的行删减，这样可以实现总输出最短。

实现方法：

1. 首先统计行与行的相同关系，对于文件1中每一行，遍历文件2中每一行，判断是否相同并记录在sameline数组里（相同为1，不同为0）（在这里，展示了手动实现strcmp函数的过程)

   ```c++
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
   ```

   ##### 

2. 创建remember数组，它的(i,j)元表示在行数不小于i且列数不小于j的范围中，能找到的目标行最多有几行。利用迭代算法，从后往前依次推导。

   ```c++
   for (int i = maxlinea; i >= 1; i--) {
           for (int j = maxlineb; j >= 1; j--) {
               if (sameline[i][j] == 0) {
                   if (remember[i + 1][j] >= remember[i][j + 1]) {
                       remember[i][j] = remember[i + 1][j];
                   } else {
                       remember[i][j] = remember[i][j + 1];
                   }
               } else {
                   remember[i][j] = remember[i + 1][j + 1] + 1;
               }
           }
       }
   ```

   ##### 

3. 创建并获得aim数组，它储存了我们要找的目标行，最终输出完全按照它来进行。

   实现方法：我们取出remember的第一个元素，它就是目标行的数量（设为x），从它开始，挑出sameline数组中的“1”。当且仅当取出这个“1”（假设它为i，j元）后，remember[i+1] [j+1]等于x-1时，这个“1”才是我们要找的1，aim中对应位置变成1，否则为0，继续寻找。当x减到0时，就获得了完整的aim。（remember的迭代过程保证了x最终一定会被减到0且这就是最优解。

   ```c++
   int a0 = remember[1][1];
       int nowline = 1;
       int nowrow = 1;
       while (a0 > 0 && nowrow <= maxlineb && nowline <= maxlinea) {
           bool have = false;
           for (int i = nowrow; i <= maxlineb; i++) {
               if (sameline[nowline][i] == 1) {
                   have = true;
                   if (remember[nowline][i] == a0) {
                       aim[nowline][i] = 1;
                       a0--;
                       nowline++;
                       nowrow = i + 1;
                       break;
                   } else {
                       nowline++;
                       break;
                   }
               }
           }
           if (!have) {
               nowline++;
           }
       }
   ```

   ##### 

第四部分：输出结果

按照aim数组输出，aim中的1表示要不变的行，0表示需要删减的行。将删减信息放入strout即可。



### part2

##### grep指令

<center> <font size=4 face = "黑体">简介与实现框架展示</font> </center>
本次大作业TinyShell的Grep指令实现，输出样式、报错样式、匹配模式等均按照标准的Linux样式。对于文件的处理，采用“面向对象程序设计”的思想，构建了File类。每个File对象负责对一个文件（包括从strin读入）进行处理，并记录每个文件的具体内容、其匹配的行数、匹配的区段等信息。最终执行指令的时候，由doGrep函数统一调用File类中存储的已经处理好的信息进行输出。

![image-20211229234550587](https://tva1.sinaimg.cn/large/008i3skNly1gxv7lsaeygj31me0i4mz8.jpg)



<center> <font size="4" face="黑体">代码与关键函数总览</font></center>
* **stringProcess相关函数**

|                    stringProcess相关函数                     |                         函数功能简介                         |
| :----------------------------------------------------------: | :----------------------------------------------------------: |
|      **void splitByStar(string s,vector\<string\>& v)**      |          将s通过*号分割开，分成若干段不含*号的子串           |
|       **int findFirstOf(string P,string T, int idx)**        | 在文本串T中找到模式串P第一次出现的位置，(.可以匹配任意字符)  |
|            **int findLastOf(string P,string T)**             | 在文本串T中找到模式串P最后一次出现的位置（.）可以匹配任意元素 |
| **bool match(string P,string T,int &front, int& back, int i_flag)** | 检测模式串P与字符串T是否匹配，并以引用的形式给出匹配的区间段[front,back] |
|         **string colorLine(string s, int l, int r)**         | 在[l,r]区间内的字符标红处理,如遇到l=r=-1则不用标红直接返回即可 |
|        **string colorFile(string fName, bool match)**        | 输出带颜色的文件名，如果是与子串匹配的要输出filename: 如果不是匹配的输出filename- |
|       **string colorLineNum(int lineNum, bool match)**       | 输出带颜色的行号，如果是匹配的，则输出lineNum： 如果不是匹配的则输出lineNum- |

* **fileProcess相关函数**

|                  fileProcess相关函数                   |                         函数功能简介                         |
| :----------------------------------------------------: | :----------------------------------------------------------: |
|              **File::File(string Name)**               | 构造函数，记录文件名，<font color="red">若输入的文件名是- 则将文件名置为(standard input)，并且从stdin读取。</font> |
|              **bool File::getContents()**              | 获取当前文件的内容,若成功读取则返回true，若该文件不存在则返回false |
| **void File::checkMatch(string patterns, int i_flag)** |    检测模式串与该文件内每一行是否匹配，并记录下匹配的位置    |

* **doGrep相关函数**

|                        doGrep相关函数                        |                         函数功能介绍                         |
| :----------------------------------------------------------: | :----------------------------------------------------------: |
|                       **void init()**                        | 在每次解析命令行之前，先将c_flag,h_flag,i_flag,A_num,B_num初始化 |
| **int commandLineParse(int argc,char * argv[], vector\<File>& file, string& p)** | 对命令行参数进行解析.若为非法输入则返回0;若为help指令,返回2；其余返回1 |
|           **void doGrep(int argc, char * argv[])**           | grep指令函数，将按照传入的参数进行指令处理，并将结果输出到strout |

<center> <font face = "黑体" size=4> 主要实现功能说明</font> </center>
*  **实现带由*和.的模式串与文本串的<font color="red">最长</font>匹配**

  **整体构思**

  对于带有\*号的模式串P，可通过\* 号将其分割为$${P=p_1+'*'+p_2+'*'+…+p_n}$$.设$${T[l_i,r_i]}$$为子串$${p_i}$$在文本串T中分布的位置。则模式串与文本串能够匹配的条件$${\forall i\leq n,p_i}$$为文本串P的子串，且${\forall i <j,r_i <l_j}$.另外，要使匹配能够<font color="red">最长</font>,则需要使得$${p_1}$$的匹配尽量靠前，而${p_n}$的匹配尽量靠后。则匹配最长匹配的区段为${T[l_1,r_ n]}$.

  **具体实现**

  * 通过\* 切割模式串P的函数**void splitByStar(string s,vector\<string\>& v)**

    逐一扫描字符串中的每一个字符，如果遇到\* 号就利用STL中的substr函数将上一个\*号与该\*号之间的部分截取出来，如果遇到多个连续分布的多个\*号，只考虑其中一个即可。并将分割出来的子串存入STL提供的vector中。

    <font color="red"> 注：对于\*号之前或之后没有字符的情况，视分割出来的子串为空串。例如：\*abc分割后为空串以及"abc". </font>

  * 找到子串$${p_i}$$在文本串中首次出现位置的函数**int findFirstOf(string P,string T, int idx)**以及最后一次出现位置的函数**int findLastOf(string P,string T)**

    注：findFirstOf函数与FindLastOf实现思路类似，本处只介绍findFirstOf函数的实现思路。

    findFirstOf函数采用蛮力算法。对于模式串P和文本串T，按自左向右的次序考察各子串。在初始状态下，T的前m个字符将与P的m个字符两两对齐。接下来，自左向右检查相互对其的这m对字符。若当前字符对相互匹配，则转向下一个字符；一旦失配，则说明在此位置文本串与模式串不可能完全匹配，于是可将P向右移动一位，然后从其首字母开始与T中对应的新子串重新对比。若经过检查，当前的m对字符均匹配，则意味着整体匹配成功，从而返回匹配字符串的位置。

    <font color="red">注：由于.可以匹配任意字符，所以对于某对字符，模式串中字符为.或者模式串与字符串中对应的字符相同都视为该对字符匹配.对于空字符串，规定其与任何字符串都匹配。</font>

  * 判断模式串P（可带有\* 和.）是否与文本串匹配的函数**bool match(string P,string T,int &front, int& back, int i_flag)**

    先将模式串P拆解为不带\*号的字符串。然后找出第一个字符串在文本串中位置以及最后一个字符串在文本串中出现的最后一个位置。接下来只要检测除这两个字符串以外的所有字符串是否为文本串中的子串以及是否满足${\forall i <j,r_i <l_j}$即可。若上述检测均成功，则返回true，否则返回false。

    <font color="red">注：i_flag表示的是是否忽略大小写，i_flag为1时忽略大小写，为0时区分大小写。</font>

* **实现指令解析**

  **整体构思**

  Linux中的grep指令一般遵循以下原则：

  * -c的优先级最高。输入grep指令的时候，只要含有-c就只会输出对应的行数，并不会输出文本串具体内容。

    例如。例如，输入grep -c -A3 a a.txt会输出以下结果：

    ![image-20211204192131050](https://tva1.sinaimg.cn/large/008i3skNly1gxv7lyuamuj318i0g4jsb.jpg)

  * 对于互相“排斥”的指令，以最后出现的为准

    例如,grep -A3 -A2 a a.txt则会以后出现的-A2为准:

    ![image-20211204194153347](https://tva1.sinaimg.cn/large/008i3skNly1gxv7m1nb43j318g0im0tr.jpg)

    又如，grep -H -h a a.txt会以后出现的-h为准：

    ![image-20211204195315723](https://tva1.sinaimg.cn/large/008i3skNly1gxv7m5ed5ej311m0isq40.jpg)

  基于以上思路，为了控制各个各个指令的开启关闭等状态，本次实现大作业时，在doGrep.h文件中定义了几个变量。

  ```c++
  static int c_flag; //1代表命令中含有-c,应输出计算符合样式的行数
  static int h_flag; //0和-1代表不显示所属的文件名称（-1为默认值），1代表输出所属的文件名称
  static int n_flag; //0代表不用输出对应的行号，1代表要输出对应的行号
  static int i_flag; //0代表区分大小写，1代表不区分大小写
  static int A_num; //代表输出符合范本样式后的行数
  static int B_num; //代表输出符合范本样式前的行数
  ```

  **int commandLineParse(int argc,char * argv[], vector\<File>& file, string& p)**函数会根据传入的参数将对应的变量设置为对应值，从而在输出的时候可以实现对应的操作。

* **当输入非法时实现报错**

  要处理的输入非法一般分为以下几种情况：

  * 文件不存在

    文件存在性的检测由FIle类完成。ifstream fin打开对应文件失败时，即文件不存在。在此情况下，返回false并输出**No such file or directory**的提示。

  * 指令错误

    若输入的指令不是${\{-h\}\{-H\}\{-n\}\{-c\}\{-A[行数]\}\{-B[行数]\}}$之一，则**commandLineParse**函数会给出invalid option的提示。

    例如，输入grep -g A a.txt：

    ![image-20211204201634045](https://tva1.sinaimg.cn/large/008i3skNly1gxv7m8nc3kj30gk0403yt.jpg)

  * 参数输入错误

    对于${\{-A[行数]\}\{-B[行数]\}}$，若输入的行数非法，则**int commandLineParse(int argc,char * argv[], vector\<File>& file, string& p)**函数会给出invalid context length argument的提示。

    例如：

    ![image-20211204201942982](https://tva1.sinaimg.cn/large/008i3skNly1gxv7mbi0nbj30h80380t2.jpg)

### part3

##### tee指令

##### cat指令

##### cp指令

##### cd指令

##### pwd指令

### part4

##### echo指令



##### 框架实现和复合指令

根据需求文档, 进行以下架构的设计:

1. 框架整体是一个死循环，不断等待通过getline来阻塞监听用户的输入，每读取输入的一行，则处理相应的用户输入，处理为命令和参数的形式。
2. 对于读取到到每一行，先用正则表达式进行指令的分割（面向复合指令），再一次进行指令的选择调用。
3. 每一条指令执行完毕，并且有下一条指令时，将strout拷贝到strin部分，标准输出部分清空。
4. 到一行指令执行完毕，再将strout输出到屏幕，并将strout清空。

实现细节方面, 主要分为指令分割和参数分割: 

指令分割: 通过正则表达式分割后再用迭代器将分割结果交给下游处理

```c++
bool splitInstr(string tmp) {
    bool flag = true;
    regex parttern("\\|");//以'|'作为分割字符
    sregex_token_iterator pos(tmp.begin(), tmp.end(), parttern, -1);
    decltype(pos) end;
    for (; pos != end; ++pos) {
        proceseInstr(pos->str());
        memcpy(gTerm.strin, gTerm.strout, MAXFILE);// 进行指令选择前将输出流拷贝到输入流,同时将输出流清空
        memset(gTerm.strout, 0, MAXFILE);
        bool tmpf = selectInstr(); //选择指令
        flag = flag and tmpf; //如果遇到指令选择错误，则跳出指令的执行
        if (not tmpf)cerr << "command \"" << Argv[0] << "\" not found!" << endl;
    }
    return flag;
}
```

指令参数处理: 通过正则表达式, 利用空格或者空白符进行分割, 分割完成的参数写入全局变量`Argv[]`中, 通过记录参数个数`Argc`,最后进行指令的选择, 通过将`Argc`和`Argv[]`作为参数传入下游函数:

```c++
void proceseInstr(string tmp) {
    Argc = 0;
    regex parttern("\\s+");
    tmp.erase(0, tmp.find_first_not_of(' '));//清除开头空白字符
    sregex_token_iterator pos(tmp.begin(), tmp.end(), parttern, -1);
    decltype(pos) end;
    for (; pos != end; ++pos) { // 写入参数
        memset(Argv[Argc], 0, sizeof(char) * MAXLINE);
        memcpy(Argv[Argc], pos->str().c_str(), strlen(pos->str().c_str()));
        Argc++;
    }
}
```

指令选择部分: 按第一个参数匹配命令, 最后进行相应函数的调用。

### part5

##### ls指令

通过简单的系统调用实现实现当前文件夹中的内容, 如果文件夹不存在, 由系统提示错误。

```c++
void doLs() {
    string root = gTerm.root;
    string work = gTerm.wdir;
    string ans = "ls " + root + work;
    system(ans.c_str());
}
```

##### cls/clear指令

通过系统调用达到清空屏幕的目的。

```c++
void doCls() {
    system("clear");
}
```

##### change指令(主题切换)



##### vim指令(系统调用)

##### exit指令(退出程序)



## 测试部分



### 视频测试内容

由于视频时间所限, 我们挑选了较为复杂的测试样例进行测试, 主要聚焦基本功能的实现和一部分的出错处理:

// todo每个部分想自己的测试样例, 尽量组合成复合指令

```shell
Machine Name:mac
Root Directory: #相应工作目录
Login:usr
ls
pwd
echo hello|grep hel -|cat -n -
echo hello|diff - a.txt|cat -s -
cat a.txt |grep di -|cat -n -s  -E -
cls

```





### 其他测试内容和测试结果

```shell
#diff 部分
diff --help
#输出：
diff--help:
Compare FILES line by line
 - b(ignore - space - change) :ignore changes in the amount of white space
 - B(ignore - blank - lines) :ignore changes where lines are all blank
 - i(ignore - case) :ignore case differences in file contents
 - w(--ignore - all - space) :ignore all white space
 - I(ignore - matching - lines = RE) :ignore changes where all lines match RE
 - q(--brief) :report only when files differ
 
 /Users/iMac-1/Github/tinyshell_combine
//接下来的三组测试数据来展示diff对于特殊比较要求的处理
# a.txt中内容为：
# f i  rs t
# second
#
# thi  r d

# b.txt中内容为：
# fir s   t
#
# seconnnnnd
# THi r D
1.diff -b -i -B a.txt b.txt:
#输出：
1d1
<f i  rs t
<second
4a1
>fir s   t
>seconnnnnd
2.diff -Isecon -w a.txt b.txt:
#输出：
2a2
>
3d4
<
<thi  r d
4a4
>THi r D
3.diff -q -i -Isec -B -w a.txt b.txt:
#输出：(按照特殊的比较要求，两个文件无差异，故无输出)


//本组测试数据来直观展示diff输出行数尽可能少（a第一行与b最后一行相同，但为了行数最短，还是要将其删掉）
# a.txt中内容为：
# 9898
# 2 2  2
# 33
# 
# A
# b.txt中内容为：
# 222
# 
# 3   3
# a
# 9999
diff -I9 -w -B -i a.txt b.txt:
#输出：
1d1
<9898
5a5
>9999


//最后，展示diff比较大文件的能力（几千行的文件可以瞬间输出比较结果）
diff的复杂度足以撑住上千行的文件，由于输入输出过大，不在此详细展示，仅截取部分输出：
diff doDiff.cpp doGrep.cpp:
#部分输出：
<        cout << endl;
<        //system("pause");
<    }
490a212
>                        strcat(gTerm.strout, colorLine(fileVec[i].contents[j], fileVec[i].matchPositions[j].first,
>                                                       fileVec[i].matchPositions[j].second).c_str());
>                        strcat(gTerm.strout, "\n");
491d216
<    if (command[3] == 1) {
<        for (int i = 1; i <= maxlinea - konga; i++) {
```

