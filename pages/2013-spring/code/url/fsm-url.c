/** 
 *  Originally Written by Tim Macinta 1997 for their     <br>
 *  book "Guide to Search Engines" in Java.              <br>
 *
 *  Distributed under the GNU Public License
 *       (a copy of which is enclosed with the source).   <br>
 *                                                        <br> 
 *  This LinkExtractor can extract URLs from HTML files.  <br>
 *
 *  modified by Xiannong Meng to fix the finite state machine
 *  to recognize urls containing white spaces.            <br>
 *  April 2005                                            <p>
 *
 *  modified by Xiannong Meng to convert the program into C.<br>
 *  December 2012                                         <p>
 *
 *  modified by Xiannong Meng to remove extra functionality
 *  so we can see the bare-minimum of the function.<br>
 *  January 2013.<p>
 */

/*
 * A simple finite-state-machine to recognize a url
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAXSTRLEN 256

char * analyze(char* param);

void extractURLs( char* pageSrc, char*retval[], int *count) {
  /*
   * Input:
   * char * pageSrc:  original Html page
   *
   * Output: 
   * char * extracted url
   */

  int  len = strlen(pageSrc);   /* length of the page */
  char ch;                      /* current char */
  int  i;                       /* loop index */
  int  state = 0;               /* state of the finite state machine */
  char *sb = (char*) malloc(len + 1);  /* string buffer, len < len(page) */
  char *out = (char*) malloc(len + 1);  /* string buffer, len < len(page) */
  int  c = 0;

  for (i = 0; i < len; i ++) {
    ch = pageSrc[i];
    switch (state) {
    case 0:
      if (ch == '<') state = '<';
      break;
    case '<':
      if (ch == '>') {
	state = 0;
	sb[c] = 0;    /* terminate the string buffer */
	if (c < MAXSTRLEN-1)  { // skip the extreme long urls
	  out = analyze(sb);
	  strcpy((retval[*count]), out);
	  (*count)++;
	}
	c = 0;   /* reset the string buffer */
      } else if (ch == 'a' || ch == 'A') {
	state = 'a';
	sb[c] = ch;
	c ++;
      }
      break;
    case 'a':
      if (isspace(ch)) {
	state = '+';
	sb[c] = ch;
	c ++;
      }
      break;
    case '+':
      if (!isspace(ch))	{
	  state = '-';
	  sb[c] = ch;
	  c ++;
	}
      break;
    case '-':
      if (ch == '>')  {
	state = 0;
	sb[c] = 0;   /* terminate the string */
	if (c < MAXSTRLEN-1)  { // skip the extreme long urls
	  out = analyze(sb);
	  strcpy((retval[*count]), out);
	  (*count)++;
	}
	c = 0;       /* reset the string buffer */
      } else if (!isspace(ch)) {
	sb[c] = ch;
	c ++;
      }
      break;
    }   // end of 'switch'
  }  // end of 'for'
}

/*
 * Analyzes "param", which should be the contents between a '<' and a '>',
 *  and adds any URLs that are found to the list of URLs.
 */
char * analyze(char* param) {

  char *start, *end;
  int i;
  int len = strlen(param);
  char *out = (char*)malloc(len + 1);
  /* 
   * the 'param' should be inthe form of 'a href="a url" 
   * we need to extract the 'a url'
   */

  printf("in analyze src : %s\n", param);
  for (i = 0; i < len; i ++)
    param[i] = tolower(param[i]);
  start = strstr(param, "\"");
  end = strstr((start+1), "\"");
  strncpy(out, start, (end-start+1));
  out[(end-start+1)] = 0;
  printf("in analyze ret : %s\n", out);
  return out;
}

int main(int argc, char*argv[]) {

  int  count = 5;
  int  i;
  char *msgbuf[count];
  for (i = 0; i < count; i ++) {
    msgbuf[i] = (char*)malloc(MAXSTRLEN + 1);
  }

  /* the following are testing urls */
  char * url = "<a   href =        \"http://cnn.com:90//testpages/grading.html\">anchor text</a>";
  //  char * url = "<a href = \"http://cnn.com/level0/./testpages/../level1/lelve2/../../grading.html\">anchor</a>";
  //  char * url = "<a  href =   \"../testpages/level2/../level3/grading.html\">anchor </a>";
  // char * url = "<a href  = \"../testpages/level2/../level3/.././grading.html#abc\">anchor</a>"; 
  //  char * url = "some other text <a  href =    \"grading.html?abc\">anchore </a> after <a href = \"http://some.com/file\">ancore </a>"; 

    count = 0;
    extractURLs( url, msgbuf, &count );
    for (i = 0; i < count; i ++) {
      printf("extracted url |%s|\n", msgbuf[i]);
    }
}
