#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "sq.h"

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

typedef struct service_queue {
	LIST *theQ;
	LIST *buzz;
	int mapSize;
	int length;
	NODE **map;
} SQ;

SQ * sq_create() {
	SQ *q;
	q = malloc(sizeof(SQ));
	q->theQ = lst_create();
	q->buzz = lst_create();
	q->length = 0;
	q->mapSize = 16;
	q->map = (NODE **) malloc(sizeof(NODE *) * q->mapSize);
	if(q->map == NULL){//if map was not allocated correctly
		exit(1);
	}
	for(int i = 0; i < q->mapSize; i++){
                q->map[i] = NULL;
        }
	return q;
}

void sq_free(SQ *q){
	lst_free(q->theQ);
	lst_free(q->buzz);
	free(q->map);
	free(q);
}

void sq_display(SQ *q){
	printf("current-queue contents:\n    ");
	lst_print(q->theQ);
	printf("\n");
}

int sq_length(SQ *q){
	return q->length;
}

int sq_give_buzzer(SQ *q){
	int buzzer;
	if(!lst_is_empty(q->buzz)){
		buzzer = lst_pop_front(q->buzz);
		lst_push_back(q->theQ, buzzer);
		q->length++;
		if(q->length >= q->mapSize){// DYNAMICALLY REALLOCATING THE MAP
			NODE **newMap = (NODE **) malloc(sizeof(NODE *) * q->mapSize * 2);
			int copyCounter = 0;
			int copyBackCounter = 0;
			while(copyCounter < q->length){
				newMap[copyCounter] = q->map[copyCounter];
				copyCounter++;
			}
			free(q->map);
			q->mapSize *= 2;
			q->map = (NODE **) malloc(sizeof(NODE *) * q->mapSize);
			for(int i = 0; i < q->mapSize; i++){
                		q->map[i] = NULL;
        		}
			while(copyBackCounter < q->length){
				q->map[copyBackCounter] = newMap[copyBackCounter];
				copyBackCounter++;
			}
			
			free(newMap);
		}
		q->map[buzzer] = q->theQ->back;

		return buzzer;
	}
	else{
		buzzer = sq_length(q);
		lst_push_back(q->theQ, buzzer);
		q->length++;
		if(q->length >= q->mapSize){// DYNAMICALLY REALLOCATES THE MAP
                        NODE **newMap = (NODE **) malloc(sizeof(NODE *) * q->mapSize * 2);
                        int copyCounter = 0;
                        int copyBackCounter = 0;
                        while(copyCounter < q->length){
                                newMap[copyCounter] = q->map[copyCounter];
                                copyCounter++;
                        }
                        free(q->map);
                        q->mapSize *= 2;
                        q->map = (NODE **) malloc(sizeof(NODE *) * q->mapSize);
                        for(int i = 0; i < q->mapSize; i++){
                		q->map[i] = NULL;
        		}
			while(copyBackCounter < q->length){
                                q->map[copyBackCounter] = newMap[copyBackCounter];
                                copyBackCounter++;
                        }
                        free(newMap);
                }
		q->map[buzzer] = q->theQ->back;
		return buzzer;
	}
}
int sq_seat(SQ *q){
	int buzzer;
	if(lst_is_empty(q->theQ)){
		return -1;
	}
	else{
		buzzer = lst_pop_front(q->theQ);
		lst_push_front(q->buzz, buzzer);
		q->map[buzzer] = NULL;
		q->length--;
		return buzzer;
	}
}

int sq_kick_out(SQ *q, int buzzer){
	if(buzzer < 0) return 0;
	if(q->map[buzzer] == NULL){
		return 0;
	}
	if((q->map[buzzer])->prev == NULL){
		sq_seat(q);
		return 1;
	}
	if((q->map[buzzer])->next == NULL){
		NODE *p = q->theQ->back;
		lst_push_front(q->buzz, buzzer);
		q->theQ->back = p->prev;
		p->prev->next = NULL;
		p->prev = NULL;
		q->map[buzzer] = NULL;
		q->length--;
		free(p);
		return 1;
	}
	NODE *p = q->map[buzzer];
	p->prev->next = p->next;
	p->next->prev = p->prev;
	p->prev = NULL;
	p->next = NULL;
	lst_push_front(q->buzz, buzzer);
	q->map[buzzer] = NULL;
	q->length--;
	free(p);
	return 1;
}

int sq_take_bribe(SQ *q, int buzzer){
	if(buzzer < 0) return 0;
	if(lst_is_empty(q->theQ)){
		return 0;
	}
	if(q->map[buzzer] == NULL){
		return 0;
	}
	if((q->map[buzzer])->prev == NULL){
		return 1;
	}
	if((q->map[buzzer])->next == NULL){
		NODE *p = q->theQ->back;
		q->theQ->back = p->prev;
		p->prev->next = NULL;
		p->prev = NULL;
		p->next = q->theQ->front;
		q->theQ->front->prev = p;
		q->theQ->front = p;
		q->map[buzzer] = q->theQ->front;
		return 1;
	}
	NODE *p = q->map[buzzer];
	p->prev->next = p->next;
	p->next->prev = p->prev;
	p->next = q->theQ->front;
	q->theQ->front->prev = p;
	p->prev = NULL;
	q->theQ->front = p;
	q->map[buzzer] = q->theQ->front;
	return 1;


}
