#include "subeditor.h"
#include "stdafx.h"
#include <fstream>
#include <iostream>
#include <string.h>
using namespace std;

const int MAXL = 500; //单行字符串长度

void openNewFile(Article & Ar, wchar_t * Address) //打开新文件
{
	wifstream inFile;
	inFile.open(Address); //打开文件并写入数据结构
	if (!inFile.is_open()) return;

	wchar_t * str = new wchar_t[MAXL];//存储每行信息的字符串
	line curL = Ar.GetLine(0); //当前行
	line nextL; //下一行

	while (inFile.getline(str, MAXL)) {

		size_t len = wcslen(str);
		curL->Insert((const wchar_t * &)str);//将数据插入到当前行
		nextL = curL->NewLine();//在当前行后面插入一行
		Ar.IncLineN(); //行数加一;

	}
	line lastL = Ar.GetLine(Ar.LineNum() - 1);
	Ar.Remove(lastL); //移除文章的最后一行
	cout << Ar.LineNum();
	inFile.close();
}

void saveFile(Article & Ar, wchar_t * Address) { //保存文件

	wofstream outFile(Address);
	if (!outFile) return;

	line curL = Ar.GetLine(0);
	wchar_t * str;

	while (!curL->IsLastL()) {
		str = curL->curContent();
		outFile << str;
		curL = curL->next;
	}
	InvalidateRect(hWnd, NULL, TRUE);
}

void createNewFile(Article & Ar, wchar_t * address) { //新建文件

	wofstream outfile(address);
	if (!outfile) return;

	Ar.clearWord();
	InvalidateRect(hWnd, NULL, TRUE);
}