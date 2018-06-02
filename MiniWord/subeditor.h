#pragma once

const int GapIncrement = 50;// gapArticleÿ�����Ӵ�С
const int DefaultSize = 100;//Ĭ��
typedef class Line * line;
enum stack { LF=-1 , RG=1 };

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

	ע��newLine ���У�line �ͣ���ָ��Ա����Ѵ��ڵ���һ�л���һ��,�����´�����һ��

~���Լ�ʵ�ָ���ճ��������Ҫ�µ�Article
*/

typedef class Line
{
public:
	int len;//��Ч�ַ�����
	int size;//Line���ܴ�С(�ַ�����)
	int gstart;//gapstart,gap��ʼλ�ã����λ�ã�������ڣ�
	int gend;//gapend,gap����λ��
	wchar_t* arr;//����

	line pre;//��һ��Line
	line next;//��һ��Line

	Line(int sz = DefaultSize);//����һ����Line����������ָ�룬����ռ�size
	~Line();//����һ��Line,����������ָ��
	line NewLine();//�ڱ�Line�����½�һ��Line

	int IsFull();//�ж�Line�Ƿ�Ϊ��
	int RleaseProcess();//�ͷ�gapbufferΪ0;
	void OverflowProcess(); //�������˺���������

	int PointMove(int p); //Ϊ�����������β�ƶ�pλ��Ϊ��������������ƶ�pλ��
	void PointMoveto(int d);//������ƶ�����d���ַ�
	//PointMove( -1 )		int LeftMovePoint();//���ı�ԭ�䣬�������
	//PointMove( 1 )		int RightMovePoint();//���ı�ԭ�䣬�������
	int Gapmove();//�ı�point���ƶ�gap

	int UsertoGap(int);//��������user�ַ���λ�ã�������arr�е���ʵλ��

	int Gapgsize();//ȡgap�Ŀ��;
	int GetPoint();//ȡgstart��Ŀǰ���λ�ã�
	int GetGend();//ȡgend;
	int Getsize();//ȡsize (�ܴ�С)ֵ
	int Getlen();//ȡlen ��Ч�ַ����ȣ��û������ַ����ȣ�
	int Getlen(int i) const;//ȡlen ��Ч�ַ����ȣ��û������ַ����ȣ�

	wchar_t * GetPos();//���ظ����ַ���ָ��
	wchar_t * GetPos(int i);//���������ַ��� LF, return arr ��RG �� return arr+gend
	wchar_t * GetStr();//�����ַ���;

	int CharWidth(HDC hdc);//�ַ�����
	int CharWidth(int i, HDC hdc) const;//i=1 �Ҳ࣬i=-1 ���

	int IsEmpty(int i) const;	//�ж�Line�Ƿ�Ϊ�� ��i��ΪLF��RG
	int IsEmpty();			//�ж�Line�Ƿ�Ϊ��


	void MakeEmpty();//�������
	void MakeEmpty(int i);//������� �����������

	wchar_t Top(int i);//LG�õ����Ԫ�� RG�õ��Ҳ�Ԫ��
	void Push(const wchar_t c, int i);//����һ���ַ� LF,����RG����

	line Insert(wchar_t * cc);//�����ַ��������ز����ַ�����ǰ��
	line Insert(wchar_t * cc, int &num);//�����ַ��������ز����ַ�����ǰ��,num�����˲����ַ����еĻس�����

	wchar_t Pop(int p);//ɾ��һ���ַ���p=1ɾ����������൱��del , p=-1ɾ�����ǰ����൱��backspace��
	void Rwrite(const wchar_t &c);//�滻������һ�ַ�

	bool IsFirstL() { return this->pre->pre == nullptr; }
	bool IsLastL() { return this->next->next == nullptr; }

	//	int Savespace();//���ٲ�����Line��newLine����point�Ƴ����У�ʱִ�У���LineΪ�գ��ͷ����顣��gap>400������gap��400��
} *line;

class Article {
private:
	line firstL;
	line lastL;
	int lineNum;
public:
	line L;//Ŀǰ��������Lineͷ

	Article();//���� linehead��nextΪ��
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
	void clearWord(); //��յ�ǰArticle
	void Delete(int py, int px, int my, int mx); //ɾ�� �� py�е�px���ַ��Ҳ��� �� my�е�mx���ַ��Ҳ��� ֮��������ַ�
	wchar_t* GetStr(int py, int px, int my, int mx); //���� �� py�е�px���ַ��Ҳ��� �� my�е�mx���ַ��Ҳ��� ֮��������ַ�
	 /* ���ҹ��� */
	line onSearch(line tmpL, const wchar_t * t);
	int KMP(const wchar_t *s, const wchar_t *t);
	int * getNextVal(const wchar_t *s);

	/*�滻����*/
	line OnReplace(line tmpL, wchar_t * preStr,wchar_t * rpStr); //�滻������preStr�ǲ��ҵĴ���rpStr�Ǵ��滻�ϵĴ�

	/*��������x��������+���������˾��룩���к�y
	returnֵΪ�������º��gstart�������о෵��glen*/
	int GetCharNum(int x, int y, HDC& hdc);
};

