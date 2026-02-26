#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <wiringPi.h>

#define RIGHT 17
#define QUIT 27
#define LEFT 22
#define OUTPUT 1
#define INPUT 0
#define HIGH 1
#define LOW 0

const int LEDS[] = {13, 19, 26};
const int BUTTONS[] = {RIGHT, QUIT, LEFT};
const int NUM_BUTTONS = 3;
const int NUM_LEDS = 3;

int cycle(int, int);
int getButton();

int main()
{       
        wiringPiSetupGpio();
        struct timespec T1, T2;

	// Starts cycling right
	int dir = RIGHT;
	int button = 0;
	int lit = 0;
        
        for (int i = 0; i < NUM_LEDS; i++)        
        {
                pinMode(LEDS[i], OUTPUT);                
        }       
        for (int i = 0; i < NUM_BUTTONS; i++)
        {
                pinMode(BUTTONS[i], INPUT);
        }

        clock_gettime(CLOCK_MONOTONIC, &T1);
	while (button != QUIT)
	{	
		button = getButton();
		if (button == RIGHT || button == LEFT)
		{
			dir = button;
		}
                clock_gettime(CLOCK_MONOTONIC, &T2);
                if (T1.tv_sec < T2.tv_sec)
                {
                        lit = cycle(dir, lit);
                        clock_gettime(CLOCK_MONOTONIC, &T1);
                }
	}

	return 0;
}

int cycle(int dir, int lit)
{
	if (dir == RIGHT)
	{
		// sub 1 mod leds
		lit = (lit + (NUM_LEDS - 1)) % NUM_LEDS;
	}
	else if (dir == LEFT)
	{	
		// add 1 mod leds
		lit = (lit + 1) % NUM_LEDS;
	}
	
	// Set LEDs
	for (int i = 0; i < NUM_LEDS; i++)
	{	
		if (i == lit)
		{
			digitalWrite(LEDS[i], HIGH);
		}
		else
		{
			digitalWrite(LEDS[i], LOW);
		}
	}
        return lit;
}

int getButton()
{
        for (int i = 0; i < NUM_BUTTONS; i++)
        {
                if (digitalRead(BUTTONS[i]))
                {
                        return BUTTONS[i];
                }
        }
}
