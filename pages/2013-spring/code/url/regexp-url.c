/*
 * 01-26-2013 from 
 *http://stackoverflow.com/questions/1085083/regular-expressions-in-c-examples
 *
 * Revised so it can recognize a URL in a stream of characters (a text)
 * This example examines one match (first one) only. If a pice of text
 * contains multiple matches, one has to extract them all.
 */

#include <stdio.h>
#include <stdlib.h>
#include <regex.h>        
#include <string.h>

int main(int argc, char*argv[]) {

  regex_t regex;
  regmatch_t matched[8];
  int nmatch = 1;
  int len;
  int reti;
  char msgbuf[128];
  char * url_pattern = "<a\\s*href\\s*=\\s*\"[a-z:/.0-9?#]*\"";

  /* the following are testing urls */
  //  char * url = "<a   href =        \"http://cnn.com:90//testpages/grading.html\">anchor text</a>";
  //  char * url = "<a href = \"http://cnn.com/level0/./testpages/../level1/lelve2/../../grading.html\">anchor</a>";
  //  char * url = "<a  href =   \"../testpages/level2/../level3/grading.html\">anchor </a>";
  // char * url = "<a href  = \"../testpages/level2/../level3/.././grading.html#abc\">anchor</a>"; 
    char * url = "some other text <a  href =    \"grading.html?abc\">anchore </a> after"; 


  /* Compile regular expression */
  reti = regcomp(&regex, url_pattern, REG_ICASE);
  if( reti ){ fprintf(stderr, "Could not compile regex\n"); exit(1); }

  /* Execute regular expression */
  reti = regexec(&regex, url, nmatch, matched, 0);
  if( !reti )  {
    puts("Match");
    len = matched[0].rm_eo - matched[0].rm_so;
    printf("b=%d e=%d\n", matched[0].rm_so, matched[0].rm_eo);
    strncpy(msgbuf, &(url[matched[0].rm_so]), len);
    msgbuf[len] = 0;
    printf("|%s|\n", msgbuf);
  }  else if( reti == REG_NOMATCH )  {
    puts("No match");
  }  else  {
    regerror(reti, &regex, msgbuf, sizeof(msgbuf));
    fprintf(stderr, "Regex match failed: %s\n", msgbuf);
    exit(1);
  }

  /* Free compiled regular expression if you want to use the regex_t again */
  regfree(&regex);
}
