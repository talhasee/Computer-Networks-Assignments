#include<stdio.h>
#include<string.h>

int main(){
    char a[] = "PK";
    char data[5][1024];
    strcpy(data[0], a);
    strcpy(data[1], "TD");
    // data[1][0] = "Pk";
    // char *p = "PK", *t = "TD";
    // sprintf(data, "%sx%sx%sx%s", p, t, p, t);
    printf("%s\n", data[0]);
    return 0;
}