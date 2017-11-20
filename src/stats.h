
#ifndef STATS_H

extern int completeReqCount;
extern int failReqCount;
extern int threadCount;
extern int portNo;
extern long timeTaken;
extern long contentLength;
extern long totalContentLength;
extern char *hostName;

int checkStatusCode(char *statusCode);

void print_usage();

void print_stats();

#endif // !STATS_H
