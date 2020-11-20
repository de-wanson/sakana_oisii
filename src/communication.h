#ifndef _SIXAXIS_H_
#define _SIXAXIS_H_

#define CIRCLE		0x8000
#define TRIANGLE	0x4000
#define SQUARE		0x2000
#define CROSS		0x1000
#define RIGHT		0x0800
#define UP		0x0400
#define LEFT		0x0200
#define DOWN		0x0080
#define R1		0x0040
#define R2		0x0020
#define L1		0x0010
#define L2		0x0008
#define SLEEP		0x0004
#define MODE		0x0002

struct Button{
public:
	int press;
	int count;
	int flg;
	int toggle;
	Button(){
		this->press = 0;
		this->count = 0;
		this->flg = 0;
		this->toggle = 0;
	}
};

extern Button circle;
extern Button triangle;
extern Button square_;
extern Button cross;
extern Button right;
extern Button left;
extern Button up;
extern Button down;
extern Button r1;
extern Button r2;
extern Button l1;
extern Button l2;

extern int sleep;
extern int mode;

extern int jsrx;
extern int jsry;
extern int jslx;
extern int jsly;

int updateState(int *rcv);
void updateButtonState(int button_state_higher, int button_state_lower);
void updateJoyStickState(int _jsrx, int _jsry, int _jslx, int _jsly);
int checkSum(int *rcv);
int pushCount(Button *any);
int is(int n);
#endif