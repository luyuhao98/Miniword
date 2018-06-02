#pragma once

const int GapIncrement = 50;// gapArticle每次增加大小
const int DefaultSize = 100;//默认
typedef class Line * line;
enum stack { LF=-1 , RG=1 };

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

	注：newLine 新行，line 型，是指相对本行已存在的上一行或下一行,不是新创建的一行

~若自己实现复制粘贴，则需要新的Article
*/

typedef class Line
{
public:
	int len;//有效字符数量
	int size;//Line的总大小(字符数量)
	int gstart;//gapstart,gap开始位置，光标位置（光标若在）
	int gend;//gapend,gap结束位置
	wchar_t* arr;//数组

	line pre;//上一个Line
	line next;//下一个Line

	Line(int sz = DefaultSize);//创建一个空Line，包括创建指针，申请空间size
	~Line();//析构一个Line,并连接上下指针
	line NewLine();//在本Line后面新建一个Line

	int IsFull();//判断Line是否为满
	int RleaseProcess();//释放gapbuffer为0;
	void OverflowProcess(); //用于满了后申请数组

	int PointMove(int p); //为正，光标往行尾移动p位。为负，光标往行首移动p位。
	void PointMoveto(int d);//将光标移动到第d个字符
	//PointMove( -1 )		int LeftMovePoint();//不改变原句，光标左移
	//PointMove( 1 )		int RightMovePoint();//不改变原句，光标右移
	int Gapmove();//改变point后移动gap

	int UsertoGap(int);//传入面向user字符的位置，返回在arr中的真实位置

	int Gapgsize();//取gap的宽度;
	int GetPoint();//取gstart（目前光标位置）
	int GetGend();//取gend;
	int Getsize();//取size (总大小)值
	int Getlen();//取len 有效字符长度（用户眼中字符长度）
	int Getlen(int i) const;//取len 有效字符长度（用户眼中字符长度）

	wchar_t * GetPos();//返回该行字符串指针
	wchar_t * GetPos(int i);//返回左右字符串 LF, return arr ，RG ， return arr+gend
	wchar_t * GetStr();//返回字符串;

	int CharWidth(HDC hdc);//字符长度
	int CharWidth(int i, HDC hdc) const;//i=1 右侧，i=-1 左侧

	int IsEmpty(int i) const;	//判断Line是否为空 ，i可为LF，RG
	int IsEmpty();			//判断Line是否为空


	void MakeEmpty();//清空内容
	void MakeEmpty(int i);//清空内容 清空左右内容

	wchar_t Top(int i);//LG得到左侧元素 RG得到右侧元素
	void Push(const wchar_t c, int i);//插入一个字符 LF,插左，RG插右

	line Insert(wchar_t * cc);//插入字符串，返回插入字符串后当前行
	line Insert(wchar_t * cc, int &num);//插入字符串，返回插入字符串后当前行,num增加了插入字符串中的回车数。

	wchar_t Pop(int p);//删除一个字符，p=1删除光标后面的相当于del , p=-1删除光标前面的相当于backspace。
	void Rwrite(const wchar_t &c);//替换输入下一字符

	bool IsFirstL() { return this->pre->pre == nullptr; }
	bool IsLastL() { return this->next->next == nullptr; }

	//	int Savespace();//不再操作本Line（newLine或者point移出本行）时执行，若Line为空，释放数组。若gap>400，整理gap至400。
} *line;

class Article {
private:
	line firstL;
	line lastL;
	int lineNum;
public:
	line L;//目前正操作的Line头

	Article();//构造 linehead的next为空
	~Article();

	bool IsEmpty() const;
	bool IsFirstL(line& L) const { return L->pre == firstL; }
	bool IsLastL(line& L) const { return L->next == lastL; }
	bool IsEnd(line& L) const { return L == lastL; }

	line GetLine(int lineNum) const;
	int GetNum(line& L) const;
	int MaxWidth(HDC) const;

	void InsertAfter(line& L);
	void Remove(line &L);

	int LineNum(void) const { return lineNum; }
	void IncLineN(void) { lineNum++; }
	void clearWord(); //清空当前Article
	void Delete(int py, int px, int my, int mx); //删除 从 py行第px个字符右侧光标 到 my行第mx个字符右侧光标 之间的所有字符
	wchar_t* GetStr(int py, int px, int my, int mx); //复制 从 py行第px个字符右侧光标 到 my行第mx个字符右侧光标 之间的所有字符
	 /* 查找功能 */
	line onSearch(line tmpL, const wchar_t * t);
	int KMP(const wchar_t *s, const wchar_t *t);
	int * getNextVal(const wchar_t *s);

	/*替换功能*/
	line OnReplace(line tmpL, wchar_t * preStr,wchar_t * rpStr); //替换操作，preStr是查找的串，rpStr是待替换上的串

	/*给定坐标x（左条长+现鼠标与左端距离），行号y
	return值为：若点下后的gstart。超过行距返回glen*/
	int GetCharNum(int x, int y, HDC& hdc);
};

