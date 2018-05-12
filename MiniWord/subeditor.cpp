#include "stdafx.h"

/*����һ����piece����������ָ�룬����ռ�size*/

piece::piece(int sz)
{
	if (sz < 0)
		return;
	size = sz;
	len = 0;
	IsPoint = 1;//�½�һ��ʱ��������һ������
	mark = -1;
	gstart = 0;//gstart Ĭ��ָ��Ҫ�����ַ���λ��,��gap�ĵ�һ��λ��
	gend = size;//gend Ĭ��ָ���Ҳ�buffer��һ���ַ���λ�ã������±��0��size-1, gend=size��ʾ�Ҳ�û���ַ���
	arr = NULL;
	arr = new wchar_t[size];
	if (arr == NULL) exit(0);//����ռ�ʧ��

}
/*����һ��piece,����������ָ��*/
piece::~piece()
{
	pre->next = next;
	next->pre = pre;
	delete this;
}

/*�ж�piece�Ƿ�Ϊ��*/

int piece::IsFull()
{
	return gstart == gend;
}

/*�ж�piece�Ƿ�Ϊ��*/

int piece :: IsEmpty()
{
	return gstart == 0 && gend == size;
}


///*�ڱ�piece�����½�һ��piece�����ڼ��س��ϡ�ps:��newpieceʱִ��savespace����*/
//
//piece * piece::Newpiece()
//{
//	return;
//}
//
///*���ٲ�����piece��newpiece����point�Ƴ����У�ʱִ�У���pieceΪ�գ��ͷ����顣��gap>400������gap��400��*/
//
//int piece::Savespace()
//{
//	return;
//}
//

/*�ͷ�gapbufferΪ0;*/

int piece::RleaseProcess()
{
	delete[]arr;
	arr = NULL;
	return 1;
}

/*����������������*/

void piece::OverflowProcess()
{
	int newSize = size + GapIncrement;

	wchar_t * newarr = NULL;
	newarr = new wchar_t[newSize];
	if (newarr = NULL) exit(0);

	/*
	std::copy: ��Чcopy������
	Syntax: std::copy(InIt first, InIt last, OutIt x);
	���У�	fist [IN]: Ҫ����Ԫ�ص��׵�ַ
		last [IN]:Ҫ����Ԫ�ص����һ��Ԫ�ص���һ����ַ
		last = first + num  ����numΪԪ�ظ���
		[OUT] : ������Ŀ�ĵص��׵�ַ
	����memcpy,memmove��
	�������鸴�Ʋ������ʹ�����ַ�ʽ
	*/
	std::copy(arr, arr + gstart, newarr);//����ǰ���
	std::copy(arr + gend, arr + size, newarr + gend + GapIncrement);//��������
	delete[] arr;
	arr = newarr;
	gend += GapIncrement;
}

/*Ϊ�����������β�ƶ�pλ��Ϊ��������������ƶ�pλ��
	PointMove(1) �� LeftMovePoint �ȼ�
*/

int piece::PointMove(int p) 
{
	if (!IsPoint) return 0;
	if (p > 0) {
		if (p == 1 && gstart == len)
		{
			//move point to next line begin
			return 1;
		}
		if (p > len - gstart)
			return 0;
		else {
			std::copy(arr + gend, arr + gend + p, arr + gstart);
			gstart += p;
		}
	}
	else {
		p = - p;
		if (p == 1 && gstart == 0)
		{
			//move point to previous line end
			return 1;
		}
		if (p > gstart)
			return 0;
		else {
			std::copy(arr + gstart - p, arr + gstart, arr + gend - p);
			gstart -= p;
		}
	}
	return 1;
}

/*������ƶ�����d���ַ�*/

int piece ::PointMoveto(int d)
{
	d = d - gstart;
	PointMove(d);
}
//
///*���ı�ԭ�䣬�������*/
//
//int piece::LeftMovePoint() {
//	/*���Ƶ�ͷ������������ӻص���һ����β����*/
//	if (gstart == 0) return 0;
//
//	arr[gend] = arr[--gstart];
//	return 1;//�������Ƴɹ�
//}
//
//
///*���ı�ԭ�䣬�������*/
//
//int piece::RightMovePoint()
//{
//	/*���Ƶ�ͷ������������ӽ�����һ����ͷ����*/
//	if (gstart == len) return 0;
//
//	arr[gstart++] = arr[gend++];
//	return 1;//�������Ƴɹ�
//}
//

/*�ı�point���ƶ�gap*/

int piece::Gapmove()
{
	if (!IsPoint) {
		/*���������ݿ�����ǰ��ϲ�*/
		std::copy(arr + gend, arr + size, arr + gstart);
		gend = size;
		gstart = len;
		//piece::Savespace();
	}
	return 1;//�ɹ��ƶ�
}




/*ȡlen ��Ч�ַ����ȣ��û������ַ����ȣ�*/

int piece::Getlen()
{
	return len;
}


/*ȡsize(�ܴ�С)ֵ*/
int piece::Getsize()
{
	return size;
}

/*��������user�ַ�(��Ч�ַ�)��λ��(index)��������arr�е���ʵλ��*/
int piece::UsertoGap(int x) {
	if (x < 0 || x >= size) return -1;//error,x���Ϸ�
	if (x < gstart) return x;
	else return x + piece::Gapgsize();
}

/*ȡgs��Ŀǰ���λ�ã�*/

int piece::Getgstart()
{
	return gstart;
}

/*ȡgap�Ŀ��*/

int piece::Gapgsize()
{
	return gend - gstart;
}

/*ȡ��Ч�ַ���*/
wchar_t * piece::Getstr()
{
	Gapmove();
	wchar_t* newarr = new wchar_t[len + 1];
	std::copy(arr, arr + len, newarr);
	return newarr;
}

/* point�����һ���ַ�*/

void piece::Push(const wchar_t &c)
{
	if (IsFull() == true) {
		OverflowProcess();
	}
	arr[gstart++] = c;
	len++;
}

/*�����ַ���*/
void piece::Insert(const wchar_t * &cc)
{
	int cclen = wcslen(cc);
	while (cclen > Gapgsize()) {
		OverflowProcess();
		cclen -= Gapgsize();
	}
	cclen = wcslen(cc);
	std::copy(cc, cc + cclen, arr + gstart);
	PointMove(cclen);
	len += cclen;
}

/*ɾ��һ���ַ�*/

void piece::Pop(int p)
{
	if (p == -1) {
		if (gstart == 0)
		{
			/*�ϲ���������һ��*/
		}
		else {
			len--;
			gstart--;
		}
	}
	if (p == 1) {
		if (gstart == len)
		{
			/*�ϲ���������һ��*/
		}
		else {
			len--;
			gend++;
		}
	}

}

/*ɾ�����mark�����gstart.mark����ڵ�mark�ַ����Ҳ࣬mark+1����ࡣ*/

void piece :: Delete() 
{
	if (gstart <= mark) {
		len -= mark - gstart;
		gend += mark - gstart;
	}
	else{
		len -= gstart - mark;
		gstart = mark;
	}
}


/*�滻������һ�ַ�*/
void piece::Rwrite(const wchar_t &c)
{
	arr[gstart++] = c;
	gend++;
}
/*�滻����һ���ַ�*/
void piece :: Rwrite(const wchar_t * &cc)
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
int Search(const wchar_t * &c, position &m, position &p);//�ҵ������ַ���c��λ�ã�������ʼΪmark������Ϊpoint
int Replace(const wchar_t * &cc, position &m, position &p);//��mark��point�����ַ���

/*
class piece
{
private:
int len;//��Ч�ַ�����
int size;//buffer���ܴ�С(�ַ�����)
int IsPoint;//point�Ƿ�������
int mark;//Mark��������Ϊ-1���������У�markΪ��ʵ��λ��
int gstart;//gapstart,gap��ʼλ�ã����λ�ã�������ڣ�
int gend;//gapend,gap����λ��

wchar_t * arr;//����

piece * pre;//��һ��piece
piece * nexe;//��һ��piece

//	int Pointpos;//point���ڵ�λ��

public:
int Readfile();//�����ж��ļ� *
int Writefile(); //������д���ļ�*


};
*/