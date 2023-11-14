#include <iostream>
#include <fstream>
#include <string>
#include "lexer.h"
#include "parser.h"
using namespace std;

int rowNo = 1;  // 当前解析的行号
int colNo = 0;  // 当前解析的列号
bool debug = true;
bool comment = false; // 处理可换行的注释
bool lexer = false; // 词法分析debug开关
bool parser = false; // 语法分析debug开关
bool error = true;
ifstream fin; // 输入文件的文件描述符
ofstream fout; // 输出文件的文件描述符
ofstream ferr;

int main() {
    const char* fileInPath = "./testfile.txt";
    const char* fileOutPath = "./output.txt";
    const char* fileErrorPath = "./error.txt";
    fin.open(fileInPath, ios::in);
    fout.open(fileOutPath, ios::out);
    ferr.open(fileErrorPath, ios::out);
    if (!fin.is_open())
        printf("can't open the file!\n");

    std::string strLine;
    while(getline(fin, strLine)) {
        // printf("get new line! rowNo is %d and colNo is %d\n", rowNo, colNo);
        if (strLine.empty()) {
            colNo = 0;
            rowNo++;
            continue;
        }

        if (!comment) {
            getTokens(strLine, strLine.length());
        } else {
            size_t r = strLine.find("*/");
            if (r != string::npos) {
                comment = false;
                colNo = (int) (r + 2); // 小心溢出
                getTokens(strLine, strLine.length());
            }
        }
        // cout << strLine << endl;
        colNo = 0;
        rowNo++;
    }

    printTokens();  // 词法分析debug

    parseCompUnit();

    fin.close();
    fout.close();
    ferr.close();
    return 0;
}
