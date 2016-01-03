#include <iostream>
#include <string.h>
using namespace std;

int main(int argc, const char *argv[])
{
    char line[2048];
    int isStart = 1;

    while (gets(line)) {
        char *c;

        for (int i = 0; line[i]; i++) {
            c = line + i;

            if (*c == '"') {
                if (isStart == 1) {
                    isStart = 0;
                    printf("``");
                } else {
                    isStart = 1;
                    printf("''");
                }
            } else {
                printf("%c", *c);
            }
        }

        printf("\n");

        // Reset variable
        memset(line, 0, 2048);
    };

    return 0;
}
