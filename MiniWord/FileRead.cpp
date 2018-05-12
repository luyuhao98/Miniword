#include "LinkedLine.h"
#include "FileRead.h"
#include "stdafx.h"
#include <fstream>
#include <iostream>
#include <string.h>


using namespace std;

const int aMAXL = 100; //地址字符串长度
const int MAXL = 500; //单行字符串长度

void FileRead(Article & Ar, char * Address)
{
	ifstream inFile;
	inFile.open(Address);

	char * str = new char[MAXL];//存储每行信息的字符串
	Line curL = Ar.GetLine(0); //当前行
	Line nextL; //下一行

	while (inFile.getline(str, MAXL)) {

		int len = strlen(str);

		for (int i = 0; i < len; i++) {
			curL->lnBuff.Push(str[i], LF);
		}

		nextL = new struct Node;  //申请新的一行空间
		Ar.InsertAfter(curL, nextL); //在当前行后面插入一行
		Ar.IncLineN(); //行数加一
		curL = nextL;

	}
	Ar.Remove(Ar.GetLine(Ar.LineNum() - 1)); //移除文章的最后一行
	cout << Ar.LineNum();
	inFile.close();
}

