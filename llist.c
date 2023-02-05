#include <stdio.h>
#include <stdlib.h>
#include "list.h"


typedef struct node {
    ElemType val;
    struct node *next;
    struct node *prev;
} NODE;


struct list_struct {
    NODE *front;
    NODE *back;
};


/*
* returns pointer to newly created empty list
*/
LIST *lst_create() {
LIST *l = malloc(sizeof(LIST));

  l->front = NULL;
  l->back = NULL;
  return l;
}

void lst_free(LIST *l) {
NODE *p = l->front;
NODE *pnext;

  while(p != NULL) {
    pnext = p->next;   // keeps us from de-referencing a freed ptr
    free(p);
    p = pnext;
  }
  // now free the LIST 
  free(l);
}

void lst_print(LIST *l) {
NODE *p = l->front;

  printf("[");
  while(p != NULL) {
    printf(FORMAT, p->val);
    p = p->next;
  }
  printf("]\n");
}


void lst_push_front(LIST *l, ElemType val) {
NODE *p = malloc(sizeof(NODE));

  p->val = val;
  if(l->front == NULL){
  	p->next = NULL;
  }
  if(l->front != NULL){
  	p->next = l->front;
  	l->front->prev = p;
  }
  p->prev = NULL;
  l->front = p;
  if(l->back == NULL)   // was empty, now one elem
  	l->back = p;
}

void lst_push_back(LIST *l, ElemType val) {
  NODE *p;

  if(l->back == NULL)   // list empty - same as push_front
    lst_push_front(l, val);
  else {  // at least one element before push
    p = malloc(sizeof(NODE));
    p->val = val;
    p->next = NULL;
    p->prev = l->back;
    l->back->next = p;
    l->back = p;  
  }
}

int lst_length(LIST *l) {
NODE *p = l->front;
int n=0;

  while(p != NULL) {
    n++;
    p = p->next;
  }
  return n;
}

int lst_is_empty(LIST *l) {
  return l->front == NULL;
}






ElemType lst_pop_front(LIST *l) {
ElemType ret;
NODE *p;
 

  if(lst_is_empty(l))
	return DEFAULT;   // no-op

  ret = l->front->val;
  
  if(l->front == l->back) {  // one element
	free(l->front);
	l->front = NULL;
	l->back = NULL;
  }
  else {
	p = l->front;  // don't lose node being deleted
	l->front = l->front->next;// hop over
	l->front->prev = NULL;
	free(p);
  }
  return ret;
}

     
  
