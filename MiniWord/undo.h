#pragma once
#include "stdafx.h"
enum Operation{Ins,Del};
typedef class Undo {
public:
	selectPos Begin;
	int Op;//��¼ִ�й��Ĳ���

	selectPos End;//����ʱ����Endλ�ã�ɾ��ʱɾ����x,y �� End
	wchar_t * str;//ɾ��ʱ��¼���룬����ʱ��x,yλ�ò���str

	Undo(selectPos, selectPos);//����ʱ����Endλ�ã�ɾ��ʱɾ����x,y �� End
	Undo(selectPos, wchar_t *);//ɾ��ʱ��¼���룬����ʱ��x,yλ�ò���str
	~Undo();
}*undo;