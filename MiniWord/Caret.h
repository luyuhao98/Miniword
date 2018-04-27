#pragma once
#include "LinkedLine.h"

class Caret
{
public:
	int CaretPosX, CaretPosY;

	Caret(int x = 0, int y = 0);
	~Caret() {};

	void MvRight	(Line L);
	void MvLeft		(Line L);
	void MvUp		(Line L);
	void MvDown		(Line L);
	void MvHome		(Line L);
	void MvEnd		(Line L);
	void MvPrior	(Line L);
	void MvNext		(Line L);

	wchar_t CtrDelete	(Line L);
	void CtrEnter	(Line L);
	void CtrCaretMv	(Line L, int X, int Y);
	void CtrDelLine	(Line L);
};