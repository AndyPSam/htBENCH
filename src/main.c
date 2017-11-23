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

  /**
   * pthread_t is an data type which acts as a handle for new threads.
    */
  pthread_t threads[args.threads];

  int rc;
  long t;

  // stores the current time in the startTime global variable
  args.startTime = currentTime();


  printf("Running htBENCH, Version 1.0\n\nPlease be patient while we benchmark http://%s:%d/...\n(Press Ctrl + C at any time to stop benchmarking)\n", args.hostName, args.portNo);

  for (t = 0; t < args.threads; t++)
  {
    /**
     * int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
                          void *(*start_routine) (void *), void *arg);
     * 
     * The  pthread_create()  function  starts  a  new thread in the calling process.  The new thread starts execution by
       invoking start_routine(); arg is passed as the sole argument of start_routine().
     * 
     * @params {{pthread_t *}} thread: An opaque, unique identifier for the new thread returned by the subroutine.
     * @params {{const pthread_attr_t *}} attr: An opaque attribute object that may be used to set thread attributes. You can specify a thread attributes object, or NULL for the default values.
     * @params {{void *}} start_routine: the C routine that the thread will execute once it is created.
     * @params {{void *}} arg: A single argument that may be passed to start_routine. It must be passed by reference as a pointer cast of type void. NULL may be used if no argument is to be passed. 
     * @returns {{int}} return code to check for failure.
      */
    rc = pthread_create(&threads[t], NULL, (void *)getMethod, &args);

    if (rc != 0)
    {
      printf("ERROR; return code from pthread_create() is %d\n", rc);
      exit(EXIT_FAILURE);
    }
  }

  int jrc;

  /**
   * int pthread_join(pthread_t thread, void **retval);
   * 
   * The  pthread_join()  function  waits  for the thread specified by thread to terminate.
   * If that thread has already terminated, then pthread_join() returns immediately.
   * 
   * @params {{pthread_t}} thread: thread suspended until the thread identified by thread terminates, either by calling pthread_exit() or by being cancelled.
   * @params {{void **}} retval: If retval is not NULL, the return value of thread is stored in the location pointed to by retval.
   * @returns {{int}} return code to check for failure.
    */
  for (t = 0; t < args.threads; t++)
  {
    pthread_join(threads[t], NULL);
    if (jrc != 0)
    {
      printf("ERROR; return code from pthread_join() is %d\n", jrc);
      exit(EXIT_FAILURE);
    }
  }

  return 0;
}