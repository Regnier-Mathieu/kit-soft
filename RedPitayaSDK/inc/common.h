#ifndef COMMON_H
#define COMMON_H


/*** Global Libraries ***/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>

/*** Local Libraries ***/
#include "rp.h"
#include "configuration.h"

/*** Global Variables ***/
extern int stop;
char *data_to_send;

#endif
