#include <stdio.h>

int main(int argc, char const *argv[])
{
    // This pointer is not initialized and will point to the NULL address (0x0) which is for sure not a valid address
    int *p = NULL;
    // This will cause a segmentation fault
    *p = 100;
    return 0;
}
