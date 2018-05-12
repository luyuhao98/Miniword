#include "LinkedLine.h"
#include "FileRead.h"
#include "stdafx.h"
#include <fstream>
#include <iostream>
#include <string.h>


using namespace std;

const int aMAXL = 100; //��ַ�ַ�������
const int MAXL = 500; //�����ַ�������

void FileRead(Article & Ar, char * Address)
{
	ifstream inFile;
	inFile.open(Address);

	char * str = new char[MAXL];//�洢ÿ����Ϣ���ַ���
	Line curL = Ar.GetLine(0); //��ǰ��
	Line nextL; //��һ��

	while (inFile.getline(str, MAXL)) {

		int len = strlen(str);

		for (int i = 0; i < len; i++) {
			curL->lnBuff.Push(str[i], LF);
		}

		nextL = new struct Node;  //�����µ�һ�пռ�
		Ar.InsertAfter(curL, nextL); //�ڵ�ǰ�к������һ��
		Ar.IncLineN(); //������һ
		curL = nextL;

	}
	Ar.Remove(Ar.GetLine(Ar.LineNum() - 1)); //�Ƴ����µ����һ��
	cout << Ar.LineNum();
	inFile.close();
}

