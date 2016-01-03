#include <iostream>
#include <string.h>
using namespace std;
// 900000 999999

int getCycleLength(int seed) {
    // Initial value
    unsigned int ret = 1;

    while (seed != 1) {
        ret++;

        if (seed % 2 == 0) {
            // even
            seed = seed / 2;
        } else {
            // odd
            seed = seed * 3 + 1;
        }
    }

    return ret;
}

int main(int argc, const char *argv[])
{
    char line[1000];
    unsigned int from, to, max, start, end;

    while (gets(line)) {
        sscanf(line, "%d %d", &start, &end);

        from = start < end ? start : end;
        to = start < end ? end : start;

        for (int i = from; i <= to; i++) {
            unsigned int length = getCycleLength(i);
            max = length > max ? length : max;
        }

        printf("%d %d %d\n", start, end, max);

        // Reset variable
        max = 0;
        memset(line, 0, 1000);
    };

    return 0;
}
