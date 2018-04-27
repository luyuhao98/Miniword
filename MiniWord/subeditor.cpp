#include "stdafx.h"

/*创建一个空piece，包括创建指针，申请空间size*/

piece::piece(int sz)
{
	if (sz < 0)
		return;
	size = sz;
	len = 0;
	IsPoint = 1;//新建一段时光标就在这一行行首
	mark = -1;
	gstart = 0;//gstart 默认指向将要输入字符的位置,即gap的第一个位置
	gend = size;//gend 默认指向右侧buffer第一个字符的位置，数组下标从0到size-1, gend=size表示右侧没有字符。
	arr = NULL;
	arr = new wchar_t[size];
	if (arr == NULL) exit(0);//申请空间失败

}
/*析构一个piece,并连接上下指针*/
piece::~piece()
{
	delete[] arr;
	arr = NULL;
}

/*判断piece是否为满*/

int piece::IsFull()
{
	return gstart == gend;
}

/*判断piece是否为空*/

int piece :: IsEmpty()
{
	return gstart == 0 && gend == size;
}


///*在本piece后面新建一个piece，用在检测回车上。ps:若newpiece时执行savespace函数*/
//
//piece * piece::Newpiece()
//{
//	return;
//}
//
///*不再操作本piece（newpiece或者point移出本行）时执行，若piece为空，释放数组。若gap>400，整理gap至400。*/
//
//int piece::Savespace()
//{
//	return;
//}
//

/*释放gapbuffer为0;*/

int piece::RleaseProcess()
{
	delete[]arr;
	arr = NULL;
	return 1;
}

/*用于满后申请数组*/

void piece::OverflowProcess()
{
	int newSize = size + GapIncrement;

	wchar_t * newarr = NULL;
	newarr = new wchar_t[newSize];
	if (newarr = NULL) exit(0);

	/*
	std::copy: 高效copy函数。
	Syntax: std::copy(InIt first, InIt last, OutIt x);
	其中：	fist [IN]: 要拷贝元素的首地址
		last [IN]:要拷贝元素的最后一个元素的下一个地址
		[OUT] : 拷贝的目的地的首地址
	集成memcpy,memmove。
	所有数组复制操作请均使用这种方式
	*/
	std::copy(arr, arr + gstart, newarr);//拷贝前半段
	std::copy(arr + gend, arr + size, newarr + gend + GapIncrement);//拷贝后半段
	delete[] arr;
	arr = newarr;
	gend += GapIncrement;
}


/*不改变原句，光标左移*/

int piece::LeftMovePoint() {
	/*左移到头，后序可能增加回到上一行行尾功能*/
	if (gstart == 0) return 0;

	arr[gend] = arr[--gstart];
	return 1;//行内左移成功
}


/*不改变原句，光标右移*/

int piece::RightMovePoint()
{
	/*右移到头，后序可能增加进入下一行行头功能*/
	if (gstart == len) return 0;

	arr[gstart++] = arr[gend++];
	return 1;//行内右移成功
}


/*改变point后移动gap*/

int piece::Gapmove()
{
	if (!IsPoint) {
		/*将后半的数据拷贝与前半合并*/
		std::copy(arr + gend, arr + gend + len - gstart, arr + gstart);
		gend = size;
		//piece::Savespace();
	}
	return 1;//成功移动
}


/*设置len*/

void piece::Setlen()
{
	len=size+gstart-gend;
}
/*取len 有效字符长度（用户眼中字符长度）*/

int piece::Getlen()
{
	return len;
}


/*取size(总大小)值*/
int piece::Getsize()
{
	return size;
}

/*传入面向user字符(有效字符)的位置(index)，返回在arr中的真实位置*/
int piece::UsertoGap(int x) {
	if (x < 0 || x >= size) return -1;//error,x不合法
	if (x < gstart) return x;
	else return x + piece::Gapgsize();
}

/*取gs（目前光标位置）*/

int piece::Getgstart()
{
	return gstart;
}

/*取gap的宽度*/

int piece::Gapgsize()
{
	return gend - gstart;
}

/*取有效字符串*/
wchar_t * piece::Getstr()
{
	wchar_t* newarr = new wchar_t[len + 1];
	std::copy(arr, arr + len, newarr);
	return newarr;
}
/*
class piece
{
private:
int len;//有效字符数量
int size;//buffer的总大小(字符数量)
int IsPoint;//point是否在这行
int mark;//Mark不在这行为-1，若在这行，mark为其实际位置
int gstart;//gapstart,gap开始位置，光标位置（光标若在）
int gend;//gapend,gap结束位置

wchar_t * arr;//数组

piece * pre;//上一个piece
piece * nexe;//下一个piece

//	int Pointpos;//point所在的位置

public:
int Readfile();//从这行读文件 *
int Writefile(); //用这行写入文件*


int Insert(const char &c, int p);//插入一个字符
int Insert(const char * &cc, int p, int len);//插入字符串
int Delete(const char &c, int d);//删除一个字符
int Delete(const char * &cc, int p, int len);//删除一串字符,在userbuffer的第p个位置删除len长度元素
int Rwrite(const char &c, int d);//替换一个字符
int Rwrite(const char * &cc, int p, int len);//替换一串字符,在userbuffer的第p个位置删除len长度元素
int Search(const char * &c, position &m, position &p);//找到所求字符串c的位置，并且起始为mark，结束为point
int Replace(const char * &cc, position &m, position &p);//将mark到point处的字符串

};
*/