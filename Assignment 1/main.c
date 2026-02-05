/*
 * File: main.c
 * Author: Allison Marmura
 * Date: 2026/02/04
 * Description: Looks up either a single given http error code and prints out its information,
 * or prints out all http error codes within a given range. Extensive Input checking is done to
 * ensure that input is exactly of the form "[1-3 digit number]" or "[1-3 digit number]-[1-3 digit number]".
 */

#include "status.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static void Usage(const char *prog)
{
  fprintf(stderr, "Usage:\n");
  fprintf(stderr, "  %s <CODE>\n", prog);
  fprintf(stderr, "  %s <LOW>-<HIGH>\n", prog);
  fprintf(stderr, "Examples:\n");
  fprintf(stderr, "  %s 404\n", prog);
  fprintf(stderr, "  %s 200-204\n", prog);
}

// Takes a string and parses it for an integer value. Ignore surrounding whitespace.
static bool ParseInt(const char *s, int *out)
{
  // Get int ingoring whitespace at the start
  char *intEnd = malloc(sizeof(char));
  *out = strtol(s, &intEnd, 10);

  // Returns false if there are non-digit characters found after parsed int
  return (isspace(*intEnd) || *intEnd == '\0');
}

int main(int argc, char *argv[])
{

  int *low = malloc(sizeof(int));
  int *high = malloc(sizeof(int));

  if (argc != 2)
  {
    Usage(argv[0]); // Calls usage with program name (first argument from CLI)
    return 1;
  }

  char *arg = argv[1]; // argument passed in from CLI
  char *token = arg;
  int numbers = 1;

  // Scan input string to see how many numbers we should expect based on number of '-' characters
  while (*token != '\0')
  {
    if (*token == '-')
    {
      numbers++;
    }
    token++;
  }

  token = strtok(arg, "-");

  // Input Check One Checks for: empty except dashes, too many dashes, first number is invalid, first number is too many digits
  if (token == NULL || numbers > 2 || !ParseInt(token, low) || *low > 999)
  {
    printf("Error: Invalid Input\n");
    Usage(argv[0]);
    return 2;
  }

  if (numbers == 1)
  {
    // Valid single number input given
    if (LookupStatus(*low) != NULL)
    {
      PrintStatus(stdout, LookupStatus(*low));
    }
    else
    {
      printf("Error: HHTP status code %d not found\n", *low);
    }
  }
  else // Expecting a 2nd number
  {
    token = strtok(NULL, "-");

    // Input Check Two Checks: second num string not empty, second num valid, second num > first num, second num correct # of digits
    if (token != NULL && ParseInt(token, high) && *low < *high && *high < 1000)
    {
      PrintRange(stdout, *low, *high);
    }
    else
    {
      printf("Error: Invalid Input\n");
      Usage(argv[0]);
      return 2;
    }
  }

  return 0;
}
