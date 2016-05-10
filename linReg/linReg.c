#include <stdio.h>
#include <obliv.h>
#include <time.h>

#include "linReg.h"
#include "../common/util.h"

void readMat2d(protocolIO *io, int src) {
    char *SOURCE1 = "./testMats/mat1.txt";
    char *SOURCE2 = "./testMats/mat2.txt";
    char *source = (src == 1 ? SOURCE1 : SOURCE2);
    FILE *inputFile = fopen(source, "r");
    
    if (inputFile == NULL) {
        fprintf(stderr, "FILE '%s' not found.\n", src);
        exit(1);
    }
    
    int rows, cols;
    if (fscanf(inputFile, "%d %d", &rows, &cols) != 2) {
        fprintf(stderr, "Need matrix dimensions from FILE '%s.\n'");
        exit(1);
    }
    
    io->rows = rows;
    io->cols = cols;
    
    float a;
    int r, c, n = 0;
    for (r = 0; r < rows; r++) {
        for (c = 0; c < cols; c++) {
            if (fscanf(inputFile, "%f", &a) != 1) {
                fprintf(stderr, "Not enough values for a %d-by-%d matrix in FILE '%s'.\n", rows, cols, src);
                exit(1);
            }
            io->src[n++] = (int) (a * SCALE);
        }
    }
    
    fclose(inputFile);
}

int main(int argc, char *argv[]) {    
    ProtocolDesc pd;
    protocolIO io;
    int i, index = 0;

    if(argc < 3) {
        fprintf(stderr, "Not enough args\n");
        return 1;
    }

    int party = (argv[1][0] == '1' ? 1 : 2);
    char swapChar = (party == 1 ? 'C' : 'B');
    protocolUseStdio(&pd);
    
    readMat2d(&io, atoi(argv[2]));
    setCurrentParty(&pd, party);

    double lap = wallClock();
    execYaoProtocol(&pd, linearRegression, &io);

    fprintf(stderr,"Party: %d\n", party);
    fprintf(stderr,"Total time: %lf s\n", wallClock() - lap);
    fprintf(stderr,"Results:\n");
    
    for (i = 0; i < io.resLen; i++) {
        fprintf(stderr,"%09.4f\n", io.res[i]);
    }
    
    fprintf(stderr,"End Results:\n\n");

    cleanupProtocol(&pd);
    return 0;
}
