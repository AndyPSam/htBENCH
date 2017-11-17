#ifndef UTILS_H

// Method that calls perror method and print out the error
void error(const char *msg);

// Method that returns current time in seconds
int currentTime();

/* 
 * @params {{char *}} text  -- the text to be searched
 * @params {{char *}} pattern -- the pattern to be sought
 * @returns {{int}} index of the found pattern value
*/
int valIndex(char *text, char *pattern);

#endif // !UTILS_H

