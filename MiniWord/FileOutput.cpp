#include "stdafx.h"
#include "subeditor.h"
#include <iostream>
#include <fstream>
#include <string.h>

using namespace std;

void FileOutput(Article & Ar, wchar_t * Address) {

	wofstream outFile(Address);
	if (!outFile) return;

	line curL = Ar.GetLine(0);
	wchar_t * str;

	while (!curL->IsLastL()) {
		str = curL->curContent();
		outFile << str;
		curL = curL->next;
	}
}