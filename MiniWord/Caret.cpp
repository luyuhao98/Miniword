#include "stdafx.h"

Caret::Caret(int x, int y)
{
	CaretPosX = x;
	CaretPosY = y;
}

void Caret::MvLeft(Line L)
{
	if ( !L->lnBuff.IsEmpty(LF) ) {
		if (0x4E00 <= L->lnBuff.Top(LF) && L->lnBuff.Top(LF) <= 0x9FBB) {
			CaretPosX -= 2;
		}
		else
			CaretPosX--;
		L->lnBuff.LFtoRG();
	}
	else {
		if (!L->IsFirstL()) {
			while (!L->lnBuff.IsEmpty(RG)) {
				L->lnBuff.RGtoLF();
			}
			CaretPosX = L->prevL->lnBuff.CharWidth();
			CaretPosY -= 1;
		}
	}
}

void Caret::MvRight(Line L)
{
	if (!L->lnBuff.IsEmpty(RG)) {
		if (0x4E00 <= L->lnBuff.Top(RG) && L->lnBuff.Top(RG) <= 0x9FBB) {
			CaretPosX += 2;
		}
		else
			CaretPosX++;
		L->lnBuff.RGtoLF();
	}
	else {
		if (!L->IsLastL()) {
			CaretPosX = 0;
			CaretPosY += 1;
			while (!L->nextL->lnBuff.IsEmpty(LF)) {
				L->nextL->lnBuff.LFtoRG();
			}
		}
	}
}

void Caret::MvUp(Line L)
{
	if (!L->IsFirstL()) {
		while (!L->lnBuff.IsEmpty(RG))
			L->lnBuff.RGtoLF();
		CaretPosY--;

		if (CaretPosX > L->prevL->lnBuff.CharWidth()) {
			CaretPosX = L->prevL->lnBuff.CharWidth();
		}
		else {
			while (L->prevL->lnBuff.CharWidth(LF) > CaretPosX)
				L->prevL->lnBuff.LFtoRG();
			CaretPosX = L->prevL->lnBuff.CharWidth(LF);
		}
	}
	else {
		while (!L->lnBuff.IsEmpty(LF))
			L->lnBuff.LFtoRG();
		CaretPosX = 0;
	}
}

void Caret::MvDown(Line L)
{
	if (!L->IsLastL()) {
		while (!L->lnBuff.IsEmpty(RG))
			L->lnBuff.RGtoLF();
		CaretPosY++;

		while (L->nextL->lnBuff.CharWidth(LF) > CaretPosX)
			L->nextL->lnBuff.LFtoRG();
		CaretPosX = L->nextL->lnBuff.CharWidth(LF);
	}
	else {
		while (!L->lnBuff.IsEmpty(RG))
			L->lnBuff.RGtoLF();
		CaretPosX = L->lnBuff.CharWidth();
	}
}

void Caret::MvHome(Line L)
{
	while (!L->lnBuff.IsEmpty(RG)) {
		L->lnBuff.RGtoLF();
	}
	while (!L->IsFirstL()) {
		L = L->prevL;
	}
	CaretPosX = 0;
	CaretPosY = 0;
	while (!L->lnBuff.IsEmpty(LF)) {
		L->lnBuff.LFtoRG();
	}
}

void Caret::MvEnd(Line L)
{
	while (!L->lnBuff.IsEmpty(RG)) {
		L->lnBuff.RGtoLF();
	}
	while (!L->IsLastL()) {
		L = L->nextL;
		CaretPosY++;
	}
	CaretPosX = L->lnBuff.CharWidth();
	while (!L->lnBuff.IsEmpty(RG)) {
		L->lnBuff.RGtoLF();
	}
}

wchar_t Caret::CtrDelete(Line L)
{
	if (!L->lnBuff.IsEmpty(RG)) {
		wchar_t x = L->lnBuff.PopandTop(RG);
		return x;
	}
	return 0;
}

void Caret::CtrEnter(Line L)
{
	Line newL = new struct Node;

	L->NewLine(newL);

	while (!L->lnBuff.IsEmpty(RG)) {
		newL->lnBuff.Push(L->lnBuff.PopandTop(RG), LF);
	}

	while (!newL->lnBuff.IsEmpty(LF)) {
		newL->lnBuff.LFtoRG();
	}

	CaretPosX = 0;
	CaretPosY++;
}