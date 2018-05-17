#include "subeditor.h"
#include "stdafx.h"
#include <fstream>
#include <iostream>
#include <string.h>
using namespace std;

const int MAXL = 500; //�����ַ�������

void openNewFile(Article & Ar, wchar_t * Address) //�����ļ�
{
	wifstream inFile;
	inFile.open(Address); //���ļ���д�����ݽṹ
	if (!inFile.is_open()) return;

	wchar_t * str = new wchar_t[MAXL];//�洢ÿ����Ϣ���ַ���
	line curL = Ar.GetLine(0); //��ǰ��
	line nextL; //��һ��

	while (inFile.getline(str, MAXL)) {

		size_t len = wcslen(str);
		curL->Insert((const wchar_t * &)str);//�����ݲ��뵽��ǰ��
		nextL = curL->NewLine();//�ڵ�ǰ�к������һ��
		Ar.IncLineN(); //������һ;

	}
	line lastL = Ar.GetLine(Ar.LineNum() - 1);
	Ar.Remove(lastL); //�Ƴ����µ����һ��
	cout << Ar.LineNum();
	inFile.close();
}

void saveFile(Article & Ar, wchar_t * Address) { //�����ļ�

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

void createNewFile(Article & Ar, wchar_t * address) { //�½��ļ�

	wofstream outfile(address);
	if (!outfile) return;

	Ar.clearWord();
	InvalidateRect(hWnd, NULL, TRUE);
}