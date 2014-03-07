#ifndef _schedule_ec_h_
#define _schedule_ec_h_

int addProcessEC(int tid, int priority);
int removeProcessEC(int tid);
int nextProcessEC();
void printAll();
struct node * getSubgroup(struct node *head);
#endif
