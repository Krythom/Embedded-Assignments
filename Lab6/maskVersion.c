#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>

void pinMode(int, int, volatile unsigned int*);
void setPin(int, int, volatile unsigned int*);
int readPin(int, volatile unsigned int*);
int getButton(volatile unsigned int*);
void cycle(int, int*, volatile unsigned int*);

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


int main()
{	
	unsigned int BASE = 0xFE200000;
	volatile unsigned int* GPIO;
	int MEM;

	// Memory access setup
	if (getuid() != 0)
	{
		printf("ROOT PRIVELEGES REQUIRED\n");
		return 1;
	}
	if ((MEM = open("/dev/mem", O_RDWR | O_SYNC)) < 0)
	{
		printf("CANNOT OPEN MEMORY INTERFACE\n");
		return 2;
	}

	GPIO = (unsigned int*)mmap(0, getpagesize(), PROT_READ | PROT_WRITE, MAP_SHARED, MEM, BASE);

	if ((unsigned int*)GPIO < 0)
	{
		printf("MEMORY MAPPING FAILED\n");
		return 3;
	}
		
	// Set up pin modes
	for (int i = 0; i < NUM_LEDS; i++)
	{
		pinMode(LEDS[i], OUTPUT, GPIO);
	}
	for (int i = 0; i < NUM_BUTTONS; i++)
	{
		pinMode(BUTTONS[i], INPUT, GPIO);
	}
        
        struct timespec T1, T2;

	// Starts cycling right
	int dir = RIGHT;
	int button = 0;
	int* lit;
	*lit = 0; 

        clock_gettime(CLOCK_MONOTONIC, &T1);
	while (button != QUIT)
	{	
		button = getButton(GPIO);
		if (button == RIGHT || button == LEFT)
		{
			dir = button;
		}
                clock_gettime(CLOCK_MONOTONIC, &T2);
                if (T1.tv_sec < T2.tv_sec)
                {
                        cycle(dir, lit, GPIO);
                        clock_gettime(CLOCK_MONOTONIC, &T1);
                }
	}

	return 0;
}

void cycle(int dir, int* lit, volatile unsigned int* GPIO)
{
	if (dir == RIGHT)
	{
		// sub 1 mod leds
		*lit = (*lit + (NUM_LEDS - 1)) % NUM_LEDS;
	}
	else if (dir == LEFT)
	{	
		// add 1 mod leds
		*lit = (*lit + 1) % NUM_LEDS;
	}
	
	// Set LEDs
	for (int i = 0; i < NUM_LEDS; i++)
	{	
		if (i == *lit)
		{
			setPin(LEDS[i], HIGH, GPIO);
		}
		else
		{
			setPin(LEDS[i], LOW, GPIO);
		}
	}
}

// mode: 0 = input, >0 = output
void pinMode(int pin, int mode, volatile unsigned int* GPIO)
{
	int regIndex = (pin / 10);
	int bitPos = (pin % 10) * 3;
	int mask = ~(0x07 << bitPos); // All 1s except for designated pin's bits
	*(GPIO + regIndex) &= mask;
	if (mode)
	{
		*(GPIO + regIndex) |= (1 << bitPos); // Sets output bit
	}
}

void setPin(int pin, int out, volatile unsigned int* GPIO)
{
	if (out)
	{
		*(GPIO + 7) = (1 << pin); // Set pin bit high
	}
	else
	{
		*(GPIO + 10) = (1 << pin); // Set pin bit low
	}
}

int readPin(int pin, volatile unsigned int* GPIO)
{
	return *(GPIO + 13) & (1 << pin); // Read pin bit and return it
}

int getButton(volatile unsigned int* GPIO)
{
	for (int i = 0; i < NUM_BUTTONS; i++)
	{	
		if (readPin(BUTTONS[i], GPIO))
		{
			return BUTTONS[i];
		}
	}
	return 0;
}
