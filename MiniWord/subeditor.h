#pragma once

const int GapIncrement = 50;// gapArticleÿ�����Ӵ�С
const int DefaultSize = 100;//Ĭ��
enum stack { LF=-1 , RG=1 };

class position {
	class Line* y;	//����Line
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
	�����ĳ�� �� �ոձ༭ĳ��
	��¼��ʱ��굽��ͷ����D=gstart��
	�˺����¼������㣺
	��������(newLine),��겻����
	��D >= newLine->len,�����ƶ������е����һλ��point.x = newLine->len;
	��D < newLine->len,�����ƶ������е�Dλ��;

	ע��newLine ���У�Line* �ͣ���ָ��Ա����Ѵ��ڵ���һ�л���һ��,�����´�����һ��

~���Լ�ʵ�ָ���ճ��������Ҫ�µ�Article
*/

class Line
{
private:
	int len;//��Ч�ַ�����
	int size;//Article���ܴ�С(�ַ�����)
	int mark;//Mark��������Ϊ-1���������У�markΪ��ʵ��λ��
	int gstart;//gapstart,gap��ʼλ�ã����λ�ã�������ڣ�
	int gend;//gapend,gap����λ��

	wchar_t * arr;//����

public:
	Line * pre;//��һ��Line
	Line * next;//��һ��Line


	Line(int sz = DefaultSize);//����һ����Line����������ָ�룬����ռ�size
	~Line();//����һ��Line,����������ָ��
	Line* NewLine();//�ڱ�Line�����½�һ��Line

	int IsFull();//�ж�Line�Ƿ�Ϊ��
	int RleaseProcess();//�ͷ�gapbufferΪ0;
	void MakeEmpty();//�������
	void OverflowProcess(); //�������˺���������


	int PointMove(int p); //Ϊ�����������β�ƶ�pλ��Ϊ��������������ƶ�pλ��
	void PointMoveto(int d);//������ƶ�����d���ַ�
	//PointMove( -1 )		int LeftMovePoint();//���ı�ԭ�䣬�������
	//PointMove( 1 )		int RightMovePoint();//���ı�ԭ�䣬�������
	int Gapmove();//�ı�point���ƶ�gap

		int UsertoGap(int);//��������user�ַ���λ�ã�������arr�е���ʵλ��
	int Getsize();//ȡsize (�ܴ�С)ֵ
	
	int GetPoint();//ȡgstart��Ŀǰ���λ�ã�
	int Gapgsize();//ȡ  gap�Ŀ��;

	wchar_t * GetPos(int i);//LF, ���׵�һ���� ��RG �� ����һ���ַ�λ��

	int Getlen();//ȡlen ��Ч�ַ����ȣ��û������ַ����ȣ�
	int Getlen(int i) const;//ȡlen ��Ч�ַ����ȣ��û������ַ����ȣ�


	int CharWidth();//�ַ�����
	int CharWidth(int i) const;//i=1 �Ҳ࣬i=-1 ���
	
	int IsEmpty(int i) const;	//�ж�Line�Ƿ�Ϊ�� ��i��ΪLF��RG
	int IsEmpty();			//�ж�Line�Ƿ�Ϊ��


	wchar_t Top(int i);//LG�õ����Ԫ�� RG�õ��Ҳ�Ԫ��
	void Push(const wchar_t c,int i);//����һ���ַ� LF,����RG����
	void Insert(const wchar_t * &cc);//�����ַ���
	wchar_t Pop(int p);//ɾ��һ���ַ���p=1ɾ����������൱��del , p=-1ɾ�����ǰ����൱��backspace��
	
	void Delete();//ɾ�����mark�����gstart.mark����ڵ�mark�ַ����Ҳ࣬mark+1����ࡣ
	void Rwrite(const wchar_t &c);//�滻������һ�ַ�
	void Rwrite(const wchar_t * &cc);//�滻����һ���ַ�
	
	bool IsFirstL() { return this->pre->pre == nullptr; }
	bool IsLastL() { return this->next->next == nullptr; }

	//	int Savespace();//���ٲ�����Line��newLine����point�Ƴ����У�ʱִ�У���LineΪ�գ��ͷ����顣��gap>400������gap��400��
	//int Readfile();//�����ж��ļ� * ///
	//int Writefile(); //������д���ļ�* ///
};


class Article {
private:
	Line * firstL;
	Line * lastL;
	int lineNum;
public:
	Line * L;//Ŀǰ��������Lineͷ

	position point;
	position mark;
	Article();//���� Line *head��nextΪ��
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
