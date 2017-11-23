#include <stdio.h>  /* printf, sprintf */
#include <stdlib.h> /* exit */
#include <string.h> /* memcpy, memset */
#include <unistd.h> /* read, write, close */

#include <sys/types.h>
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */

#include <pthread.h>
#include "inputs.h"
#include "constants.h"
#include "utils.h"
#include "httpMethods.h"
#include "stats.h"

/**
 * pthread_mutex_t mymutex = PTHREAD_MUTEX_INITIALIZER;
 * 
 * Mutex variables must be declared with type pthread_mutex_t, and must be initialized before they can be used.
  */
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;

void getMethod(void *arguments)
{

  struct arg_struct *args = arguments;

  hostName = args->hostName;
  portNo = args->portNo;
  threadCount = args->threads;

  
  while (1)
  {
    timeTaken = currentTime() - args->startTime;
    
    /**
     * pthread_mutex_lock (mymutex)
     * 
     * @params mymutex: mutex instance to be locked.
     * pthread_mutex_lock() routine is used by a thread to acquire a lock on the specified mutex variable.
       If the mutex is already locked by another thread, this call will block the calling thread until
       the mutex is unlocked. 
     */ 
    pthread_mutex_lock(&mutex1);

    if (completeReqCount >= args->noOfReq || currentTime() >= args->startTime + args->duration)
    {
      timeTaken = currentTime() - args->startTime;
      print_stats();
    }

    /**
     * pthread_mutex_unlock (mymutex)
     * 
     * @params mymutex: mutex instance to be unlocked.
     * pthread_mutex_unlock()will unlock a mutex if called by the owning thread.
       Calling this routine is required after a thread has completed its use of protected data if other
       threads are to acquire the mutex for their work with the protected data.
     * 
     */ 
    pthread_mutex_unlock(&mutex1);

    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[BUFFER_SIZE];

    portno = args->portNo;

    // socket is created using socket system call
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
      error("ERROR opening socket");

    server = gethostbyname(args->hostName);
    if (server == NULL)
    {
      fprintf(stderr, "ERROR, no such host\n");
      exit(0);
    }

    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
          (char *)&serv_addr.sin_addr.s_addr,
          server->h_length);
    serv_addr.sin_port = htons(portno);

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
      failReqCount++;
      error("ERROR connecting");
    }

    n = write(sockfd, "GET / HTTP/1.0\r\n\r\n", strlen("GET / HTTP/1.0\r\n\r\n"));

    if (n < 0)
    {
      failReqCount++;
      error("ERROR writing to socket");
    }

    bzero(buffer, BUFFER_SIZE);

    char *tmpResult = NULL;
    long thisReadSize;
    long totalBytesRead = 0;
    while (1)
    {
      memset(buffer, 0, BUFFER_SIZE);
      thisReadSize = recv(sockfd, buffer, BUFFER_SIZE - 1, 0);

      if (thisReadSize <= 0)
        break;

      tmpResult = (char *)realloc(tmpResult, thisReadSize + totalBytesRead);

      memcpy(tmpResult + totalBytesRead, buffer, thisReadSize);
      totalBytesRead += thisReadSize;
    }

    if (totalBytesRead <= 0)
    {
      failReqCount++;
      error("ERROR reading from socket");
    }
    else
    {
      pthread_mutex_lock(&mutex2);

      if (args->isVerbose == 1)
        printf("%s\n", tmpResult);

      contentLength = totalBytesRead;
      totalContentLength += totalBytesRead;

      char statusCodeForHttp10[4];
      strncpy(statusCodeForHttp10, tmpResult + valIndex(tmpResult, "HTTP/1.0"), 3);
      statusCodeForHttp10[3] = '\0';

      char statusCodeForHttp11[4];
      strncpy(statusCodeForHttp11, tmpResult + valIndex(tmpResult, "HTTP/1.1"), 3);
      statusCodeForHttp11[3] = '\0';

      if (checkStatusCode(statusCodeForHttp10) || checkStatusCode(statusCodeForHttp11))
      {
        completeReqCount++;
      }
      else
      {
        failReqCount++;
      }

      if (completeReqCount >= args->noOfReq || currentTime() >= args->startTime + args->duration)
      {
        timeTaken = currentTime() - args->startTime;
        print_stats();
      }

      free(tmpResult);
      pthread_mutex_unlock(&mutex2);
    }

    shutdown(sockfd, SHUT_RDWR);
    close(sockfd);
  }
  /**
   * pthread_mutex_destroy(mymutex)
   * 
   * @params mymutex: mutex instance to be destroyed.
   * pthread_mutex_destroy() should be used to free a mutex object which is no longer needed. 
    */
  
  pthread_mutex_destroy(&mutex1);
  pthread_mutex_destroy(&mutex2);
}
