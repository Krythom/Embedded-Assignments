#include <linux/kd.h> // KDSETLED
#include <sys/ioctl.h>
#include <unistd.h> // sleep()
#include <stdlib.h>
#include <stdio.h>

#define MAX_INDEX 7
#define MIN_INDEX 0

int main()
{
	int keybits[] = {0x0, 0x1, 0x4, 0x5, 0x2, 0x3, 0x6, 0x7}; //lights aren't in normal binary order so this is shuffled
	int input;

	printf("Enter A Number From 0 to 7\n");
	input = getchar() - '0'; // convert from charcter representation of digit to actual digit

	if (input < MIN_INDEX || input > MAX_INDEX) // outside bounds of array
	{
		printf("Invalid Input\n");
		return 1;
	}
	else
	{
		ioctl(1, KDSETLED, keybits[input]);
		sleep(1);
		ioctl(1, KDSETLED, 0x0); // turn lights off
	}
	return 0;
}

