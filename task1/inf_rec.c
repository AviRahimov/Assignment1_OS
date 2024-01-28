#include <stdio.h>

int inf_rec(int num){
    return inf_rec(num - 1);
}

int main(){
    inf_rec(100);
    //illegalAccess();
    return 0;
}