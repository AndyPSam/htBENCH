#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#include "stats.h"
#include "inputs.h"

int completeReqCount = 0;
int failReqCount = 0;
int threadCount = 0;
int portNo = 0;
long timeTaken = 0;
long contentLength = 0;
long totalContentLength = 0;
char *hostName = "";

void print_usage()
{
  printf("Usage: htbench [options]\nOptions are:\n\t-o hostname Hostname of the web server\n\t-p portno portno of the web server\n\t-r request Number of requests to perform\n\t-d duration duration of the test in seconds\n\t-t threads total number of threads to use\n\t-h help Show the usage of the app\n\t-v verbose Provides verbose display of statistics\n");
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
  printf("\nDone!\n\nStatistics:\n");

  printf("\tServer Hostname:        %s\n", hostName);
  printf("\tServer Port:            %d\n", portNo);
  printf("\tThreads:                %d\n", threadCount);
  printf("\tTime elapsed:           %ld seconds\n", timeTaken);
  printf("\tComplete requests:      %d\n", completeReqCount);
  printf("\tFailed requests:        %d\n", failReqCount);
  printf("\tContent length:         %ld bytes (mean)\n", totalContentLength / (completeReqCount + failReqCount));
  printf("\tTotal content length:   %ld bytes\n", totalContentLength);
  printf("\tRequests per second:    %ld (mean)\n", (completeReqCount + failReqCount) / timeTaken);
  printf("\tTime per request:       %ld ms (mean)\n", (timeTaken * 1000) / (completeReqCount + failReqCount));
  printf("\tTransfer rate:          %ld bytes/sec\n", totalContentLength / (timeTaken ? timeTaken : 1));
  exit(2);
}