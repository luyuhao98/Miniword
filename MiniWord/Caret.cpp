#include "stdafx.h"

void Caret::MvLeft(line& L, HDC& hdc)
{
	if (!L->IsEmpty(LF)) {
		int nCharWidth;
		GetCharWidth32W(hdc, (UINT)L->Top(LF), (UINT)L->Top(LF),
			&nCharWidth);
		/*		if (0x4E00 <= L->Top(LF) && L->Top(LF) <= 0x9FBB) {
					CaretPosX -= 2;
				}
				else
					CaretPosX--;
		*/
		CaretPosX -= nCharWidth;
		L->PointMove(-1);
	}
	else {
		if (!L->IsFirstL()) {
			L->Gapmove();
			CaretPosX = L->pre->CharWidth(hdc);
			CaretPosY -= 1;
		}
	}
}

void Caret::MvRight(line& L, HDC& hdc)
{
	if (!L->IsEmpty(RG)) {
		int nCharWidth;
		GetCharWidth32W(hdc, (UINT)L->Top(RG), (UINT)L->Top(RG),
			&nCharWidth);

		CaretPosX += nCharWidth;
		L->PointMove(1);
	}
	else {
		if (!L->IsLastL()) {
			CaretPosX = 0;
			CaretPosY += 1;
			L->next->PointMoveto(0);
		}
	}
}

void Caret::MvUp(line& L, HDC& hdc)
{
	if (!L->IsFirstL()) {
		L->Gapmove();
		CaretPosY--;
		if (CaretPosX > L->pre->CharWidth(hdc)) {
			CaretPosX = L->pre->CharWidth(hdc);
		}
		else {
			while (L->pre->CharWidth(LF, hdc) > CaretPosX)
				L->pre->PointMove(-1);
			CaretPosX = L->pre->CharWidth(LF, hdc);
		}
	}
	else {
		L->PointMoveto(0);
		CaretPosX = 0;
	}
}

void Caret::MvDown(line& L, HDC& hdc)
{
	if (!L->IsLastL()) {
		L->Gapmove();
		CaretPosY++;

		while (L->next->CharWidth(LF, hdc) > CaretPosX)
			L->next->PointMove(-1);
		CaretPosX = L->next->CharWidth(LF, hdc);
	}
	else {
		L->Gapmove();
		CaretPosX = L->CharWidth(hdc);
	}
}

void Caret::MvHome(line& L, HDC& hdc)
{
	L->Gapmove();
	while (!L->IsFirstL()) {
		L = L->pre;
	}
	CaretPosX = 0;
	CaretPosY = 0;
	L->PointMoveto(0);
}

void Caret::MvEnd(line& L, HDC& hdc)
{
	L->Gapmove();
	while (!L->IsLastL()) {
		L = L->next;
		CaretPosY++;
	}
	CaretPosX = L->CharWidth(hdc);
	L->Gapmove();
}

void Caret::CtrDelete(line& L, HDC& hdc)
{
	if (!L->IsEmpty(RG)) {
		L->Pop(RG);
	}
}

void Caret::CtrEnter(line& L, HDC& hdc)
{
	line newL = L->NewLine();

	while (!L->IsEmpty(RG)) {
		newL->Push(L->Pop(RG),LF);
	}
	newL->PointMoveto(0);
	CaretPosX = 0;
	CaretPosY++;
}

void Caret::CtrCaretMv(Article& Ar, int x, int y, HDC& hdc)
{
	line L = Ar.GetLine(CaretPosY);
	L->Gapmove();
	CaretPosY = y;
	L = Ar.GetLine(CaretPosY);
	int width = 0;
	int i = 0, nCharWidth = 0;

	for (i = 0; i != L->gstart && width <= x; ++i) {
		GetCharWidth32W(hdc, (UINT)L->arr[i], (UINT)L->arr[i], &nCharWidth);
		width += nCharWidth;
	}
	CaretPosX = width;
	L->PointMoveto(i);

}