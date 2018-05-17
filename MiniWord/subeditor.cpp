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
	gstart = 0;//gstart Ĭ��ָ��Ҫ�����ַ���λ��,��gap�ĵ�һ��λ��
	gend = size;//gend Ĭ��ָ���Ҳ�buffer��һ���ַ���λ�ã������±��0��size-1, gend=size��ʾ�Ҳ�û���ַ���
	arr = NULL;

	arr = new wchar_t[sz+1];
	arr[sz] = L'\0';//β�Ͷ���һλ������L'\0'��β������ʹ��wcsncpy
	
	if (arr == NULL) exit(0);//����ռ�ʧ��

}

/*����һ��Line,����������ָ��*/
Line::~Line()
{
	pre->next = next;
	next->pre = pre;
	delete[]arr;
}


/*�ж�Line�Ƿ�Ϊ��*/
int Line::IsFull()
{
	return gstart == gend;
}

/*�ж�Line�Ƿ�Ϊ��*/
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

/*�ڱ�Line�����½�һ��Line�����ڼ��س��ϡ�ps:��newLineʱִ��savespace����*/
line  Line::NewLine()
{
	line  l = new Line;
	l->next = this->next;
	l->pre = this;
	this->next->pre = l;
	this->next = l;

	return l;
}

/*�ͷ�gapbufferΪ0;*/
int Line::RleaseProcess()
{
	delete[]arr;
	arr = NULL;
	return 1;
}

/*����������������*/
void Line::OverflowProcess()
{
	size += GapIncrement;

	wchar_t * newarr = NULL;
	
	newarr = new wchar_t[size + 1];
	newarr[size] = L'\0';//���ֶ�һλL'\0'

	if (newarr == NULL) exit(0);

	wcsncpy(newarr, arr, gstart);
	wcsncpy(newarr + gend + GapIncrement, arr + gend, len-gend);

	delete[] arr;
	arr = newarr;
	gend += GapIncrement;

}

/*Ϊ�����������β�ƶ�pλ��Ϊ��������������ƶ�pλ��
PointMove(1) �� LeftMovePoint �ȼ�
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

/*������ƶ�����d���ַ�*/
void Line::PointMoveto(int d)
{
	d = d - gstart;
	PointMove(d);
}

/*�ı�point���ƶ�gap*/
int Line::Gapmove()
{
	/*���������ݿ�����ǰ��ϲ�*/
	wcsncpy(arr + gstart, arr + gend, size - gend);
	gend = size;
	gstart = len;
	//Line::Savespace();

	return 1;//�ɹ��ƶ�
}

/*ȡlen ��Ч�ַ����ȣ��û������ַ����ȣ�*/
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



/*ȡsize(�ܴ�С)ֵ*/
int Line::Getsize()
{
	return size;
}

/*��������user�ַ�(��Ч�ַ�)��λ��(index)��������arr�е���ʵλ��*/
int Line::UsertoGap(int x)
{
	if (x < 0 || x >= size) return -1;//error,x���Ϸ�
	if (x < gstart) return x;
	else return x + Line::Gapgsize();
}

/*ȡgs��Ŀǰ���λ�ã�*/
int Line::GetPoint()
{
	return gstart;
}

/*ȡgap�Ŀ��*/
int Line::Gapgsize()
{
	return gend - gstart;
}

/*LF, ���ǰһ�ַ�λ�� ��RG �� ����һ���ַ�λ��*/
wchar_t * Line::GetPos(int i)
{
	if (i == LF) {
		return arr;
	}
	else return arr + gend;
}

/* ����һ���ַ�*/
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

/*�����ַ���*/
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

/* �õ�������Ԫ��*/
wchar_t Line::Top(int i)
{
	if (i == LF && !IsEmpty(LF))
		return arr[gstart - 1];
	else if (i == RG && !IsEmpty(RG))
		return arr[gend];
	return L'\0';
}


/*ɾ��һ���ַ�*/
wchar_t Line::Pop(int p)
{
	if (p == -1) {
		if (gstart == 0)
		{
			/*�ϲ���������һ��*/
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
			/*�ϲ���������һ��*/
		}
		else {
			len--;
			gend++;
			return arr[gend - 1];
		}
	}
	return NULL;
}

/*ɾ�����mark�����gstart.mark����ڵ�mark�ַ����Ҳ࣬mark+1����ࡣ*/
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

/*�滻������һ�ַ�*/
void Line::Rwrite(const wchar_t &c)
{
	arr[gstart++] = c;
	gend++;
}

/*�滻����һ���ַ�*/
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