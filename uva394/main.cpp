#include <iostream>
#include <string.h>
using namespace std;

struct dimension {
    int lowerBound;
    int upperBound;
} __attribute__((__packed__));

struct arrayDeclaration {
    char name[11];
    int baseAddress;
    int elementSize;
    int dimensionSize;
    struct dimension d[11];
    int constants[11];
} __attribute__((__packed__));

void calculateConstants(struct arrayDeclaration *db) {
    int dimensionSize = db->dimensionSize;
    int elementSize = db->elementSize;

    db->constants[dimensionSize] = elementSize;

    for (int i = dimensionSize - 1; i > 0; i--) {
        int nextConstant = db->constants[i + 1];
        int upperBound = db->d[i + 1].upperBound;
        int lowerBound = db->d[i + 1].lowerBound;
        db->constants[i] = nextConstant * (upperBound - lowerBound + 1);
    }

    db->constants[0] = db->baseAddress;

    for (int i = 1; i <= dimensionSize; i++) {
        int lowerBound = db->d[i].lowerBound;
        db->constants[0] -= db->constants[i] * lowerBound;
    }

    /*
    printf("constants: ");
    for (int i = 0; i <= dimensionSize; i++) {
        printf("%d ", db->constants[i]);
    }
    printf("\n");
    */
};

int searchByName(struct arrayDeclaration db[], char *name) {
    int N = 9999;

    for (int i = 0; i < N; i++) {
        if (strcmp(db[i].name, name) == 0) {
            return i;
        }
    }

    return N;
};

void calulatePhysicalAddress(struct arrayDeclaration *db, int indexes[]) {
    int dimensionSize = db->dimensionSize;

    printf("%s[", db->name);
    for (int i = 1; i <= dimensionSize; i++) {
        if (i == dimensionSize) {
            printf("%d] = ", indexes[i]);
        } else {
            printf("%d, ", indexes[i]);
        }
    }

    int address = db->constants[0];
    for (int i = 1; i <= dimensionSize; i++) {
        address += db->constants[i] * indexes[i];
        // printf("\ndb->constants[%d]=%d indexes[%d]=%d\n", i, db->constants[i], i, indexes[i]);
    }

    printf("%d\n", address);
};

int main(int argc, const char *argv[])
{
    char line[2048];
    int N, R, ln;

    // Get N and R
    memset(line, 0, 2048);
    gets(line);
    sscanf(line, "%d %d", &N, &R);

    // printf("N=|%d| R=|%d|\n", N, R);

    // Line number
    ln = 0;

    // Initialize arrayDeclaration
    struct arrayDeclaration db[N];

    memset(line, 0, 2048);
    while (gets(line)) {
        // printf("line=|%s|\n", line);
        // printf("ln=%d\n", ln);
        int byteConsumed = 0;

        if (ln < N) {
            // Initialize db
            memset(&db[ln], 0, sizeof(struct arrayDeclaration));

            // Read array declaration
            sscanf(
                line,
                "%s %d %d %d %n",
                db[ln].name,
                &db[ln].baseAddress,
                &db[ln].elementSize,
                &db[ln].dimensionSize,
                &byteConsumed
            );

            // Read dimension
            for (int d = 1; d <= db[ln].dimensionSize; d++) {
                int tmp;
                sscanf(
                    line + byteConsumed,
                    "%d %d %n",
                    &db[ln].d[d].lowerBound,
                    &db[ln].d[d].upperBound,
                    &tmp
                );

                byteConsumed += tmp;

                /*
                printf(
                    "d=|%d| lb=|%d| ub=|%d|\n",
                    d,
                    db[ln].d[d].lowerBound,
                    db[ln].d[d].upperBound
                );
                */
            }

            /*
            printf(
                "name=|%s| size=|%d| dimension=|%d| byteRead=|%d| substring=|%s|\n",
                db[ln].name,
                db[ln].elementSize,
                db[ln].dimensionSize,
                byteConsumed,
                line + byteConsumed
            );
            */

            // Calculate constants
            calculateConstants(&db[ln]);
        } else {
            // Reading array reference
            char name[11];
            sscanf(line, "%s %n", name, &byteConsumed);

            int index = searchByName(db, name);
            int dimensionSize = db[index].dimensionSize;
            int indexes[dimensionSize + 1];

            for (int i = 0; i <= dimensionSize; i++) {
                int tmp;
                sscanf(
                    line + byteConsumed,
                    "%d %n",
                    indexes + i + 1,
                    &tmp
                );

                byteConsumed += tmp;
            }

            calulatePhysicalAddress(&db[index], indexes);
        }

        // Increase line number
        ln++;

        // Reset line
        memset(line, 0, 2048);
    };

    return 0;
}
