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

bool Article::IsEmpty() const {
	if (firstL->next->next == lastL) {
		line curL = firstL->next;
		if (curL->Getlen() == 0) return true;
	}
	return false;
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
/*给line返回行号，注意，如果没找到这一行，则返回-1 */
int Article::GetNum(line l) const 
{
	int i = 0;
	line lstart = this->firstL->next;
	while (l != lstart && !IsEnd(lstart))
	{
		i++;
		lstart = lstart->next;
	}
	if (IsEnd(lstart)) return -1;
	return i;
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
	lineNum--;
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

	line curL = firstL -> next;
	curL->MakeEmpty();
	curL = curL-> next;
	line nextL;

	while (!IsLastL(curL)) {
		nextL = curL->next;
		Remove(curL);
		curL = nextL;
	}
	lineNum = 1;
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

	memset(arr, 0, sizeof(wchar_t)*(sz + 1));
	
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
	memset(newarr, 0, sizeof(wchar_t)*(size + 1));

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
int Line::GetGend()//取gend;
{
	return gend;
}

/*返回该行字符串*/
wchar_t * Line::GetPos()
{
	
	return arr;
}

/*LF, 光标前一字符位置 ，RG ， 光标后一个字符位置*/
wchar_t * Line::GetPos(int i)
{
	if (i == LF) {
		return arr;
	}
	else return arr + gend;
}

/*返回字符串;*/
wchar_t * Line::GetStr()
{
	wchar_t * l = new wchar_t[len + 1];
	memset(l,0,sizeof(wchar_t)*(len+1));
	wcsncpy(l, GetPos(LF), Getlen(LF));
	wcsncpy(l + Getlen(LF), GetPos(RG), Getlen(RG));
	return l;
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



/*插入字符串，返回插入字符串后当前行*/
line Line::Insert(wchar_t * &cc)
{
	int num = 0;
	return Insert(cc, num);
}

/*插入字符串*/
/* TMD,windows环境下换行处 是一个\r回车符 和一个\n换行符构成 :\r\n */

line Line::Insert(wchar_t * &cc, int &num)
{
	line tmpl = this;
	size_t cclen = wcslen(cc);
	
	int counter = 0;
	int flag = 0;

	for (int i = 0; i < cclen; i++) {
		if (cc[i] == L'\r') {
			flag = 1;
			break;
		}	
	}
	
	if (!flag) {
		while (cclen > Gapgsize()) OverflowProcess();
		cclen = wcslen(cc);
		wcsncpy(arr + gstart, cc, cclen);
		gstart += cclen;
		len += cclen;
	}
	else {
		int storelen = Getlen(RG);
		wchar_t * store = new wchar_t[storelen+ 1];
		memset(store, 0, sizeof(wchar_t)*(storelen + 1));
		sizeof(store);/////////////
		wcsncpy(store, this->GetPos(RG), storelen);
		
		MakeEmpty(RG);

		for (int i = 0; i < cclen; i++) {
			
			if (cc[i] != L'\r' && i != cclen - 1) {
				continue;
			}
			int l = i - counter;
			while (l > tmpl->Gapgsize()) tmpl->OverflowProcess();
			l = i - counter;
			
			wcsncpy(tmpl->arr + tmpl->gstart, cc+counter, l);
			
			tmpl->gstart += l;
			tmpl->len += l;


			if (cc[i] == L'\r')
			{
				i++;//跳到'\n'处,然后for循环的++跳到下一字符
				counter = i + 1;
				tmpl = tmpl->NewLine();
				num++;
			}
			else {
				while (storelen > tmpl->Gapgsize()) tmpl->OverflowProcess();

				tmpl->gend = tmpl->size - storelen;
				wcsncpy(tmpl->arr+tmpl->gend, store, storelen);
				tmpl->len += storelen;
				
			}
		}

	}
	return tmpl;
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
void Line::Rwrite(wchar_t * &cc)
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
	len = 0;
	mark = -1;
	gstart = 0;
	gend = size;
	memset(arr, 0, sizeof(wchar_t)*(size + 1));
}
void Line::MakeEmpty(int i)
{
	if (i == -1) {
		memset(arr, 0, sizeof(wchar_t)*gstart);
		len -=gstart;
		gstart = 0;
	}
	else {

		memset(arr+gend, 0, sizeof(wchar_t)*(len - gstart));
		len = gstart;
		gend = size;
	}
}