//#include <stdlib.h>
//#include <stdio.h>
//#include <string.h>
//#include <unistd.h>
//#include <time.h>
//#include <fcntl.h>
//#include <ctype.h>
//#include <stdint.h>

#include <iostream>
#include <chrono>
#include <functional>
#include <thread>

#include <sys/time.h>




double resta_timeval(struct timeval *t1, struct timeval *t0);
double espera_siguiente_intervalo( struct timeval *t0, int n, double dt ); 



