#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdint.h>
#include "segmentation_guard.h"

int main() {
    enable_segmentation_guard();

    int page_size = getpagesize();
    
    uint8_t *mem = mmap(NULL, page_size * 2, PROT_READ | PROT_WRITE | PROT_EXEC, 
                        MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    
    if (mem == MAP_FAILED) {
        perror("mmap");
        return 1;
    }

    if (mprotect(mem + page_size, page_size, PROT_NONE) == -1) {
        perror("mprotect");
        return 1;
    }


    uint8_t *boundary = mem + page_size - 1;
    //first byte of a two byte nop.
    boundary[0] = 0x0F;
    //note: it doesn't crash with the single byte instruction, trivially.
    //boundary[0] = 0x90;
    printf("[*] Pages setup. Page 1: RWX, Page 2: NONE.\n");

    void (*func)() = (void(*)())boundary;
    func();

    printf("[!] something wrong\n");
    return 0;
}
