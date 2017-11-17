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

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;

void getMethod(void *arguments)
{

  struct arg_struct *args = arguments;
  struct resultStats resStat;

  while (1)
  {
    pthread_mutex_lock(&mutex1);

    if (args->completeReqCount >= args->noOfReq || currentTime() >= args->startTime + args->duration)
    {
      resStat.completeReqCount = args->completeReqCount;
      resStat.hostName = args->hostName;
      resStat.portNo = args->portNo;
      resStat.failReqCount = args->failReqCount;
      resStat.threadCount = args->threads;
      resStat.timeTaken = currentTime() - args->startTime;
      print_stats(&resStat);
    }

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
      args->failReqCount++;
      error("ERROR connecting");
    }

    n = write(sockfd, "GET /\r\n", strlen("GET /\r\n"));

    if (n < 0)
    {
      args->failReqCount++;
      error("ERROR writing to socket");
    }

    bzero(buffer, BUFFER_SIZE);

    n = read(sockfd, buffer, BUFFER_SIZE - 1);

    if (n < 0)
    {
      args->failReqCount++;
      error("ERROR reading from socket");
    }
    else
    {
      pthread_mutex_lock(&mutex2);

      if (args->isVerbose == 1)
        printf("%s\n", buffer);

      char contentLength[4];
      strncpy(contentLength, buffer + valIndex(buffer, "Content-Length:"), 3);
      contentLength[3] = '\0';
      resStat.contentLength = contentLength;

      char statusCode[4];
      strncpy(statusCode, buffer + valIndex(buffer, "HTTP/1.0"), 3);
      statusCode[3] = '\0';

      if (checkStatusCode(statusCode))
      {
        args->completeReqCount++;
      }
      else
      {
        args->failReqCount++;
      }

      // printf("\n--\nCompleted Request Count: %d\n--\n", args->completeReqCount);
      // printf("\n--\nFailed Request Count: %d\n--\n", args->failReqCount);

      if (args->completeReqCount >= args->noOfReq || currentTime() >= args->startTime + args->duration)
      {
        resStat.completeReqCount = args->completeReqCount;
        resStat.hostName = args->hostName;
        resStat.portNo = portno;
        resStat.failReqCount = args->failReqCount;
        resStat.threadCount = args->threads;
        resStat.timeTaken = currentTime() - args->startTime;
        print_stats(&resStat);
      }
      pthread_mutex_unlock(&mutex2);
    }

    shutdown(sockfd, SHUT_RDWR);
    close(sockfd);
  }
}
