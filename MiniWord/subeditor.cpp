#include "stdafx.h"

Article::Article()
{
	L = new Line;
	firstL = new Line;
	lastL = new Line;

	firstL->pre = nullptr;
	lastL->next = nullptr;

	firstL->next = L;
	lastL->pre = L;
	L->next = lastL;
	L->pre = firstL;
	lineNum = 1;
}

Article::~Article()
{
	line  p = firstL->next;
	while (p != lastL) {
		Remove(p);
		p = firstL->next;
	}
}

line Article::GetLine(int lineNum) const
{

	line p = firstL->next;
	int j = 0;
	while (p != lastL && j != lineNum) {
		p = p->next;
		j++;
	}
	if (p == lastL)
		return nullptr;
	return p;
}

void Article::InsertAfter(line L)
{
	line newL = new Line;
	newL->pre = L;
	newL->next = L->next;
	L->next->pre = newL;
	L->next = newL;
}

void Article::Remove(line & L)
{
	L->pre->next = L->next;
	L->next->pre = L->pre;
	delete(L);
	this->lineNum--;
}

int Article::MaxWidth(void) const
{
	int Max = -1;
	int a;
	for (line L = this->firstL->next; L != this->lastL; L = L->next) {
		a = L->CharWidth();
		if (a > Max)
			Max = a;
	}
	return Max;
}

void Article::clearWord() {

	line curL = GetLine(0);
	line nextL;

	while (!IsLastL(curL)) {
		nextL = curL->next;
		Remove(curL);
		curL = nextL;
	}

}

Line::Line(int sz)
{
	if (sz < 0)
		return;
	size = sz;
	len = 0;
	mark = -1;
	gstart = 0;//gstart 默认指向将要输入字符的位置,即gap的第一个位置
	gend = size;//gend 默认指向右侧buffer第一个字符的位置，数组下标从0到size-1, gend=size表示右侧没有字符。
	arr = NULL;

	arr = new wchar_t[sz+1];
	arr[sz] = L'\0';//尾巴多留一位，保持L'\0'结尾，可以使用wcsncpy
	
	if (arr == NULL) exit(0);//申请空间失败

}

/*析构一个Line,并连接上下指针*/
Line::~Line()
{
	pre->next = next;
	next->pre = pre;
	delete[]arr;
}


/*判断Line是否为满*/
int Line::IsFull()
{
	return gstart == gend;
}

/*判断Line是否为空*/
int Line::IsEmpty()
{
	return gstart == 0 && gend == size;
}

int Line::IsEmpty(int i) const
{
	if (i == LF)
		return gstart == 0;
	return gend == size;
}

/*在本Line后面新建一个Line，用在检测回车上。ps:若newLine时执行savespace函数*/
line  Line::NewLine()
{
	line  l = new Line;
	l->next = this->next;
	l->pre = this;
	this->next->pre = l;
	this->next = l;

	return l;
}

/*释放gapbuffer为0;*/
int Line::RleaseProcess()
{
	delete[]arr;
	arr = NULL;
	return 1;
}

/*用于满后申请数组*/
void Line::OverflowProcess()
{
	size += GapIncrement;

	wchar_t * newarr = NULL;
	
	newarr = new wchar_t[size + 1];
	newarr[size] = L'\0';//保持多一位L'\0'

	if (newarr == NULL) exit(0);

	wcsncpy(newarr, arr, gstart);
	wcsncpy(newarr + gend + GapIncrement, arr + gend, len-gend);

	delete[] arr;
	arr = newarr;
	gend += GapIncrement;

}

/*为正，光标往行尾移动p位。为负，光标往行首移动p位。
PointMove(1) 跟 LeftMovePoint 等价
*/
int Line::PointMove(int p)
{
	if (p > 0) {
		if (p == 1 && gstart == len)
		{
			//move point to next line begin
			return 1;
		}
		if (p > len - gstart)
			return 0;
		else {
			wcsncpy(arr + gstart, arr + gend, p);
			gstart += p;
			gend += p;
		}
	}
	else {
		p = -p;
		if (p == 1 && gstart == 0)
		{
			//move point to previous line end
			return 1;
		}
		if (p > gstart)
			return 0;
		else {
			wcsncpy(arr + gend - p, arr + gstart - p, p);
			gstart -= p;
			gend -= p;
		}
	}
	return 1;
}

/*将光标移动到第d个字符*/
void Line::PointMoveto(int d)
{
	d = d - gstart;
	PointMove(d);
}

/*改变point后移动gap*/
int Line::Gapmove()
{
	/*将后半的数据拷贝与前半合并*/
	wcsncpy(arr + gstart, arr + gend, size - gend);
	gend = size;
	gstart = len;
	//Line::Savespace();

	return 1;//成功移动
}

/*取len 有效字符长度（用户眼中字符长度）*/
int Line::Getlen()
{
	return len;
}

int Line::Getlen(int i) const
{
	if (i == LF)
		return gstart;
	if (i == RG)
		return len - gstart;
}



/*取size(总大小)值*/
int Line::Getsize()
{
	return size;
}

/*传入面向user字符(有效字符)的位置(index)，返回在arr中的真实位置*/
int Line::UsertoGap(int x)
{
	if (x < 0 || x >= size) return -1;//error,x不合法
	if (x < gstart) return x;
	else return x + Line::Gapgsize();
}

/*取gs（目前光标位置）*/
int Line::GetPoint()
{
	return gstart;
}

/*取gap的宽度*/
int Line::Gapgsize()
{
	return gend - gstart;
}

/*LF, 光标前一字符位置 ，RG ， 光标后一个字符位置*/
wchar_t * Line::GetPos(int i)
{
	if (i == LF) {
		return arr;
	}
	else return arr + gend;
}

/* 插入一个字符*/
void Line::Push(const wchar_t c, int i)
{
	if (IsFull() == 1) {
		OverflowProcess();
	}

	if (i == LF) {
		arr[gstart++] = c;
	}
	else {
		arr[--gend] = c;
	}
	len++;
}

/*插入字符串*/
void Line::Insert(const wchar_t * &cc)
{
	size_t cclen = wcslen(cc);
	while (cclen > Gapgsize()) {
		OverflowProcess();
		cclen -= Gapgsize();
	}
	cclen = wcslen(cc);
	
	wcsncpy(arr + gstart, cc, cclen);
	PointMove(cclen);
	len += cclen;
}

/* 得到光标左侧元素*/
wchar_t Line::Top(int i)
{
	if (i == LF && !IsEmpty(LF))
		return arr[gstart - 1];
	else if (i == RG && !IsEmpty(RG))
		return arr[gend];
	return L'\0';
}


/*删除一个字符*/
wchar_t Line::Pop(int p)
{
	if (p == -1) {
		if (gstart == 0)
		{
			/*合并本段与上一段*/
			return NULL;
		}
		else {
			len--;
			gstart--;
			return arr[gstart];
		}
	}
	if (p == 1) {
		if (gstart == len)
		{
			return NULL;
			/*合并本段与下一段*/
		}
		else {
			len--;
			gend++;
			return arr[gend - 1];
		}
	}
	return NULL;
}

/*删除标记mark到光标gstart.mark标记在第mark字符的右侧，mark+1的左侧。*/
void Line::Delete()
{
	if (gstart <= mark) {
		len -= mark - gstart;
		gend += mark - gstart;
	}
	else {
		len -= gstart - mark;
		gstart = mark;
	}
}

/*替换输入下一字符*/
void Line::Rwrite(const wchar_t &c)
{
	arr[gstart++] = c;
	gend++;
}

/*替换输入一串字符*/
void Line::Rwrite(const wchar_t * &cc)
{
	int cclen = wcslen(cc);
	if (cclen >= len - gstart) {
		gend = size;
	}
	else {
		gend += cclen;
	}
	Insert(cc);
}

std::wstring Line::curContent() {

	std::wstring str;
	
	for (int i = 0; i < gstart; i++) {
		str += arr[i];
	}
	for (int i = gend; i < size; i++) {
		str += arr[i];
	}
	return str;
}

int Line::CharWidth()
{
	int width = 0;
	for (int i = gstart - 1; i >= 0; i--) {
		if (0x4E00 <= arr[i] && arr[i] <= 0x9FBB) {
			width += 2;
		}
		else width += 1;
	}
	for (int i = gend; i < size; i++) {
		if (0x4E00 <= arr[i] && arr[i] <= 0x9FBB) {
			width += 2;
		}
		else width += 1;
	}
	return width;
}

int Line:: CharWidth(int d) const
{

	int width = 0;

	if (d == LF) {
		for (int i = gstart - 1; i >= 0; i--) {
			if (0x4E00 <= arr[i] && arr[i] <= 0x9FBB) {
				width += 2;
			}
			else width += 1;
		}
	}
	else {
		for (int i = gend; i < size; i++) {
			if (0x4E00 <= arr[i] && arr[i] <= 0x9FBB) {
				width += 2;
			}
			else width += 1;
		}
	}
	return width;
}

void Line::MakeEmpty()
{
	delete[] arr;
	gstart = 0;
	gend = size;
	arr = new wchar_t[size];
}