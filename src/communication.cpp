#include "communication.h"

Button circle;
Button triangle;
Button square_;
Button cross;
Button right;
Button left;
Button up;
Button down;
Button r1;
Button r2;
Button l1;
Button l2;

int sleep;
int mode;

int jsrx;
int jsry;
int jslx;
int jsly;

int updateState(int *rcv)
{
	int err;
	err = checkSum(rcv);
	if(err != -1){
		updateButtonState(rcv[1], rcv[2]);
		updateJoyStickState(rcv[3], rcv[4], rcv[5], rcv[6]);
	}
	return err;
}

void updateButtonState(int button_state_higher, int button_state_lower)
{
	int button_state = 0x0000;
	button_state = (button_state_higher << 8) | button_state_lower;
	circle.press   = is(button_state & CIRCLE);
	triangle.press = is(button_state & TRIANGLE);
	square_.press   = is(button_state & SQUARE);
	cross.press    = is(button_state & CROSS);
	right.press    = is(button_state & RIGHT);
	up.press       = is(button_state & UP);
	left.press     = is(button_state & LEFT);
	down.press     = is(button_state & DOWN);
	r1.press       = is(button_state & R1);
	r2.press       = is(button_state & R2);
	l1.press       = is(button_state & L1);
	l2.press       = is(button_state & L2);
	
	circle.count   += pushCount(&circle);
	triangle.count += pushCount(&triangle);
	square_.count   += pushCount(&square_);
	cross.count    += pushCount(&cross);
	right.count    += pushCount(&right);
	up.count       += pushCount(&up);
	left.count     += pushCount(&left);
	down.count     += pushCount(&down);
	r1.count       += pushCount(&r1);
	r2.count       += pushCount(&r2);
	l1.count       += pushCount(&l1);
	l2.count       += pushCount(&l2);
	//////////±///////////
	circle.toggle   = circle.count%2;
	triangle.toggle = triangle.count%2;
	square_.toggle   = square_.count%2;
	cross.toggle    = cross.count%2;
	right.toggle    = right.count%2;
	up.toggle       = up.count%2;
	left.toggle     = left.count%2;
	down.toggle     = down.count%2;
	r1.toggle       = r1.count%2;
	r2.toggle       = r2.count%2;
	l1.toggle       = l1.count%2;
	l2.toggle       = l2.count%2;

	sleep    = is(button_state & SLEEP);
	mode     = is(button_state & MODE);
}

void updateJoyStickState(int _jsrx, int _jsry, int _jslx, int _jsly)
{
	jsrx = _jsrx - 127;
	jsry = _jsry - 127;
	jslx = _jslx - 127;
	jsly = _jsly - 127;
}
int pushCount(Button *any){
	if(any->press){
		if(any->flg == 0){
			any->flg = 1;
			return 1;
		}
	}else{
		any->flg = 0;
	}
	return 0;
}
int checkSum(int *rcv)
{
	int i,sum = 0;
	for(i = 0;i < 7;i++){
		sum += rcv[i] >> 1;
	}
	return (((sum % 0xFF)|0x01) == rcv[7]) ? 0 : -1;
}
int is(int n)
{
	return !!n;
}
