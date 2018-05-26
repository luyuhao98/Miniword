#include "stdafx.h"
#define MAXL 500

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
/*��line�����кţ�ע�⣬���û�ҵ���һ�У��򷵻�-1 */
int Article::GetNum(line& l) const
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

void Article::InsertAfter(line& L)
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

int Article::MaxWidth(HDC hdc) const
{
	int Max = -1;
	int a;
	for (line L = this->firstL->next; L != this->lastL; L = L->next) {
		a = L->CharWidth(hdc);
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

	while (!IsEnd(curL)) {
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
	gstart = 0;//gstart Ĭ��ָ��Ҫ�����ַ���λ��,��gap�ĵ�һ��λ��
	gend = size;//gend Ĭ��ָ���Ҳ�buffer��һ���ַ���λ�ã������±��0��size-1, gend=size��ʾ�Ҳ�û���ַ���
	arr = NULL;

	arr = new wchar_t[sz+1];
	memset(arr, 0, sizeof(wchar_t)*(sz + 1));

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
	memset(newarr, 0, sizeof(wchar_t)*(size + 1));

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
int Line::GetGend()//ȡgend;
{
	return gend;
}

/*���ظ����ַ���*/
wchar_t * Line::GetPos()
{
	return arr;
}

/*LF, ���ǰһ�ַ�λ�� ��RG �� ����һ���ַ�λ��*/
wchar_t * Line::GetPos(int i)
{
	if (i == LF) {
		return arr;
	}
	else return arr + gend;
}

/*�����ַ���;*/
wchar_t * Line::GetStr()
{
	wchar_t * l = new wchar_t[len + 1];
	memset(l,0,sizeof(wchar_t)*(len+1));
	wcsncpy(l, GetPos(LF), Getlen(LF));
	wcsncpy(l + Getlen(LF), GetPos(RG), Getlen(RG));
	return l;
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


/*�����ַ��������ز����ַ�����ǰ��*/
line Line::Insert(wchar_t * &cc)
{
	int num = 0;
	return Insert(cc, num);
}

/*�����ַ���*/
/* TMD,windows�����»��д� ��һ��\r�س��� ��һ��\n���з����� :\r\n */

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

			wcsncpy(tmpl->arr + tmpl->gstart, cc + counter, l);

			tmpl->gstart += l;
			tmpl->len += l;


			if (cc[i] == L'\r')
			{
				i++;//����'\n'��,Ȼ��forѭ����++������һ�ַ�
				counter = i + 1;
				tmpl = tmpl->NewLine();
				num++;
			}
		}
		
		while (storelen > tmpl->Gapgsize()) tmpl->OverflowProcess();

		tmpl->gend = tmpl->size - storelen;
		wcsncpy(tmpl->arr + tmpl->gend, store, storelen);
		tmpl->len += storelen;

		

	}
	return tmpl;
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


/*�滻������һ�ַ�*/
void Line::Rwrite(const wchar_t &c)
{
	arr[gstart++] = c;
	gend++;
}




int Line::CharWidth(HDC hdc)
{
	/*	int width = 0;
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
	*/
	return CharWidth(LF, hdc) + CharWidth(RG, hdc);
}


int Line::CharWidth(int d, HDC hdc) const
{
	int width = 0, nCharWidth = 0;
	if (d == LF) {
		for (int i = gstart - 1; i >= 0; i--) {
			GetCharWidth32W(hdc, (UINT)arr[i], (UINT)arr[i],
				&nCharWidth);
			width += nCharWidth;
			/*			if (0x4E00 <= arr[i] && arr[i] <= 0x9FBB) {
			width += 2;
			}
			else width += 1;
			*/
		}
	}
	else {
		for (int i = gend; i < size; i++) {
			GetCharWidth32W(hdc, (UINT)arr[i], (UINT)arr[i],
				&nCharWidth);
			width += nCharWidth;
			/*			if (0x4E00 <= arr[i] && arr[i] <= 0x9FBB) {
			width += 2;
			}
			else width += 1;
			*/
		}
	}
	return width;
}


void Line::MakeEmpty()
{
	len = 0;
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

/*ɾ�� �� py�е�px���ַ��Ҳ��� �� my�е�mx���ַ��Ҳ��� ֮��������ַ�*/
void Article::Delete(int py, int px, int my, int mx)
{

	if (py == my) {
		if (px == mx) return;
		/*ͬ�в���*/
		else {
			if (px > mx)
			{
				int t = px;
				px = mx;
				mx = t;
			}
			/* px < mx */
			line tmpl = GetLine(py);
			tmpl->PointMoveto(px);
			tmpl->gend += mx - px;
		}
	}
	if (py > my) {
		int t = py;
		py = my;
		my = t;
		t = px;
		px = mx;
		mx = t;
	}

	line lp = GetLine(py);
	line lm = GetLine(my);
	lp->PointMoveto(px);
	lm->PointMoveto(mx);
	
	
	lp->MakeEmpty(RG);
	lp->len = lp->gstart;

	int lenm = lm->Getlen(RG);
	while (lenm < lp->Gapgsize()) lp->OverflowProcess();
	wcsncpy(lp->GetPos(RG), lm->GetPos(RG),lenm);
	lp->len += lenm;

	while (lp->next != lm)
		delete lp->next;
	delete lm;
}

/*���� �� py�е�px���ַ��Ҳ��� �� my�е�mx���ַ��Ҳ��� ֮��������ַ�*/
wchar_t* Article::GetStr(int py, int px, int my, int mx) {
	if (py == my) {
		if (px == mx) return NULL;
		/*ͬ�в���*/
		else {
			/*ǰ��p(���)����λ��gstart��px��*/
			line l = GetLine(py);
			if (px > mx) {
				wchar_t * tmp = new wchar_t[px - mx];
				wcsncpy(tmp, l->arr + mx, px - mx);
				return tmp;
			}
			else {
				wchar_t * tmp = new wchar_t[mx - px];
				wcsncpy(tmp, l->arr + l->gend, mx - px);
				return tmp;

			}
		}
	}

	/*���Ϊm,p���λ�þ�Ϊmx��px��*/

	if (my > py)
	{
		int t = my;
		my = py;
		py = t;
		t = mx;
		mx = px;
		px = t;
	}



	line m = GetLine(my);
	line p = GetLine(py);
	if (my < py) {
		/*�õ�����*/
		int sum = 0;
		sum += m->Getlen(RG) + 2;
		line t = m->next;
		while (t != p)
		{
			sum += t->Getlen() + 2;
			t = t->next;
		}
		sum += p->Getlen(LF);

		/*���ƽ����ַ���*/

		wchar_t * tmp = new wchar_t[sum + 1];
		sum = 0;

		wcsncpy(tmp + sum, m->GetPos(RG), m->Getlen(RG));
		sum += m->Getlen(RG);
		wcsncpy(tmp + sum, L"\r\n", 2);
		sum += 2;

		t = m->next;
		while (t != p)
		{
			wcsncpy(tmp + sum, t->GetPos(), t->Getlen());
			sum += t->Getlen();
			wcsncpy(tmp + sum, L"\r\n", 2);
			sum += 2;
		}
		wcsncpy(tmp + sum, p->GetPos(LF), p->Getlen(LF));
		return tmp;
	}
}


int * Article::getNextVal(const wchar_t *s)
{
	size_t len = wcslen(s);
	int * next = new int[len];
	int i = 0;
	int j = -1;
	next[0] = -1;
	while (i<len - 1)//ע�������KMP��������Ĳ�ͬ
	{
		if (j == -1 || s[i] == s[j])
		{
			++i;
			++j;
			next[i] = j;
		}
		else
		{
			j = next[j];
		}
	}
	return next;
}

int Article::KMP(const wchar_t *s, const wchar_t *t)
{
	size_t slen, tlen;
	int i=0 , j=0;
	int * next = getNextVal(t);
	slen = wcslen(s);
	tlen = wcslen(t);

	while (i<slen)
		while ( j<tlen && i<slen)
		{
			if (j == -1 || s[i] == t[j]){
				++i; ++j;
			}
			else  j = next[j];
			if (j == tlen) {
				return i - tlen + 1;
			}
		}

	delete[] next;
	return -1;
}

int Article::onSearch(line & tmpL, const wchar_t * t) //tmpL�ǵ�ǰ��������У�t�Ǵ�ƥ����ִ�
{
	
	/*�ȶԵ�ǰ�й�����Ĳ��ֽ��в���*/
	wchar_t * s = new wchar_t[tmpL->size + 1];
	wcsncpy(s , tmpL->GetPos(RG) ,  tmpL->Getlen(RG));

	int res = KMP(s, t);
	if (res != -1) {
		tmpL->PointMoveto(res);
		return res;
	}
	else {
		/*�����ǰ��֮��Ĳ���δ�ҵ�����Ժ�����н�������*/
		if (!IsEnd(tmpL)) tmpL = tmpL->next;

		while (!IsEnd(tmpL))
		{  
			wchar_t * s = tmpL->GetStr();
			int res = KMP(s, t);
			if (res != -1) {
				tmpL->PointMoveto(res);
				return res;
			}
			delete[]  s;			   
			tmpL = tmpL->next;
		}
	}
	delete[] t;
	return -1;
}