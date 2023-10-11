#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define TRUE 0.0
#define FALSE -0.0
#define not(a) -a
#define or(a, b) (a - not(b))
#define and(a, b) not(or(a, b))
#define xor(a, b) or(and(not(a), b), and(a, not(b)))
#define get(a, b) ((a>>(b))&1)
#define toBool(a) ((a) ? TRUE : FALSE)
int eval(double a){
    return ((unsigned char*)&a)[7] == 0;
}
int special(int a){
    double out1 = TRUE;
    double out2 = FALSE;
    for(int i=0;i<32;i+=2){
        out1 = and(out1, xor(out1, toBool(get(a, i))));
        out2 = and(out2, xor(out2, toBool(get(a, i+1))));
    }
    return eval(and(out1, out2));
}
char xorc(char a, char b){
    char outs = 0;
    for(int i=0;i<8;i++){
        outs |= eval(xor(toBool(get(a, i)),toBool(get(b, i)))) << i;
    }
    return outs;
}
int main(){
    clock_t start = clock();
    int j=1;
    int pn=0;
    while(pn<1000000){
        j++;
        if(special(j)){
            pn++;
        }
    }
    double cpu_time_used = ((double) (clock() - start)) / CLOCKS_PER_SEC;
    printf("special %d, #%d found in %f seconds\nkey: ", j, pn, cpu_time_used);
    for(int i=0;i<4;i++) printf("\\x%02x", ((unsigned char*)&j)[i]);
    char key[4] = {0};
    memcpy(key, &j, 4);
    printf("\n");
    char text[] = "lorem ipsum dolor sit amet";
    char* txt = calloc(100, 1);
    strcpy(txt, text);
    for(int i=0;i<strlen(txt);i++) txt[i] = xorc(txt[i], key[i%4]);
    printf("%s\n", txt);
    for(int i=0;i<strlen(txt);i++) txt[i] = xorc(txt[i], key[i%4]);
    printf("%s\n", txt);
    free(txt);
}
