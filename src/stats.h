
#ifndef STATS_H

typedef struct resultStats
{
  int completeReqCount;
  int failReqCount;
  int timeTaken;
  int threadCount;
  int portNo;
  char *contentLength;
  char *hostName;
} resultStats;

int checkStatusCode(char *statusCode);

void print_usage();

void print_stats();

#endif // !STATS_H
