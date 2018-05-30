#pragma once
#include "subeditor.h"

class Caret
{
public:
	int CaretPosX, CaretPosY;

	Caret(int x = 0, int y = 0) : CaretPosX(x), CaretPosY(y) {}
	~Caret() = default;

	void MvRight(line&, HDC&);
	void MvLeft(line&, HDC&);
	void MvUp(line&, HDC&);
	void MvDown(line&, HDC&);
	void MvHome(line&, HDC&);
	void MvEnd(line&, HDC&);

	void CtrDelete(line&, HDC&);
	void CtrEnter(line&, HDC&);
	void CtrCaretMv(Article&, int, int, HDC&);
};