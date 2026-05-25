#include <stdio.h>
#include <stdlib.h>
#include "segmentation_guard.h"

int main() {
    enable_segmentation_guard();

    // if $RSP is corrupt then where can it even put the stack frame? 
    __asm__ volatile (
        "mov $0xDEADBEEF, %rsp\n\t"
    );

    printf("[!] something wrong\n");

    return 0;
}
