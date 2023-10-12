
int func(int a[2], int b) {
    printf("no1: 21371476\n");
    printf("no2: a[0] is %d and a[1] is %d, b is %d\n",a[0], a[1], b);
    return 9;
}

void func2(int a1, int a2, int a3, int a4) {
    printf("no6: qwe - weq is %d\n", a2 - a3);
    printf("no7: wqe is %d\n", a4);
}

void func4() {
    printf("no10: it's over!\n");
}

void func3(int b[2], int a[2], int adas, int adj321) {
    int i = 0;
    for (i = 0; i < 2; i = i + 1) {
        if (b[i]!= 0|| 9 == func(b,i) && func(b, i) < 10) {
            int ad = (a[i] * adas - adj321) % b[i];
            printf("no%d: ad is %d\n", i + 8, ad);
            int asdjaksd = func(b, i);
        } else {
            printf("no%d: b[i] is zero", i + 8);
        }
    }
}

int func5() {
    return 4;
}

int main() {
    int a, b[2], c[3][2] = {{1,2}, {9,4}, {2,6}};
    a = getint();
    int i;
    for (i = 0; i < 2; i= i + 1) {
        b[i] = getint();
    }
    func(c[1], a);
    int asd = c[0][0];
    int qwe = +-+c[0][1];
    int weq = -c[2][1];
    int wqe = 9 * func5();
    int adj321 = asd + qwe - weq;
    i = 3;
    int adas = (asd * weq / (i + 1)) % 2;
    int qweq = (asd + 3) % 5;
    b[0] = (qwe + qweq * weq) % 3;
    b[1] = a / 4 + 2 * qwe;
    if (b[0] == 32) {
        printf("wqe is %d\n", wqe);
    } else {
        printf("no3: b[0] is %d and b[1] is %d\n", b[0], b[1]);
    }
    if (wqe > 10) {
        printf("no4: asd is %d, qwe is %d, weq is %d, wqe is %d\n",asd, qwe, weq, wqe);
    }
    printf("no5: b[1] is %d, b[0] is %d\n",b[1], b[0]);
    func2(asd, qwe, weq + 3, wqe);
    func3(b, c[1], adas, adj321);
    func4();

    return 0;
}