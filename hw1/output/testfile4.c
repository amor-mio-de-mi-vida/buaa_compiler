int fuunc(){
    return 3;
}
int main() {
    int a = 3;
    int i;
    for (i = 0; i < 3; i = i + 1) {
        printf("no%d: (a mod 2 + 6) * 4 - 25 is %d\n", i+1, (a%2+6)*4-25);
    }

    for (i = 0; i < 3; i = i + 1) {
        const int a = 3 + 4 * 5 - 24;
        if ( a + i > 0 ) {
            printf("no%d: a is %d\n", i + 4, a);
        } else {
            printf("no%d, i is %d\n", i + 4, i);
        }
    }

    
    for (i = 0; i < 4; i = i + 1) {
        ;
        {
            printf("no%d: (a - 4) * 2 mod (i+1) is %d\n", i + 7, (a-4)*2 % (i+1) + i);
        }
    }
    return 0;
}