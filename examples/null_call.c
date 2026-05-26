#include <stdio.h>
#include <stdlib.h>
#include "segmentation_guard.h"

void outrageous_jump(){
    printf("[*] Outrageous jump called! jumping to 0xDEADBEEF\n");
    __asm__ __volatile__(
        "mov $0xdeadbeef, %rax\n"
        "jmp *%rax\n"
    );
}

void null_func_caller() {
    printf("[*] Calling NULL from null_func_caller...\n");
    void (*null_func)() = NULL;
    null_func();
}

int main() {
    segmentation_guard_init();
    enable_segmentation_guard();
    int x = 30;
    null_func_caller();
    printf("[*] x = %d\n", x);
    printf("[*] Calling NULL...\n");
    void (*null_func)() = NULL;
    null_func();
    x = 5;
    printf("[*] Total segfaults caught: %d\n", number_of_segfaults());
    printf("x = %d\n", x);
    
    outrageous_jump();
    printf("[*] bad jumps can't be recovered from. only bad calls. so this never actually gets printed.\n");

    return 0;
}
