#ifndef STATS_H

/*****  GLOBALS for the results variables ****/
extern int completeReqCount;
extern int failReqCount;
extern int threadCount;
extern int portNo;
extern long timeTaken;
extern long contentLength;
extern long totalContentLength;
extern char *hostName;
/********************************************/

/**
 * @params {{char *}} statusCode -- Status code of the executed HTTP request
 * @returns {{int}} 1 or 0 for whether status code is 2XX or 3XX and 4XX or 5XX respectively
  */
int checkStatusCode(char *statusCode);

/**
 * Prints the usage details of the application
  */
void print_usage();

/**
 * Prints the statistics about the benchmark
  */
void print_stats();

#endif // !STATS_H
