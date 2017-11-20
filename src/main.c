#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <ctype.h>
#include <getopt.h>
#include <signal.h>

#include "inputs.h"
#include "httpMethods.h"
#include "constants.h"
#include "utils.h"
#include "stats.h"

int main(int argc, char **argv)
{

  signal(SIGINT, print_stats);

  if (argc < 2)
  {
    print_usage();
  }

  struct arg_struct args;

  /* A string listing valid short options letters.  */
  const char *const short_options = "hvo:p:d:t:r:";
  /* An array describing valid long options.  */
  const struct option long_options[] = {
      {"help", 0, NULL, 'h'},
      {"verbose", 0, NULL, 'v'},
      {"hostname", 1, NULL, 'o'},
      {"portno", 1, NULL, 'p'},
      {"duration", 1, NULL, 'd'},
      {"threads", 1, NULL, 't'},
      {"requests", 1, NULL, 'r'},
      {NULL, 0, NULL, 0} /* Required at end of array.  */
  };

  int next_option;
  do
  {
    next_option = getopt_long(argc, argv, short_options,
                              long_options, NULL);
    switch (next_option)
    {
    case 'h': /* -h or --help */
      /* User has requested usage information.  Print it to standard
         output, and exit with exit code zero (normal termination).  */
      print_usage();

    case 'v':
      args.isVerbose = 1;
      break;
      
    case 'o':
      args.hostName = optarg;
      break;

    case 'p':
      args.portNo = atoi(optarg);
      break;

    case 'd':
      args.duration = atoi(optarg);
      break;

    case 't':
      args.threads = atoi(optarg);
      break;

    case 'r':
      args.noOfReq = atoi(optarg);
      break;

    case '?': /* The user specified an invalid option.  */
              /* Print usage information to standard error, and exit with exit
         code one (indicating abonormal termination).  */
      if (optopt == 'd' || optopt == 'r' || optopt == 't' || optopt == 'h' || optopt == 'p')
        fprintf(stderr, "Option -%c requires an argument.\n", optopt);
      else if (isprint(optopt))
        fprintf(stderr, "Unknown option `-%c'.\n", optopt);
      else
        fprintf(stderr,
                "Unknown option character `\\x%x'.\n",
                optopt);
      print_usage();

    case -1: /* Done with options.  */
      break;

    default: /* Something else: unexpected.  */
      abort();
    }
  } while (next_option != -1);

  args.portNo = args.portNo ? args.portNo : PORT_NO;
  args.threads = args.threads ? args.threads : NUM_THREADS;

  if (!args.noOfReq && !args.duration)
  {
    args.noOfReq = NO_OF_REQUESTS;
    args.duration = DURATION;
  }
  else if (!args.duration)
  {
    args.duration = MAX_DURATION;
  }
  else if (!args.noOfReq)
  {
    args.noOfReq = MAX_NO_OF_REQUESTS;
  }

  pthread_t threads[args.threads];

  int rc;
  long t;

  // stores the current time in the startTime global variable
  args.startTime = currentTime();

  for (t = 0; t < args.threads; t++)
  {
    rc = pthread_create(&threads[t], NULL, (void *)getMethod, &args);

    if (rc != 0)
    {
      printf("ERROR; return code from pthread_create() is %d\n", rc);
      exit(EXIT_FAILURE);
    }
  }
  for (t = 0; t < args.threads; t++)
  {
    pthread_join(threads[t], NULL);
  }

  return 0;
}