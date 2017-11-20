#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <string.h>

#include "utils.h"

void computeLPSArray(char *pat, int *f);
int *init_array(int size);

// Method that calls perror method and print out the error
void error(const char *msg)
{
  perror(msg);
  exit(0);
}

// Method that returns current time in seconds
int currentTime()
{
  time_t curtime;

  /* 
    Note: the `tm` structure has the following definition −
    struct tm {
      int tm_sec;         // seconds,  range 0 to 59         
      int tm_min;         // minutes, range 0 to 59          
      int tm_hour;        // hours, range 0 to 23             
      int tm_mday;        // day of the month, range 1 to 31  
      int tm_mon;         // month, range 0 to 11             
      int tm_year;        // The number of years since 1900   
      int tm_wday;        // day of the week, range 0 to 6    
      int tm_yday;        // day in the year, range 0 to 365  
      int tm_isdst;       // daylight saving time             
    }; 
    */
  struct tm *loctime;

  /* Get the current time. */
  curtime = time(NULL);

  /* Convert it to local time representation. */
  loctime = localtime(&curtime);

  int seconds = loctime->tm_hour * 60 * 60 + loctime->tm_min * 60 + loctime->tm_sec;
  return seconds;
}

// create lps[] that will hold the longest prefix suffix
// values for pattern
int *init_array(int size)
{
  int *arr = (int *)malloc(size * sizeof(int));
  int i;
  for (i = 0; i < size; i++)
  {
    arr[i] = 0;
  }
  return arr;
}

/*The Knutth Morris Pratt string searching algorithm
  (or KMP algorithm)searches for occurrences of
   a "pattern" P within a main "text string" S
   by employing the observation that when a mismatch
   occurs, the pattern itself embodies sufficient information
   to determine where the next match could begin,thus
   by passing re-examination of previously matched characters.
*/
//a function which prints occurrences of txt[] in pat[]
int KMPSearch(char *t, char *p)
{
  int m = strlen(p);
  int n = strlen(t);
  int *f = init_array(m);
  // Preprocess the pattern (calculate lps[] array)
  computeLPSArray(p, f);

  // index for txt[]
  int i = 0;
  // index for pat[]
  int j = 0;

  //As the while loop executes at most 2n times, showing that the time complexity of the search algorithm is O(n).
  while (i < n)
  {
    if (t[i] == p[j])
    {
      if (j == m - 1)
      {
        return i - j;
      }
      else
      {
        i += 1;
        j += 1;
      }
    }
    else
    {
      if (j > 0)
      {
        j = f[j - 1];
      }
      else
      {
        i += 1;
      }
    }
  }

  return -1;
}

// Fills lps[] for given patttern pat[0..M-1]
void computeLPSArray(char *pat, int *f)
{
  f[0] = 0;
  int i = 1;
  int j = 0;

  int m = strlen(pat);
  while (i < m)
  {
    if (pat[i] == pat[j])
    {
      f[i] = j + 1;
      i += 1;
      j += 1;
    }
    else if (j > 0)
    {
      j = f[j - 1];
    }
    else
    {
      f[i] = 0;
      i += 1;
    }
  }
}

/* 
 * @params {{char *}} text  -- the text to be searched
 * @params {{char *}} pattern -- the pattern to be sought
 * @returns {{int}} index of the found pattern value
*/
int valIndex(char *text, char *pattern)
{
  // @returns {{int}} (position in text at which pattern is found).
  int matchedIndex = KMPSearch(text, pattern);

  return matchedIndex + strlen(pattern) + 1;
}