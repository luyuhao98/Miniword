#pragma once

const int GapIncrement = 50;// gapArticle每次增加大小
const int DefaultSize = 100;//默认
enum stack { LF=-1 , RG=1 };

class position {
	class Line* y;	//所在Line
	int x;	//在有效字符的位置
};	//一个光标点point, 一个记录点mark
/*
发现：
~x的范围：
	0 到 size
	光标的左面为第x个字符arr[x-1];即 x左边为arr[x-1],x右边为arr[x]
	0:行最左（左面没有字符）
	size:行最右（左面是最后字符,此时数组arr刚好满）
~鼠标触发性：
	单击鼠标与文本处按下（按下按下按下），即产生一个mark；
	抬起鼠标即更新point，若mark==point，delete mark
~上下键光标位置：
	鼠标点击某行 或 刚刚编辑某行
	记录此时光标到行头距离D=gstart。
	此后上下键，满足：
	若无新行(newLine),光标不动。
	若D >= newLine->len,则光标移动到新行的最后一位，point.x = newLine->len;
	若D < newLine->len,则光标移动到新行的D位置;

	注：newLine 新行，Line* 型，是指相对本行已存在的上一行或下一行,不是新创建的一行

~若自己实现复制粘贴，则需要新的Article
*/

class Line
{
private:
	int len;//有效字符数量
	int size;//Article的总大小(字符数量)
	int mark;//Mark不在这行为-1，若在这行，mark为其实际位置
	int gstart;//gapstart,gap开始位置，光标位置（光标若在）
	int gend;//gapend,gap结束位置

	wchar_t * arr;//数组

public:
	Line * pre;//上一个Line
	Line * next;//下一个Line


	Line(int sz = DefaultSize);//创建一个空Line，包括创建指针，申请空间size
	~Line();//析构一个Line,并连接上下指针
	Line* NewLine();//在本Line后面新建一个Line

	int IsFull();//判断Line是否为满
	int RleaseProcess();//释放gapbuffer为0;
	void MakeEmpty();//清空内容
	void OverflowProcess(); //用于满了后申请数组


	int PointMove(int p); //为正，光标往行尾移动p位。为负，光标往行首移动p位。
	void PointMoveto(int d);//将光标移动到第d个字符
	//PointMove( -1 )		int LeftMovePoint();//不改变原句，光标左移
	//PointMove( 1 )		int RightMovePoint();//不改变原句，光标右移
	int Gapmove();//改变point后移动gap

		int UsertoGap(int);//传入面向user字符的位置，返回在arr中的真实位置
	int Getsize();//取size (总大小)值
	
	int GetPoint();//取gstart（目前光标位置）
	int Gapgsize();//取  gap的宽度;

	wchar_t * GetPos(int i);//LF, 段首第一个字 ，RG ， 光标后一个字符位置

	int Getlen();//取len 有效字符长度（用户眼中字符长度）
	int Getlen(int i) const;//取len 有效字符长度（用户眼中字符长度）


	int CharWidth();//字符长度
	int CharWidth(int i) const;//i=1 右侧，i=-1 左侧
	
	int IsEmpty(int i) const;	//判断Line是否为空 ，i可为LF，RG
	int IsEmpty();			//判断Line是否为空


	wchar_t Top(int i);//LG得到左侧元素 RG得到右侧元素
	void Push(const wchar_t c,int i);//插入一个字符 LF,插左，RG插右
	void Insert(const wchar_t * &cc);//插入字符串
	wchar_t Pop(int p);//删除一个字符，p=1删除光标后面的相当于del , p=-1删除光标前面的相当于backspace。
	
	void Delete();//删除标记mark到光标gstart.mark标记在第mark字符的右侧，mark+1的左侧。
	void Rwrite(const wchar_t &c);//替换输入下一字符
	void Rwrite(const wchar_t * &cc);//替换输入一串字符
	
	bool IsFirstL() { return this->pre->pre == nullptr; }
	bool IsLastL() { return this->next->next == nullptr; }

	//	int Savespace();//不再操作本Line（newLine或者point移出本行）时执行，若Line为空，释放数组。若gap>400，整理gap至400。
	//int Readfile();//从这行读文件 * ///
	//int Writefile(); //用这行写入文件* ///
};


class Article {
private:
	Line * firstL;
	Line * lastL;
	int lineNum;
public:
	Line * L;//目前正操作的Line头

	position point;
	position mark;
	Article();//构造 Line *head的next为空
	~Article();

	bool IsEmpty() const { return firstL->next == nullptr; }
	bool IsFirstL(Line * L) const { return L->pre == firstL; }
	bool IsLastL(Line * L) const { return L->next == lastL; }

	Line * GetLine(int lineNum) const;

	void InsertAfter(Line *L);
	void Remove(Line*&L);

	int LineNum(void) const { return lineNum; }
	void IncLineN(void) { lineNum++; }

};
