#include "doTee.h"

bool debug = true;
void doTee(int argc, char* argv[]) {
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
	char file1[100][100] = { 0 };
	int hang = 1;
	int y = 0;
	int maxline = 0;
	while (true) {
		int z = 0;
		while (gTerm.strin[y] != '\n') {
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
	maxline = hang - 1;

	if (debug) {
		for (int j = 1; j <= maxline; j++) {
			cout << file1[j] << endl;
		}
	}

	char r1[100] = { 0 }, w1[100] = { 0 };
	strcpy(w1, gTerm.wdir);
	for (; i < argc; i++) {
		if (argv[i][0] == '/') {//根目录
			strcpy(r1, gTerm.root);
			strcat(r1, argv[i]);
			//strcat(r1, ".txt");
		}
		else {//相对路径
			strcpy(r1, w1);
			//strcat(r1, "/");
			strcat(r1, argv[i]);
			//strcat(r1, ".txt");
		}

		if (debug) {
			cout <<"路径：" <<endl<<r1 << endl;
		}
		if (command) {
			ifstream ceshi(r1);
			if (!ceshi) {
				cerr << "doTee:" << argv[i] << "不存在的文件" << endl;
				ceshi.close();
			}
			else {
				ceshi.close();
				ofstream wenjian(r1, ios::app);
				for (int j = 1; j <= maxline; j++) {
					wenjian << file1[j]<<endl;
				}
				wenjian.close();
			}
		}
		else {
			ofstream wenjian(r1);
			for (int j = 1; j <= maxline; j++) {
				wenjian << file1[j]<<endl;
			}
			wenjian.close();
		}
	}
	return;
}