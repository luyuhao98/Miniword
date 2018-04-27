#pragma once

#define StackIncreament 100
#define DefaultSize 100

typedef struct Node * ptrToNode;
typedef ptrToNode DLine;				//ÑÆÐÐ
typedef ptrToNode Line;

typedef class DualStack DualStack;

//Çø·Ö×óÓÒÕ»
enum stack { LF, RG };

class DualStack
{
public:

	DualStack(int sz = DefaultSize);
	~DualStack();

	void Push(wchar_t x, enum stack d);
	void Pop(enum stack d);
	wchar_t Top(enum stack d) const;
	wchar_t PopandTop(enum stack d);

	bool IsEmpty() const;
	bool IsEmpty(enum stack d) const;
	bool IsFull() const;

	int GetSize() const;
	int GetSize(enum stack d) const;

	int CharWidth() const;
	int CharWidth(enum stack d) const;

	void MakeEmpty();
	void OverflowProcess();

	void LFtoRG() { Push(PopandTop(LF), RG); }
	void RGtoLF() { Push(PopandTop(RG), LF); }

	wchar_t* ArrayPos(enum stack d)
	{
		if (d == LF)
			return array;
		return &array[top[1]];
	}

private:

	wchar_t* array;
	int top[2];
	int maxSize;
};


struct Node
{
	Line nextL;
	Line prevL;
	DualStack lnBuff;

	void NewLine(Line newL)
	{
		newL->prevL = this;
		newL->nextL = this->nextL;
		this->nextL->prevL = newL;
		this->nextL = newL;
	}

	bool IsFirstL() { return this->prevL->prevL == nullptr; }
	bool IsLastL() { return this->nextL->nextL == nullptr; }
};

class Article
{
public:
	Article()
	{
		Line L = new struct Node;
		firstL = new struct Node;
		lastL = new struct Node;
		firstL->prevL = nullptr;
		lastL->nextL = nullptr;

		firstL->nextL = L;
		lastL->prevL = L;
		L->nextL = lastL;
		L->prevL = firstL;
		lineNum = 1;
	};

	~Article();
	bool IsEmpty() const { return firstL->nextL == nullptr; }
	bool IsFirstL(Line L) const { return L->prevL == firstL; }
	bool IsLastL(Line L) const { return L->nextL == lastL; }

	Line GetLine(int lineNum) const;

	void InsertAfter(Line L, Line newL);
	void InsertBefore(Line L, Line newL);
	void Remove(Line L);

	int LineNum(void) const { return lineNum; }
	void IncLineN(void) { lineNum++; }
private:
	DLine firstL;
	DLine lastL;

	int lineNum;
};