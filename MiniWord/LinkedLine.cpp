#include "stdafx.h"

using namespace std;

Article::~Article()
{
	Line p = firstL;
	while (firstL != nullptr) {
		p = firstL;
		firstL = firstL->nextL;
		delete(p);
	}
}

Line Article::GetLine(int lineNum) const
{
	Line p = this->firstL->nextL;
	int j = 0;
	while (p != lastL && j != lineNum) {
		p = p->nextL;
		j++;
	}
	if (p == lastL)
		return nullptr;
	return p;
}

void Article::InsertAfter(Line L, Line newL)
{
	newL->prevL = L;
	newL->nextL = L->nextL;
	L->nextL->prevL = newL;
	L->nextL = newL;
}

void Article::InsertBefore(Line L, Line newL)
{
	newL->nextL = L;
	newL->prevL = L->prevL;
	L->prevL->nextL = newL;
	L->prevL = newL;
}

void Article::Remove(Line L)
{
	L->prevL->nextL = L->nextL;
	L->nextL->prevL = L->prevL;
	delete(L);
	this->lineNum--;
}

DualStack::DualStack(int sz)
{
	if (sz >= 0) {
		maxSize = sz;
		top[0] = -1;
		top[1] = maxSize;
		array = new wchar_t[maxSize];
	}
}

DualStack::~DualStack()
{
	delete[] array;
	array = NULL;
}

void DualStack::MakeEmpty()
{
	delete[] array;
	top[0] = -1;
	top[1] = maxSize;
	array = new wchar_t[maxSize];
}

void DualStack::Push(wchar_t x, enum stack d)
{
	if (IsFull() == true) {
		OverflowProcess();
	}

	if (d == LF) {
		top[0]++;
	}
	else {
		top[1]--;
	}
	array[top[d]] = x;
}

void DualStack::Pop(enum stack d)
{
	if (IsEmpty() == true) {
		return;
	}
	if (d == LF) {
		top[0]--;
	}
	else {
		top[1]++;
	}
}

wchar_t DualStack::Top(enum stack d) const
{
	if(!IsEmpty(d))
		return array[top[d]];
	return L'\0';
}

wchar_t DualStack::PopandTop(enum stack d)
{
	if (IsEmpty(d)) {
		return 0;
	}
	wchar_t x = array[top[d]];
	if (d == LF) {
		top[0]--;
	}
	else {
		top[1]++;
	}
	return x;
}

bool DualStack::IsEmpty() const
{
	return ((top[0] == -1) && (top[1] == maxSize)) ? true : false;
}

bool DualStack::IsEmpty(enum stack d) const
{
	if (d == LF)
		return top[0] == -1;
	return top[1] == maxSize;
}

bool DualStack::IsFull() const
{
	return (top[0] + 1 == top[1]) ? true : false;
}

int DualStack::GetSize() const
{
	return (top[0] + 1) + (maxSize - top[1]);
}

int DualStack::GetSize(enum stack d) const
{
	if (d == LF)
		return top[0] + 1;
	else
		return maxSize - top[1];
}

int DualStack::CharWidth() const
{
	int width = 0;
	for (int i = top[0]; i >= 0; i--) {
		if (0x4E00 <= array[i] && array[i] <= 0x9FBB) {
			width += 2;
		}
		else width += 1;
	}
	for (int i = top[1]; i < maxSize; i++) {
		if (0x4E00 <= array[i] && array[i] <= 0x9FBB) {
			width += 2;
		}
		else width += 1;
	}
	return width;
}

int DualStack::CharWidth(enum stack d) const
{
	int width = 0;
	if (d == LF) {
		for (int i = top[0]; i >= 0; i--) {
			if (0x4E00 <= array[i] && array[i] <= 0x9FBB) {
				width += 2;
			}
			else width += 1;
		}
	}
	else {
		for (int i = top[1]; i < maxSize; i++) {
			if (0x4E00 <= array[i] && array[i] <= 0x9FBB) {
				width += 2;
			}
			else width += 1;
		}
	}
	return width;
}

void DualStack::OverflowProcess()
{
	int newSize = maxSize + StackIncreament;

	wchar_t* newArray = new wchar_t[newSize];
	for (int i = 0; i <= top[0]; i++) {
		newArray[i] = array[i];
	}
	for (int i = maxSize - 1; i >= top[1]; i--) {
		newArray[i + StackIncreament] = array[i];
	}
	delete[] array;
	array = newArray;
	maxSize = newSize;
	top[1] += StackIncreament;
}
