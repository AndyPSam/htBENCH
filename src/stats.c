#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stats.h"
#include "inputs.h"

void print_usage()
{
  printf("Usage: htbench [options]\nOptions are:\n\t-o hostname Hostname of the web server\n\t-p portno portno of the web server\n\t-r request Number of requests to perform\n\t-d duration duration of the test in seconds\n\t-t threads total number of threads to use\n\t-h help Show the usage of the app\n\t-v verbose Provides verbose display of statistics");
  exit(2);
}

int checkStatusCode(char *statusCode)
{
  if (
      strcmp(statusCode, "100") == 0 ||
      strcmp(statusCode, "101") == 0 ||
      strcmp(statusCode, "102") == 0 ||
      strcmp(statusCode, "200") == 0 ||
      strcmp(statusCode, "201") == 0 ||
      strcmp(statusCode, "202") == 0 ||
      strcmp(statusCode, "203") == 0 ||
      strcmp(statusCode, "204") == 0 ||
      strcmp(statusCode, "205") == 0 ||
      strcmp(statusCode, "206") == 0 ||
      strcmp(statusCode, "207") == 0 ||
      strcmp(statusCode, "300") == 0 ||
      strcmp(statusCode, "301") == 0 ||
      strcmp(statusCode, "302") == 0 ||
      strcmp(statusCode, "303") == 0 ||
      strcmp(statusCode, "304") == 0 ||
      strcmp(statusCode, "305") == 0 ||
      strcmp(statusCode, "306") == 0 ||
      strcmp(statusCode, "307") == 0)
  {
    return 1;
  }
  else if (
      strcmp(statusCode, "400") != 0 ||
      strcmp(statusCode, "401") != 0 ||
      strcmp(statusCode, "402") != 0 ||
      strcmp(statusCode, "403") != 0 ||
      strcmp(statusCode, "404") != 0 ||
      strcmp(statusCode, "405") != 0 ||
      strcmp(statusCode, "406") != 0 ||
      strcmp(statusCode, "407") != 0 ||
      strcmp(statusCode, "408") != 0 ||
      strcmp(statusCode, "409") != 0 ||
      strcmp(statusCode, "410") != 0 ||
      strcmp(statusCode, "411") != 0 ||
      strcmp(statusCode, "412") != 0 ||
      strcmp(statusCode, "413") != 0 ||
      strcmp(statusCode, "414") != 0 ||
      strcmp(statusCode, "415") != 0 ||
      strcmp(statusCode, "416") != 0 ||
      strcmp(statusCode, "417") != 0 ||
      strcmp(statusCode, "418") != 0 ||
      strcmp(statusCode, "422") != 0 ||
      strcmp(statusCode, "423") != 0 ||
      strcmp(statusCode, "424") != 0 ||
      strcmp(statusCode, "425") != 0 ||
      strcmp(statusCode, "426") != 0 ||
      strcmp(statusCode, "449") != 0 ||
      strcmp(statusCode, "450") != 0 ||
      strcmp(statusCode, "500") != 0 ||
      strcmp(statusCode, "501") != 0 ||
      strcmp(statusCode, "502") != 0 ||
      strcmp(statusCode, "503") != 0 ||
      strcmp(statusCode, "504") != 0 ||
      strcmp(statusCode, "505") != 0 ||
      strcmp(statusCode, "506") != 0 ||
      strcmp(statusCode, "507") != 0 ||
      strcmp(statusCode, "509") != 0 ||
      strcmp(statusCode, "510") != 0)
  {
    return 0;
  }
  return 0;
}

void print_stats(void *arg)
{
  struct resultStats *resStat = arg;
  printf("\nStatistics:\n\tRunning %d sec test @ http://%s:%d/\n", resStat->timeTaken, resStat->hostName, resStat->portNo);
  printf("\tThread Count: %d\n", resStat->threadCount);
  printf("\tContent Length: %s\n", resStat->contentLength);
  printf("\tComplete Request Count: %d\n", resStat->completeReqCount);
  printf("\tFail Request Count: %d\n", resStat->failReqCount);
  exit(2);
}