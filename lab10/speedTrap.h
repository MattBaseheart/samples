//ROTARY ENCODER	
	volatile unsigned char newRot, oldRot, rotChanged, rotA, rotB, rotInput, rotInputNEW; 
	volatile int maxSpeed;		// maxSpeed to display

//PHOTOTRANSISTORS
	volatile unsigned char photInput, firstP, secondP, prevTState, currTState;


//TIMER
	volatile unsigned char addTime;
	volatile double finalTime;
	volatile unsigned char c1, c2, c3, c4, s4, s3, s2, s1, r0, overTime, tooFast; 
	volatile char r1, r2, r3, r4;
	volatile unsigned int speed;
	volatile unsigned int recSpeed;
	
//BUZZER
	volatile unsigned char hi, finished;
	volatile unsigned int toneTime;
	
//SERIAL
	volatile unsigned char received[5];
	volatile unsigned char sz;
	volatile unsigned char showMessage, startRead;