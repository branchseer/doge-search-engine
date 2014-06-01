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
 */

#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "list.h"   /* list is used to store the resulting urls */
#include "stringutil.h"   /* string functions */
#include "htmllinks.h"
#include "url.h"

struct List * extractURLs( char* pageSrc, char* baseUrl, 
			   struct List * urlList ) {
  /*
   * Input:
   * char * pageSrc:  original Html page
   * char * baseUrl:  base URL of the page
   *
   * Output: a list of normalized urls.
   */

  //  printf("before remove space |%s|\n", pageSrc);
  pageSrc = removeExtraSpace(pageSrc);   /* remove all space */
  //  printf("after remove space |%s|\n", pageSrc);
  int  len = strlen(pageSrc);   /* length of the page */
  char ch;                      /* current char */
  int  i;                       /* loop index */
  int  state = 0;               /* state of the finite state machine */
  char *sb = (char*) malloc(len + 1);  /* string buffer, len < len(page) */
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
	if (c < MAXSTRLEN-1)  // skip the extreme long urls
	  analyze(sb, urlList, baseUrl);
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
	if (c < MAXSTRLEN-1)  // skip the extreme long urls
	  analyze(sb, urlList, baseUrl);
	//	fprintf(stderr, "in extractor - %s\n", sb);
	c = 0;       /* reset the string buffer */
      } else if (!isspace(ch)) {
	sb[c] = ch;
	c ++;
      }
      break;
    }   // end of 'switch'
  }  // end of 'for'

  if (c > 0 && c < MAXSTRLEN - 1)  { 
    // this is try to catch the run-away <a href ...
    sb[c] = 0;
    analyze(sb, urlList, baseUrl);
  }
}

/** Analyzes "param", which should be the contents between a '<' and a '>',
 *  and adds any URLs that are found to the list of URLs.
 */
void analyze(char* param, struct List * urls, char* base) {
  //  param = lowerString( param );
  struct List * st = string_split( param );
  int llen = list_len(*st);
  int plen = strlen(param);
  struct Node *current;
  
  if (llen < 2) return;  /* nothing to do */
  current = st->head;
  char* first_word = (char*)malloc(plen + 1); 
  char* rest_words = (char*)malloc(plen + 1); 
  strcpy(first_word, current->var);

  if (first_word[0] == 'a' || first_word[0] == 'A') {
    char * work = lowerString( param );
    int rest = findStrLoc(work, "href", 0);
    if (rest != -1) {
      strcpy(rest_words, &(param[rest]));
      analyzeAnchor(rest_words, urls, base);
    }
  } 
}

/** Analyzes the <a> tag.  */
void analyzeAnchor(char* anchor, struct List * urls, char* base) {
  char* href = extract(anchor, "href");
  if (href == NULL) return;
  //  printf("url(before fix): %s\n", href);
  href = fixUrl(href);
  href = url_fixURL( href, base);
  //  printf("url(after fix): %s\n", href);
  struct URL *url = url_extractURL( href );  // make a node out of a url
  struct Node *anode = url_makeNode(*url);    // convert a url node to "Node"
  list_add(urls, anode);
}

/** Returns the value in "line" associated with "key", or null if "key"
 *  is not found.  For instance, if line were "a href="blah blah blah"
 *  and "key" were "href" this method would return "blah blah blah".
 *  <p>
 *  Keys are case insensitive.
 */
char* extract(char* line, char* key) {

  char* lower_case = lowerString(line);
  int line_len = strlen(line);
  char* retcpy = (char*)malloc(line_len + 1);

  key = lowerString(key);

  int i = findStrLoc(lower_case, key, 0);
  if (i < 0) return NULL;
  i += strlen(key);
  if (line[i] != '=') return NULL;
  i++;
  int i2;
  if (line[i] == '"') {
    i++;
    i2 = findloc(line, '"', i);
    if (i2 < 0) {
      return &(line[i]);
    } else {
      strncpy(retcpy, &(line[i]), (i2-i));
      retcpy[i2-i] = 0;   /* terminate the string */
      //      printf("return copy |%s| value of i2-i = %d\n", retcpy, (i2-i));
      return retcpy;
    }
  } else {
    i2 = i;
    for (; i2 < line_len; i2++) {
      if (isspace(line[i2])) break;
    }
    strncpy(retcpy, &(line[i]), (i2-i));
    retcpy[i2-i] = 0;   /* terminate the string */
    //    printf("return copy(else) |%s|\n", retcpy);
    return retcpy;
  }
  return NULL;
}

// this is an add-hoc fix, only works for "href"
char* fixUrl(char* inUrl)  {

  inUrl = removeExtraSlash(inUrl);
  return inUrl;
}

