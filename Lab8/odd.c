#include <stdio.h>
#include <unistd.h>

/*
    File: odd.c
    Author: Allison Marmura
    Date: 2026/03/12
    Description: This file contains a program that will print out the odd
    integers from 0 to 59, and wait for the even numbers to be printed by a
    separate program between each one.
*/
int main() 
{
    FILE *fp;

    for (int i = 0; i < 60; i = i + 1) 
    {
        if ((i & 1) == 1) // Check if i is odd  
        {
            while ((fp = fopen("oddTurn", "r")) == NULL){} // Stall until file exists
            fclose(fp);
            printf("%d\n", i);
            remove("oddTurn");  // Delete oddTurn file so it is not seen until re-made 
            fp = fopen("evenTurn", "w"); // Create file representing even's turn
            fclose(fp);
        }
    }
    remove("evenTurn"); // delete this file so it doesn't affect future runsS
}
