#include <stdio.h>
// 常量声明
const int a = 0;
const int a3[1] = {1};
const int a2[2][2] = {{3,3},{1,4}};

// 变量声明（不带初值）
int b;
int b5[3], iqwe;
int b35[2][3], qweq, qweoijq[3];
// 变量声明 （带初值）
int sadjl = 93 + 4;
int asjdk=54, asda[3] = {0,1,2};
int oiw[2][2] = {{3,3}};
int asdasd[][2] = {{1,2}};
int a343[] = {1,3,4};
const int dasd = (((4 + 32) - 6)*2 / 3 ) % 2;

int getint() {
    int n;
    scanf("%d",&n);
    return n;
}

void _func1() {
    const int osad = dasd + a; 
    printf("no1: 21371476\n");
    return;
}

void func3(int a, int b[2], int c[2][2], int d[][2]) {
    int i;
    int ds = 0;
    const int asd = 9;
    for (i = 3; i < asd && !ds; i = i + 1) {
        b[i] = i * 7 % asd;
        printf("no%d: b[i] = %d, a3 is %d\n",i-1, b[i], a3[0]);
        if (0 && b[i] >= 100) {
            break;
        }
    }
    a = b[2] + c[2][1];
    d[1][1] = a + c[1][1];
    return;
}

int func2(int a) {
    const int a1213 = 0;
const int a312312[1] = {1};
const int a21313[2][2] = {{3,3},{1,4}};

// 变量声明（不带初值）
int b;
int b5[3], iqwe;
int b35[2][3], qweq, qweoijq[3];
// 变量声明 （带初值）
int sadjl = 93 + 4;
int asjdk=54, asda[3] = {0,1,2};
int oiw[2][2] = {{3,3}};
int asdasd[][2] = {{1,2}};
int a343[] = {1,3,4};
const int dasd = (((4 + 32) - 6)*2 / 3 ) % 2;
    int adas[2][2]={{1,2},{3,4}};
    int asdasdsa[2][2]={{adas[0][0],adas[0][1]},{adas[1][0],adas[1][1]}};
    int c;
    {
        ;
    }
    ;
    {

    }
    c = getint();
    // int c = getint();
    int sad = c;
    printf("no9: this is a test %d\n", c + 2 * a / a2[0][0]);
    printf("no10: %d + %d = %d\n", a + sad, a - sad, 50);
    return a + c;
}
int func4(int asdaslj) {
    return 32;
}

int main() {
    _func1();
    int i,j = 4;
    int pos = 3;
    int sadpjds = ((a2[0][0] + a3[0]) * func4(pos) / 3 ) % 2;
    // for无缺省
    for(i = 0; i < 83; i = i + 1) {
        // if 有 else
        if (i < 3 && pos < 8 && !(j == 4)) {
            pos = pos + 3;
        } else {
            // 有break
            break;
        }
    }
    func3(a, asda, asdasd,oiw);
    // for缺省第二个ForStmt 
    for (j = 2 * 3;pos != 10 && a == 0;) {
        // if 无 else
        if (pos > 10 || a != 0) {
            // 有continue
            continue;
        } 
        pos = pos + 1;
    }
    printf("no8: pos is pos%d\n", a2[1][1]);
    // for缺省第一个ForStmt
    for (; j < 20; j = j + 1) {
        pos = pos - 2;
    }
    func2(a);

    // for缺省Cond
    for (pos = 4;;pos = pos * 2) {
        if (1 || pos > 39) {
            break;
        }
    }
    pos = 0;
    for (;;) {
        if (pos == 4) {
            break;
        }
        pos = pos + 1;
    }

    for (pos = 1;;) {
        pos += pos;
        if (pos <= 16) {
            break;
        }
    }

    for (;; pos+=+3) {
        if (pos > 20) {
            break;
        }
    }
    pos = 3;
    for (;pos != 2;) {
        if (pos != 2 && i != 10) {
            pos = 2;
        } else {
            pos = 2;
            i = 10;
            break;
        }
    }

    if (pos == 3 && pos < 4 || pos >= 9 && pos != 23 || pos < 243) {
        ;
    } else {
        if (pos + 31 <= 293 && pos - 2 > 2) {
            {

            }
        }
    }
    return 0;
}