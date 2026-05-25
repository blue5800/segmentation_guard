#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "segmentation_guard.h"


void* fault_trigger(void* arg) {
    (void)arg;
    for(int i = 0; i < 10000; i++) {
        volatile int *bad = (int*)0xDEADBEEF;
        *bad = 0; 
    }
    return NULL;
}

int main() {
    enable_segmentation_guard();
    
    // for a single thread, this would be catchable.
    // this relies on global state therefore 
    // concurrent sigsegv corrupt the global state and cause illegal instruction/memory access. 
    pthread_t t[4];
    for(int i=0; i<4; i++) {
        pthread_create(&t[i], NULL, fault_trigger, NULL);
    }

    for(int i=0; i<4; i++) {
        pthread_join(t[i], NULL);
    }
    
    printf("[?] lucky.\n");
    return 0;
}
