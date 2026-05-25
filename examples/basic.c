#include <stdio.h>
#include <stdlib.h>
#include "segmentation_guard.h"

int main() {
    enable_segmentation_guard();

    volatile int *bad_ptr = (int*)0xDEADBEEF;
    
    *bad_ptr = 42; 

    printf("[*] Total segfaults caught: %d\n", number_of_segfaults());

    return 0;
}
