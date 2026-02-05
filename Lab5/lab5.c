#include <linux/kd.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

void changemode(int);
int kbhit();
void setLED(char *, char);

int main() {
  char bits = 1;
  char dir = 'r';
  char in = ' ';
  struct timespec T1, T2;

  clock_gettime(CLOCK_MONOTONIC, &T1);
  changemode(1);
  while (in != 'q') {
    while (!kbhit()) {
      clock_gettime(CLOCK_MONOTONIC, &T2);
      if (T2.tv_sec > T1.tv_sec) {
        setLED(&bits, dir);
        clock_gettime(CLOCK_MONOTONIC, &T1);
      }
    }

    in = getchar();
    if (in == 'r' || in == 'l') {
      dir = in;
    }
  }
  changemode(0);
  ioctl(1, KDSETLED, 0);
  return 0;
}

int kbhit() {
  struct timeval tv;
  fd_set rdfs;
  tv.tv_sec = 0;
  tv.tv_usec = 0;
  FD_ZERO(&rdfs);
  FD_SET(STDIN_FILENO, &rdfs);
  select(STDIN_FILENO + 1, &rdfs, NULL, NULL, &tv);
  return FD_ISSET(STDIN_FILENO, &rdfs);
}

void changemode(int mode) {
  static struct termios oldt, newt;
  if (mode == 1) {
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  } else {
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  }
}

void setLED(char *bits, char dir) {
  if (dir == 'l') {
    *bits >>= 1;
    if (*bits == 0) {
      *bits = 4;
    }
  } else if (dir == 'r') {
    *bits <<= 1;
    if (*bits == 8) {
      *bits = 1;
    }
  }
  ioctl(1, KDSETLED, *bits);
}
