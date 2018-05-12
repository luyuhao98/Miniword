#pragma once

const int GapIncrement = 50;// gapbuffer每次增加大小

class position {
	class piece* y;	//所在piece
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
	鼠标点击某行 或 刚刚编辑某行(p.IsPoint == 1)，
	记录此时光标到行头距离D=gstart。
	此后上下键，满足：
	若无新行(newpiece),光标不动。
	若D >= newpiece->len,则光标移动到新行的最后一位，point.x = newpiece->len;
	若D < newpiece->len,则光标移动到新行的D位置;

	注：newpiece 新行，piece* 型，是指相对本行已存在的上一行或下一行,不是新创建的一行

~若自己实现复制粘贴，则需要新的buffer
*/

class buffer {

	piece * head;//总piece头
	piece * p;//目前正操作的piece头
	int piecenum;//piece总数
	position point;
	position mark;
	buffer();//构造 piece *head的next为空
	~buffer();


	int readfile();		//读文件进buffer
	int writefile();	//写文件进buffer

	int ismarkpoint();//mark在point前返回1,mark在point后返回-1，同一位置返回0
	int marktopoint();//交换mark与point的位置
	int setpoint();//设置光标的位置
	int setmark();//设置mark

	int Prepiece(piece * &p);//p 移动到前一行
	int Nextpiece(piece * &p);//p 移动到后一行
	int InsertAfter(piece *& p);//p 后插入一行
	int Deletepiece(piece *& p); //删除p

	int GetPieceNum();//获取总行数
};

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
	piece * next;//下一个piece

//	int Pointpos;//point所在的位置

public:
	int Readfile();//从这行读文件 * ///
	int Writefile(); //用这行写入文件* ///

	piece(int sz = DefaultSize);//创建一个空piece，包括创建指针，申请空间size
	~piece();//析构一个piece,并连接上下指针

	int IsFull();//判断piece是否为满
	int IsEmpty();//判断piece是否为空
	int Newpiece();//在本piece后面新建一个piece，用在检测回车上。ps:若newpiece时执行savespace函数，结束本段编辑，将本段指针指向新一段开始编辑 this ->newp;
	int Savespace();//不再操作本piece（newpiece或者point移出本行）时执行，若piece为空，释放数组。若gap>400，整理gap至400。
	int RleaseProcess();//释放gapbuffer为0;
	void OverflowProcess(); //用于满了后申请数组

	int PointMove(int p); //为正，光标往行尾移动p位。为负，光标往行首移动p位。
	int PointMoveto(int d);//将光标移动到第d个字符
	
//PointMove( -1 )		int LeftMovePoint();//不改变原句，光标左移
//PointMove( 1 )		int RightMovePoint();//不改变原句，光标右移
	int Gapmove();//改变point后移动gap

	int Getlen();//取len 有效字符长度（用户眼中字符长度）
//无用	int Getlen(int m, int p);//取从数组index m 到index p 距离内(即传递mark与point的成员长)，有效字符串长度
	int UsertoGap(int );//传入面向user字符的位置，返回在arr中的真实位置
	int Getsize();//取size (总大小)值
	int Getgstart();//取gstart（目前光标位置）
	int Gapgsize();//取gap的宽度;
//无用	piece * Getpiece();//取目前piece的地址
	wchar_t * Getstr();//取有效字符串

	void Push(const wchar_t &c);//插入一个字符
	void Insert(const wchar_t * &cc);//插入字符串
	void Pop(int p);//删除一个字符，p=1删除光标后面的相当于del , p=-1删除光标前面的相当于backspace。
	void Delete();//删除标记mark到光标gstart.mark标记在第mark字符的右侧，mark+1的左侧。
	void Rwrite(const wchar_t &c);//替换输入下一字符
	void Rwrite(const wchar_t * &cc);//替换输入一串字符
	int Search(const wchar_t * &c, position &m, position &p);//找到所求字符串c的位置，并且起始为mark，结束为point
	int Replace(const wchar_t * &cc, const position m,const position p);//将mark到point处的字符串
	
	/*------------------------------*/


};