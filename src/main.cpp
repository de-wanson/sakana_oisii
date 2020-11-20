#include <Arduino.h>
#include <MsTimer2.h>
#include <Servo.h>
#include "communication.h"
#include "arm.h"
#include "define.h"

unsigned long time = -1;
int arm_count = 0;
int arm_blank = 0;
int arm_temp = 0;
unsigned long int s1_count = 0;
unsigned long int s2_count = 0;

int hold_timing[2];

int No = -1;
int data[10];
void communication();

int auto_phase = 0;
unsigned long  time_ft = 0;
bool flg_ft[2];
bool no_vibrate_flg = true;
unsigned long  press_time = 0;


Arm arm_1(RF_1_1, RF_1_2, RF_1_P, SW_1, VOL_1);
Arm arm_2(RF_2_1, RF_2_2, RF_2_P, SW_2, VOL_2);
Servo sv_1;
Servo sv_2;

void arms() {
	static int task = -1;
	static int arm_mode = 0;
	static int arm_flg = 0;
	static int sv_flg = 0;
	if (task == -1 && circle.press) {
		task = 0;
		arm_blank = 0;
		sv_flg = 1;
	}
	if (task == -1 && cross.press) {
		task = 1;
		arm_blank = 0;
		arm_flg = 1;
		sv_flg = 1;
	}
	if(task == 0){
		if(arm_count){
			if(arm_blank < 500){
				sv_1.write(OPEN_1);
			}else if((analogRead(VOL_1) > ARM_RL || analogRead(VOL_2) < ARM_LH)&&sv_flg){
				arm_mode = 1;
				arm_temp = arm_blank;
			}else if(arm_blank <= arm_temp + 700){
				arm_mode = 0;
				sv_flg = 0;
				if(arm_blank >= arm_temp + 200){
					sv_1.write(CLOSE_1);
					arm_mode = 0;
				}
			}else{
				arm_count = !arm_count;
				task = -1;
			}
		}else{
			if(arm_blank < 500){
				sv_2.write(OPEN_2);
			}else if((analogRead(VOL_2) > ARM_LL || analogRead(VOL_1) < ARM_RH)&&sv_flg){
				arm_mode = 2;
				arm_temp = arm_blank;
			}else if(arm_blank <= arm_temp + 700){
				sv_flg = 0;
				arm_mode = 0;
				if(arm_blank >= arm_temp + 200){
					sv_2.write(CLOSE_2);
					arm_mode = 0;
				}
			}else{
				arm_count = !arm_count;
				task = -1;
			}
		}
	}

	if(task == 1){
		if(arm_count){
			if(arm_blank < 500){
				sv_2.write(OPEN_2);
			}else if(analogRead(VOL_2) < ARM_LH && arm_flg){
				arm_mode = 3;
			}else if((analogRead(VOL_2) > ARM_LL)&&sv_flg){
				arm_mode = 2;
				arm_flg = 0;
				arm_temp = arm_blank;
			}else if(arm_blank <= arm_temp +700){
				sv_flg = 0;
				arm_mode = 0;
				if(arm_blank >= arm_temp + 200){
					sv_2.write(CLOSE_2);
					arm_mode = 0;
				}
			}else{
				task = -1;
			}
		}else{
			if(arm_blank < 500){
				sv_1.write(OPEN_1);
			}else if(analogRead(VOL_1) < ARM_RH && arm_flg){
				arm_mode = 4;
			}else if((analogRead(VOL_1) > ARM_RL)&&sv_flg){
				arm_mode = 1;
				arm_flg = 0;
				arm_temp = arm_blank;
			}else if(arm_blank <= arm_temp + 700){
				sv_flg = 0;
				arm_mode = 0;
				if(arm_blank >= arm_temp + 200){
					sv_1.write(CLOSE_1);
					arm_mode = 0;
				}
			}else{
				task = -1;
			}
		}
	}

	switch (arm_mode){
	case 0:
		arm_1.move(0);
		arm_2.move(0);
		break;
	case 1:
		if(analogRead(VOL_2) < ARM_LH){
			arm_1.move(0);
			if(abs(analogRead(VOL_2) - ARM_LH) < 50){
				arm_2.move(48);
			}else{
				arm_2.move(64);
			}
		}else if(analogRead(VOL_1) > ARM_RL){
			arm_2.move(0);
			if(abs(analogRead(VOL_1) - ARM_RL) < 50){
				arm_1.move(-48);
			}else{
				arm_1.move(-64);
			}
		}else{
			arm_1.move(0);
		}
		break;
	case 2:
		if(analogRead(VOL_1) < ARM_RH){
			arm_2.move(0);
			if(abs(analogRead(VOL_1) - ARM_RH) < 50){
				arm_1.move(48);
			}else{
				arm_1.move(64);
			}
		}else if(analogRead(VOL_2) > ARM_LL){
			arm_1.move(0);
			if(abs(analogRead(VOL_2) - ARM_LL) < 50){
				arm_2.move(-48);
			}else{
				arm_2.move(-64);
			}
		}else{
			arm_2.move(0);
		}
		break;
	case 3:
		if(analogRead(VOL_2) < ARM_LH){
			arm_2.move(0);
			if(abs(analogRead(VOL_2) - ARM_LH) < 50){
				arm_2.move(48);
			}else{
				arm_2.move(64);
			}
		}else{
			arm_2.move(0);
		}
		break;
	case 4:
		if(analogRead(VOL_1) < ARM_RH){
			arm_1.move(0);
			if(abs(analogRead(VOL_1) - ARM_RH) < 50){
				arm_1.move(48);
			}else{
				arm_1.move(64);
			}
		}else{
			arm_1.move(0);
		}
		break;
	default:
		break;
	}
	Serial.print(task);
	Serial.print(", ");
	Serial.print(analogRead(VOL_1));
	Serial.print(", ");
	Serial.print(analogRead(VOL_2));
	Serial.print(", ");
	Serial.print(sv_1.read());
	Serial.print(", ");
	Serial.print(sv_2.read());

}

void optionArm(){
	arm_1.move(0);
	arm_2.move(0);
	if(square_.toggle){
		if(cross.press && analogRead(VOL_1) < ARM_RH){
			if(down.press){
				arm_1.move(SPD_LOW);
			}else{
				arm_1.move(SPD_HIGH);
			}
		}else if(circle.press){
			if(analogRead(VOL_1) > ARM_RL){
				if(down.press){
					arm_1.move(-SPD_LOW);
				}else{
					arm_1.move(-SPD_HIGH);
				}
				if(analogRead(VOL_1) < HOLD_R)sv_1.write(CLOSE_1);
			}
			arm_1.move(SPD_HIGH);
		}	
	}else{
		if(cross.press && analogRead(VOL_2) < ARM_LH){
			if(down.press){
				arm_2.move(SPD_LOW);
			}else{
				arm_2.move(SPD_HIGH);
			}
		}else if(circle.press){
			if(analogRead(VOL_2) > ARM_LL){
				if(down.press){
					arm_2.move(-SPD_LOW);
				}else{
					arm_2.move(-SPD_HIGH);
				}
				if(analogRead(VOL_2) < HOLD_L)sv_2.write(CLOSE_2);
			}
		}
	}
	
	if(!r1.press){
		//sv_1.write(CLOSE_1);
		hold_timing[0] = analogRead(VOL_1);
	}
	else{ 
		sv_1.write(OPEN_1);
	}
	if(!l1.press){
		//sv_2.write(CLOSE_2);
		hold_timing[1] = analogRead(VOL_2);
	}
	else{
		sv_2.write(OPEN_2);
	}
}

void autoArm(){
	arm_1.move(0);
	arm_2.move(0);
	if(circle.press&&time - press_time > 400){
		press_time = time;
		auto_phase++;
	}
	if(cross.press&&time - press_time > 1000){
		press_time = time;
		auto_phase = 0;
	}
	if(square_.press&&time - press_time > 400){
		press_time = time;
		no_vibrate_flg = true;
		if(auto_phase == 1)auto_phase = 4;
		else if(auto_phase == 2)auto_phase = 5;
	}
	switch (auto_phase)
	{
	case 0:		
		if(analogRead(VOL_1) < PUT_R){
			arm_1.move(SPD_HIGH);
		}
		if(analogRead(VOL_2) < PUT_L){
			arm_2.move(SPD_HIGH);
		}
		sv_1.write(OPEN_1);
		sv_2.write(OPEN_2);
		break;
	case 1:
		if(flg_ft[0]){
			sv_2.write(OPEN_2);
			flg_ft[0] = false;
			time_ft = time;
		}
		if(time - time_ft > 600){
			if(analogRead(VOL_1) < ARM_RH){
				arm_1.move(SPD_HIGH);
			}
			if(analogRead(VOL_1) > STRECH_R){
				if(analogRead(VOL_2) > ARM_LL){
					if(analogRead(VOL_2) > HOLD_L+60){
						arm_2.move(-SPD_HIGH);
					}else{
						if(no_vibrate_flg)arm_2.move(-SPD_HOLD);
					}
					if(analogRead(VOL_2) < HOLD_L){
						arm_1.move(0);
						sv_2.write(CLOSE_2);
					}
				}else{
					no_vibrate_flg = false;
				}
			}
		}
		break;
	case 2:
		if(flg_ft[1]){
			sv_1.write(OPEN_1);
			flg_ft[1] = false;
			time_ft = time;
		}
		if(time - time_ft > 600){
			if(analogRead(VOL_2) < ARM_LH){
				arm_2.move(SPD_HIGH);
			}
			if(analogRead(VOL_2) > STRECH_L){
				if(analogRead(VOL_1) > ARM_RL){
					if(analogRead(VOL_1) > HOLD_R+60){
						arm_1.move(-SPD_HIGH);
					}else{
						if(no_vibrate_flg)arm_1.move(-SPD_HOLD);
					}
					if(analogRead(VOL_1) < HOLD_R){
						arm_2.move(0);
						sv_1.write(CLOSE_1);
					}
				}else{
					no_vibrate_flg = false;
				}
			}
		}
		//if(time - time_ft > 3000)sv_1.write(CLOSE_1);
		break;
	case 3:
		flg_ft[0] = flg_ft[1] = true;
		auto_phase = 1;
		no_vibrate_flg = true;
		break;
	case 4:
		sv_2.write(OPEN_2);
		if(analogRead(VOL_2) < REHOLD_L){
			arm_2.move(SPD_HIGH);
		}else{
			auto_phase = 1;
		}
		break;
	case 5:
		sv_1.write(OPEN_1);
		if(analogRead(VOL_1) < REHOLD_R){
			arm_1.move(SPD_HIGH);
		}else{
			auto_phase = 2;
		}
		break;
	}
}


void serialEvent1(){
    while(Serial1.available() > 0){
       communication();
    }
}

void communication() {
	int buff = -1;
	buff = Serial1.read();
	if (buff == 0x80) {
		No = 0;
		data[No++] = 0x80;
	} else if (No > 0) {
		data[No++] = buff;
		if (No > 7) {
			updateState(data);
			No = -1;
		}
	}
}

void timer() {
	if (time >= 0) {
		time++;
	}
	if(time > 10000000){
		time = 0;
		press_time = time_ft = 0;
	}
	arm_1.execute();
	arm_2.execute();
	arm_blank++;
	s1_count++;
	s2_count++;
}

void setup() {
	Serial.begin(9600);
	Serial1.begin(38400);
	MsTimer2::set(1, timer);
	MsTimer2::start();
	sv_1.attach(SERVO_1, 800, 2200);
	sv_2.attach(SERVO_2, 800, 2200);
	pinMode(PAPER,OUTPUT);
	arm_1.move(0);
	//sv_1.write(OPEN_1);
	arm_2.move(0);
	//sv_2.write(OPEN_2);
	flg_ft[0] = flg_ft[1] = true;
	time = 0;
	delay(300);
}

void loop() {
	if(triangle.press){
		digitalWrite(PAPER,HIGH);
	}else{
		digitalWrite(PAPER,LOW);
	}
	autoArm();
	Serial.print(circle.press);
	Serial.print(", ");
	Serial.print(cross.press);
	Serial.print(", ");
	Serial.print(triangle.press);
	Serial.print(", ");
	Serial.print(square_.press);
	Serial.print(", ");
	Serial.print(auto_phase);
	Serial.print(", ");
	Serial.print(time);
	Serial.print(", ");
	Serial.print(time_ft);
	Serial.print(", ");
	Serial.print(square_.toggle);
	Serial.print(", ");
	Serial.print(r1.toggle);
	Serial.print(", ");
	Serial.print(l1.toggle);
	Serial.print(",");
	Serial.print(analogRead(VOL_1));
	Serial.print(", ");
	Serial.print(analogRead(VOL_2));

	Serial.println();
}
