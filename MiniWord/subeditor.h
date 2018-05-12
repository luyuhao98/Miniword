#pragma once

const int GapIncrement = 50;// gapbufferÿ�����Ӵ�С

class position {
	class piece* y;	//����piece
	int x;	//����Ч�ַ���λ��
};	//һ������point, һ����¼��mark
/*
���֣�
~x�ķ�Χ��
	0 �� size
	��������Ϊ��x���ַ�arr[x-1];�� x���Ϊarr[x-1],x�ұ�Ϊarr[x]
	0:����������û���ַ���
	size:�����ң�����������ַ�,��ʱ����arr�պ�����
~��괥���ԣ�
	����������ı������£����°��°��£���������һ��mark��
	̧����꼴����point����mark==point��delete mark
~���¼����λ�ã�
	�����ĳ�� �� �ոձ༭ĳ��(p.IsPoint == 1)��
	��¼��ʱ��굽��ͷ����D=gstart��
	�˺����¼������㣺
	��������(newpiece),��겻����
	��D >= newpiece->len,�����ƶ������е����һλ��point.x = newpiece->len;
	��D < newpiece->len,�����ƶ������е�Dλ��;

	ע��newpiece ���У�piece* �ͣ���ָ��Ա����Ѵ��ڵ���һ�л���һ��,�����´�����һ��

~���Լ�ʵ�ָ���ճ��������Ҫ�µ�buffer
*/

class buffer {

	piece * head;//��pieceͷ
	piece * p;//Ŀǰ��������pieceͷ
	int piecenum;//piece����
	position point;
	position mark;
	buffer();//���� piece *head��nextΪ��
	~buffer();


	int readfile();		//���ļ���buffer
	int writefile();	//д�ļ���buffer

	int ismarkpoint();//mark��pointǰ����1,mark��point�󷵻�-1��ͬһλ�÷���0
	int marktopoint();//����mark��point��λ��
	int setpoint();//���ù���λ��
	int setmark();//����mark

	int Prepiece(piece * &p);//p �ƶ���ǰһ��
	int Nextpiece(piece * &p);//p �ƶ�����һ��
	int InsertAfter(piece *& p);//p �����һ��
	int Deletepiece(piece *& p); //ɾ��p

	int GetPieceNum();//��ȡ������
};

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
	piece * next;//��һ��piece

//	int Pointpos;//point���ڵ�λ��

public:
	int Readfile();//�����ж��ļ� * ///
	int Writefile(); //������д���ļ�* ///

	piece(int sz = DefaultSize);//����һ����piece����������ָ�룬����ռ�size
	~piece();//����һ��piece,����������ָ��

	int IsFull();//�ж�piece�Ƿ�Ϊ��
	int IsEmpty();//�ж�piece�Ƿ�Ϊ��
	int Newpiece();//�ڱ�piece�����½�һ��piece�����ڼ��س��ϡ�ps:��newpieceʱִ��savespace�������������α༭��������ָ��ָ����һ�ο�ʼ�༭ this ->newp;
	int Savespace();//���ٲ�����piece��newpiece����point�Ƴ����У�ʱִ�У���pieceΪ�գ��ͷ����顣��gap>400������gap��400��
	int RleaseProcess();//�ͷ�gapbufferΪ0;
	void OverflowProcess(); //�������˺���������

	int PointMove(int p); //Ϊ�����������β�ƶ�pλ��Ϊ��������������ƶ�pλ��
	int PointMoveto(int d);//������ƶ�����d���ַ�
	
//PointMove( -1 )		int LeftMovePoint();//���ı�ԭ�䣬�������
//PointMove( 1 )		int RightMovePoint();//���ı�ԭ�䣬�������
	int Gapmove();//�ı�point���ƶ�gap

	int Getlen();//ȡlen ��Ч�ַ����ȣ��û������ַ����ȣ�
//����	int Getlen(int m, int p);//ȡ������index m ��index p ������(������mark��point�ĳ�Ա��)����Ч�ַ�������
	int UsertoGap(int );//��������user�ַ���λ�ã�������arr�е���ʵλ��
	int Getsize();//ȡsize (�ܴ�С)ֵ
	int Getgstart();//ȡgstart��Ŀǰ���λ�ã�
	int Gapgsize();//ȡgap�Ŀ��;
//����	piece * Getpiece();//ȡĿǰpiece�ĵ�ַ
	wchar_t * Getstr();//ȡ��Ч�ַ���

	void Push(const wchar_t &c);//����һ���ַ�
	void Insert(const wchar_t * &cc);//�����ַ���
	void Pop(int p);//ɾ��һ���ַ���p=1ɾ����������൱��del , p=-1ɾ�����ǰ����൱��backspace��
	void Delete();//ɾ�����mark�����gstart.mark����ڵ�mark�ַ����Ҳ࣬mark+1����ࡣ
	void Rwrite(const wchar_t &c);//�滻������һ�ַ�
	void Rwrite(const wchar_t * &cc);//�滻����һ���ַ�
	int Search(const wchar_t * &c, position &m, position &p);//�ҵ������ַ���c��λ�ã�������ʼΪmark������Ϊpoint
	int Replace(const wchar_t * &cc, const position m,const position p);//��mark��point�����ַ���
	
	/*------------------------------*/


};