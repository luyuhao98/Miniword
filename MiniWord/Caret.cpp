#include "stdafx.h"

Caret::Caret(int x, int y)
{
	CaretPosX = x;
	CaretPosY = y;
}

void Caret::MvLeft(Line * L)
{
	if ( !L->IsEmpty(LF) ) {
		if (0x4E00 <= L->Top(LF) && L->Top(LF) <= 0x9FBB) {
			CaretPosX -= 2;
		}
		else
			CaretPosX--;
		L->PointMove(-1);
	}
	else {
		if (!L->IsFirstL()) {
			L->Gapmove();
			CaretPosX = L->pre->CharWidth();
			CaretPosY -= 1;
		}
	}
}

void Caret::MvRight(Line * L)
{
	if (!L->IsEmpty(RG)) {
		if (0x4E00 <= L->Top(RG) && L->Top(RG) <= 0x9FBB) {
			CaretPosX += 2;
		}
		else
			CaretPosX++;
		L->PointMove(1);
	}
	else {
		if (!L->IsLastL()) {
			CaretPosX = 0;
			CaretPosY += 1;
			L->next->PointMoveto(0);
			//while (!L->next->IsEmpty(LF)) {
			//	L->next->LFtoRG();
			//}
		}
	}
}

void Caret::MvUp(Line *L)
{
	if (!L->IsFirstL()) {
		L->Gapmove();
		CaretPosY--;
		if (CaretPosX > L->pre->CharWidth()) {
			CaretPosX = L->pre->CharWidth();
		}
		else {
			while (L->pre->CharWidth(LF) > CaretPosX)
				L->pre->PointMove(-1);
			CaretPosX = L->pre->CharWidth(LF);
		}
	}
	else {
		L->PointMoveto(0);
		CaretPosX = 0;
	}
}

void Caret::MvDown(Line * L)
{
	if (!L->IsLastL()) {
		L->Gapmove();
		CaretPosY++;

		while (L->next->CharWidth(LF) > CaretPosX)
			L->next->PointMove(-1);
		CaretPosX = L->next->CharWidth(LF);
	}
	else {
		L->Gapmove();
		CaretPosX = L->CharWidth();
	}
}

void Caret::MvHome(Line  *L)
{
	L->Gapmove();
	while (!L->IsFirstL()) {
		L = L->pre;
	}
	CaretPosX = 0;
	CaretPosY = 0;
	L->PointMoveto(0);
}

void Caret::MvEnd(Line * L)
{
	L->Gapmove();
	while (!L->IsLastL()) {
		L = L->next;
		CaretPosY++;
	}
	CaretPosX = L->CharWidth();
	L->Gapmove();
}

wchar_t Caret::CtrDelete(Line *L)
{
	if (!L->IsEmpty(RG)) {
		wchar_t x = L->Pop(RG);
		return x;
	}
	return 0;
}

void Caret::CtrEnter(Line * L)
{


	Line * newL = L->NewLine();

	while (!L->IsEmpty(RG)) {
		newL->Push(L->Pop(RG),LF);
	}
	newL->PointMoveto(0);
	CaretPosX = 0;
	CaretPosY++;
}