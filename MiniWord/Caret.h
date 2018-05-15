#pragma once
#include "subeditor.h"

class Caret
{
public:
	int CaretPosX, CaretPosY;

	Caret(int x = 0, int y = 0);
	~Caret() {};

	void MvRight		(line L);
	void MvLeft		(line L);
	void MvUp		(line L);
	void MvDown		(line L);
	void MvHome		(line L);
	void MvEnd		(line L);
	void MvPrior	(line L);
	void MvNext		(line L);

	wchar_t CtrDelete	(line L);
	void CtrEnter	(line L);
	void CtrCaretMv	(line L, int X, int Y);
	void CtrDelLine	(line L);
};