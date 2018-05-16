#include "stdafx.h"
#include "subeditor.h"
#include <iostream>
#include <fstream>
#include <string.h>

using namespace std;

void FileOutput(Article & Ar, char * Address) {

	ofstream outFile(Address);
	if (!outFile) return;

	line curL = Ar.GetLine(0);
	char * str;

	while (!curL->IsLastL()) {
		str = curL->curContent();
		outFile << str;
		curL = curL->next;
	}
}