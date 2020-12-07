#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <ctype.h>
#include <stdint.h>
#include <math.h>
#include <stdbool.h>

#include "funciones.h"

double resta_timeval(struct timeval*t1, struct timeval *t0){
return (t1->tv_sec-t0->tv_sec)+(t1->tv_usec-t0->tv_usec)/1000000.0;
}


double espera_siguiente_intervalo( struct timeval *t0, int n, double dt ){

	double t_es;
	struct timeval ta;
	struct timespec ts;

	gettimeofday(&ta,NULL);
	
	t_es = (double)n*dt+(t0->tv_sec+t0->tv_usec*1e-6)-(ta.tv_sec+ta.tv_usec*1e-6);
	
	if(t_es>0.0){
		ts.tv_sec = (int)t_es;
		ts.tv_nsec = (t_es-ts.tv_sec)*1e9;
		nanosleep(&ts,NULL);	
	}	
     return t_es;
} 

