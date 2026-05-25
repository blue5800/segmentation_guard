#include <stdio.h>
#include <stdlib.h>
#include "segmentation_guard.h"

int main() {
    enable_segmentation_guard();
    //this one returns safely but it doesn't know where to go so just exits.
    void (*abyss)() = (void(*)())0xDEADBEEF;
    abyss();

    printf("[!] something wrong\n");

    return 0;
}

