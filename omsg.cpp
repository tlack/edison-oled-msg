// omsg displays stdin on the edison's OLED display
//

#define SEC 1000000
#define DRAWSLEEP 250
#define DRAWNOTFOUNDSLEEP 1000
#define INPUTLOOPS 1000000
#define NOINPUTSLEEP 100
#define STARTSLEEP 1 * SEC
#define TICKSLEEP 500
#define EFFECT_INTERVAL 10000000

#include <iostream>
#include "oled/Edison_OLED.h"
#include "gpio/gpio.h"
#include "math.h"
#include <fcntl.h>
#include <unistd.h> // for usleep
#include <stdlib.h> // Gives us atoi
#include <stdio.h>

using namespace std;

// Function prototypes:
void setupOLED();
void cleanUp();

// Global state
unsigned long Ticks;

// Define an edOLED object:
edOLED oled;

// Pin definitions:
// All buttons have pull-up resistors on-board, so just declare
// them as regular INPUT's
gpio BUTTON_UP(47, INPUT);
gpio BUTTON_DOWN(44, INPUT);
gpio BUTTON_LEFT(165, INPUT);
gpio BUTTON_RIGHT(45, INPUT);
gpio BUTTON_SELECT(48, INPUT);
gpio BUTTON_A(49, INPUT);
gpio BUTTON_B(46, INPUT);

void emit(const char* msg) {
	printf("%s\n",msg);
}
void emits(const char* msg, const char* s) {
	printf("%s: %s\n", msg, s);
}

void die(const char* msg, const char* extra) {
	if(extra!=NULL) printf("ERROR: %s (%s)\n", msg, extra);
	else printf("ERROR: %s\n", msg);
	oled.clear(PAGE);
	oled.setCursor(0,0);
	oled.print(msg);
	if (extra!=NULL) oled.print(extra);
	exit(EXIT_FAILURE);
}

void setupOLED() {
	oled.begin();
	oled.clear(PAGE);
	oled.display();
	oled.setFontType(0);
}

void tick() {
	Ticks++;
	if(Ticks % EFFECT_INTERVAL) {
		/*
		put some pleasant effects in here.
		crackling fireplace, etc.

		int x=rand() % LCDWIDTH;
		int y=rand() % LCDHEIGHT;
		oled.setColor(WHITE);
		oled.pixel(x,y,WHITE,XOR);
		// oled.invert(1);
		oled.display();
		*/
	} else {
		usleep(TICKSLEEP);
	}
}

void btn(const char* action) {
	char buf[80];
	snprintf(buf,80,"%s",action);
	printf(buf);
}

int inp() {
	// Wait for either button A or B to be pressed:
	#define PIN_ON(pin) (BUTTON_##pin.pinRead()==LOW)
	#define PIN_DISPATCH(pin) if(PIN_ON(pin)) { btn("" #pin ""); return 1; }
	emit("inp");
	int i;
	for (i=0; i<INPUTLOOPS; i++) {
		PIN_DISPATCH(A);
		PIN_DISPATCH(B);
		PIN_DISPATCH(UP);
		PIN_DISPATCH(DOWN);
		PIN_DISPATCH(LEFT);
		PIN_DISPATCH(RIGHT);
		PIN_DISPATCH(SELECT);
		tick();
	}
	return 0;
}

void draw(const char* str)
{
	emit("draw");
	oled.setCursor(0,0);
	oled.print(str);
	oled.display();
}

void cleanUp()
{
	emit("cleanUp");
	oled.clear(PAGE);
	oled.display();
}

char* stdinA() { 
	#define BUFSZ 32 * 1024
	char* buf=(char*)calloc(BUFSZ,1);
	int ofs=0, nread;
	while(nread=fread(&(buf[ofs]), 1, BUFSZ-ofs-1, stdin)) {
		ofs+=nread;
	}
	if (ofs) return buf;
	else return NULL;
}

int main(int argc, char * argv[])
{
	setupOLED();
	char* inputA=stdinA();
	if (inputA) {
		emits("input", inputA);
		draw(inputA);
		free(inputA);
	}
	inp();
	exit(EXIT_SUCCESS);
}

