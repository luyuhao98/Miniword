#include "subeditor.h"
//#include "MiniWord.h"
enum Operation
{
	Insert,
	Delete
};

typedef class Undo {
public:
	int x;
	line y;

	Operation Op;

	union {
		selectPos End;
		wchar_t * str;
	}key;

}*undo;