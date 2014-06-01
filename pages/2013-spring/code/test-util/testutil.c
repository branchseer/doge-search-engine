#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "list.h"
#include "stringutil.h"
#include "htmllinks.h"
#include "url.h"
#include "http.h"

void testlist() {
  struct List *myList = (struct List *)malloc(sizeof(struct List));
  struct List *secondList = (struct List *)malloc(sizeof(struct List));
  char * value[] = {"hello", "world", "how", "are", "you" };
  char * var[]   = {"a", "b", "c", "d", "e"};
  int i;

  list_init(myList);
  list_init(secondList);

  printf("testing list ...\n");
  for (i = 0; i < 5; i ++) {
    printf("pais v[%d] = %s\n", i, value[i]);
  }

  for (i = 0; i < 5; i ++) {
    printf("working with %d\n", i);
    struct Node *anode = list_makeNode(var[i], value[i], -1);
    struct Node *cnode = list_copyNode(anode);
    list_add(myList, anode);
    list_add(secondList, cnode);
  }

  for (i = 0; i < 5; i ++) {
    printf("get [%d] = %s\n", i, list_get(*myList, var[i]));
  }

  struct Node * bnode = list_makeNode("b", "world", -1);
  printf("list should contain (world/b), the result is %d\n", list_contains(*myList, *bnode));
  bnode->num = 9999;
  printf("list should not contain bnode, the result is %d\n", list_contains(*myList, *bnode));

  printf("the first node should be the same in both, the result is %d\n", list_isNodeSame(*(myList->head), *(secondList->head)));

  list_addList(myList, secondList);
  printf("the list that combines the two ...\n");
  while (list_isEmpty(*myList) == false) {
    struct Node *anode = list_remove(myList);
    printf("node value : (%s), (%s), (%d)\n", anode->var, anode->val, anode->num);
  }
}

void testlist2D() {

  struct List *myList = (struct List *)malloc(sizeof(struct List));
  struct List *secondList = (struct List *)malloc(sizeof(struct List));

  char * value[] = {"hello", "world", "how", "are", "you" };
  char * var[]   = {"a", "b", "c", "d", "e"};
  int i;

  list_init(myList);
  list_init(secondList);

  printf("testing list2D ...\n");
  for (i = 0; i < 5; i ++) {
    printf("working with %d\n", i);
    struct Node *anode = list_makeNode(var[i], value[i], -1);
    struct Node *cnode = list_copyNode(anode);
    list_add(myList, anode);
    list_add(secondList, cnode);
  }

  printf("make second list a part of the first list ...\n");
  struct Node * work = myList->head;
  while (work != NULL) {
    work->links = list_copyList(secondList);
    work = work->next;
  }

  printf("the result of connecting the 2D list ...\n");
  work = myList->head;
  while (work != NULL) {
    printf("work->var %s ----- \n", work->var);
    struct List * alist = work->links;
    list_traverse(*alist);
    work = work->next;
  }
}

void teststrings() {

  char* words = "Hello    \n Other cases! World, 123how , are you?";
  struct List *l = string_split( words );
  list_traverse(*l);
  printf("original string[%s]\n", words);
  printf("converted string[%s]\n", lowerString(words));

  printf("testing findStrLoc()...\n");
  printf("findStrLoc('Other') should return 11, the result is %d\n", findStrLoc(words, "Other", 0));
  printf("findStrLoc('NNN') should return -1, the result is %d\n", findStrLoc(words, "NNN", 0));

  printf("test removeExtraSpace ...\n");
  //  char * line = "he \n wo    test";
  printf("%s\n", removeExtraSpace(words));
  
  printf("test str_trim() ...\n");
  printf("++%s++\n", str_trim("   +abc hello world!    "));

  char *a = "helloworld.html";
  char *b = "hi.exe";
  char *c = "world.php";
  char *d = "no extension";

  printf("end with html (true) %d\n", str_endWith(a, ".html"));
  printf("end with exe (true) %d\n", str_endWith(b, ".exe"));
  printf("end with php (true) %d\n", str_endWith(c, ".php"));
  printf("end with html (false) %d\n", str_endWith(d, ".html"));

  char *e = "http://some//moreslash/";
  char *f = "http://normal.com/index.html";
  char *g = "..//hello.world/a//////b././";
  char *h = "nothing/domain/normal";
  printf("original |%s| removed |%s|\n",e, removeExtraSlash(e));
  printf("original |%s| removed |%s|\n",f, removeExtraSlash(f));
  printf("original |%s| removed |%s|\n",g, removeExtraSlash(g));
  printf("original |%s| removed |%s|\n",h, removeExtraSlash(h));
}

void testhtml() {

  char* page = "<html><body>\
some text <a href  = \"http://www.eg.bucknell.edu/testpages/grading.html\">anchor text</a> more text</body></html>";

  printf(" original page...\n");
  printf("%s\n", page);
  struct List urls;
  list_init(&urls);
  extractURLs( page, "http://www.bucknell.edu/", &urls);
  
  char* page2 = "<html><body>\
some text <a href  = \"http://cnn.com//testpages/grading.html#abc\">anchor text</a> more text  <a href =     \"../hello.html\">second </a> new text</body></html>";

  printf(" original page...\n");
  printf("%s\n", page2);
  extractURLs( page2, "http://www.bucknell.edu/", &urls);
  list_traverse(urls);
  
}

void testurl() {

  char * baseUrl = "http://www.bucknell.edu/home/dir/level3/file.html";
  //  char * url = "http://cnn.com:90//testpages/grading.html";
  //  char * url = "http://cnn.com:80//testpages/grading.html";
  //  char * url = "http://cnn.com/level0/./testpages/../level1/lelve2/../../grading.html";
  //  char * url = "../testpages/level2/../level3/grading.html";
  //  char * url = "../testpages/level2/../level3/.././grading.html#abc"; 
  //  char * url = "../grading.html#abc"; 
  //  char * url = "grading.html#abc"; 
  //  char * url = "/grading.html#abc"; 
  char * url = "../testpages/level1/level2/../level3/grading.html"; 
  char * r = url_fixURL( url, baseUrl);
  printf("original url: ++%s++\n", url);
  printf("base url :  ++%s++\n", baseUrl);
  printf("fixed url : ++%s++\nstring len %d\n", r, strlen(r));
  //  printf("fixed url : ++%s++\n", url_fixURL( url, "http://www.bucknell.edu/"));
}

void testpage() {
  //  int f = open("test-page.html", O_RDONLY);
  //  int f = open("bu-page.html", O_RDONLY);
  int f = open("xmeng-teaching.html", O_RDONLY);
  //  int f = open("cnn-out.html", O_RDONLY);
  int max = 65536;
  //  int max = 131072;
  char* buf = (char*)malloc(max);
  char * aurl = (char*)malloc(MAXSTRLEN);
  int n = read(f, buf, max);
  struct List urls;
  struct List normURLs;
  list_init(&urls);
  list_init(&normURLs);
  buf[n] = 0;
  //  strcpy(buf, "<a href=\"/video/?hpt=hp_t1#/video/us/2012/12/20/tsr-foreman-fiscal-cliff-explainer.cnn\">");
  //  strcpy(buf, "<a href=\"/video/?hpt=hp_t1#/video/\">");
  //  strcpy(buf, "<a href=\"http://money.cnn.com/2012/12/30/technology/netflix-hastings-pay/index.html?source=cnn_bin\">Netflix doubles CEO Hastings' pay </a>");
  extractURLs( buf, "http://www.bucknell.edu/", &urls);
  list_traverse(urls);
  struct Node *work;
  work = urls.head;
  while (work != NULL) {
    strcpy(aurl, work->val);
    //    aurl = url_fixURL(aurl, "http://www.bucknell.edu/");
    printf("%s\n", aurl);
    struct Node *anode = list_makeNode("", aurl, -1);
    list_add(&normURLs, anode);
    work = work->next;
  }
  printf("a total of %d urls found.\n", list_len(normURLs));
}

void testextracturl() {

  struct URL *result;
  //  char *aurl = "http://www.cnn.com:567/";
  //  char *aurl = "http://www.cnn.com:567/l1/l2/l3/file.name";
  char *aurl = "http://www.cnn.com:567/l1/l2/../l3/./file.name";

  result = url_extractURL(aurl);
  printf("prot |%s|\n", result->thisProt);
  printf("host |%s|\n", result->thisHost);
  printf("path |%s|\n", result->thisPath);
  printf("file |%s|\n", result->thisFile);
  printf("port |%d|\n", result->thisPort);
}

void testhttp() {
  int f = open("bu-home.html", O_RDONLY);
  int max = 65536;
  char* buf = (char*)malloc(max);
  int n = read(f, buf, max);
  buf[n] = 0;
  struct HttpHead * response = http_retrieveHead( buf, "www.bucknell.edu" );
  if (response != NULL) {
    printf("response : %d\n", response->code);
    printf("host : %s\n", response->host);
    printf("date : %s\n", response->date);
  }
}

int main(int argc, char*argv[]) {

  testlist();
  testlist2D();
  teststrings();
  testhtml();
  testurl();
  testpage();
  testextracturl();
  testhttp();
  return 0;
}
