#include <stdio.h>
#include <unistd.h>

/*
    File: even.c
    Author: Allison Marmura
    Date: 2026/03/12
    Description: This file contains a program that will print out the even
    integers from 0 to 58, and wait for the odd numbers to be printed by a
    separate program between each one.
*/
int main() 
{
    FILE* fp;
    
    for (int i = 0; i < 60; i = i + 1)
    {
        if ((i & 1) == 0) // checks if i is even
        {
            if (i != 0) // skip waiting for first turn
            {
                while ((fp = fopen("evenTurn", "r")) == NULL){} // stall until our turn
                fclose(fp);
                remove("evenTurn"); // delete file so we don't go again until odd program re-creates it
            }
            printf("%d\n", i); // \n terminates line and flushes buffer 
            fp = fopen("oddTurn", "w"); // create file to signal it is odds turn
            fclose(fp);
        }
    }
}
