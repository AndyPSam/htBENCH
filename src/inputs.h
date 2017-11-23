#ifndef INPUT_H

/******* struct to pass args to various modules *******/
typedef struct arg_struct
{
  int argc;
  char **argv;
  char* hostName;
  int portNo;
  int duration;
  int threads;
  int noOfReq;
  int startTime;
  int isVerbose;
} arg_struct;
/****************************************************/

#endif // !INPUT_H