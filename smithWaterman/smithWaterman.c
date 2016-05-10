#include <stdio.h>
#include <obliv.h>
#include <time.h>

#include "smithWaterman.h"
#include "../common/util.h"

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

    io.s = argv[2];
    io.n = strlen(argv[2]);
    io.res = malloc(sizeof(int) * MAXN);

    if(io.n > MAXN) {
        fprintf(stderr, "String too big\n");
        return 1;
    }

    setCurrentParty(&pd, party);

    double lap = wallClock();
    execYaoProtocol(&pd, smithWaterman, &io);
    
    for (i = 0; i < strlen(io.res); i++) {
        io.res[i] = (io.res[i] == swapChar) ? '-' : io.s[index++];
    }

    fprintf(stderr,"Party: %d\n", party);
    fprintf(stderr,"Total time: %lf s\n", wallClock() - lap);
    fprintf(stderr,"Result: %s\n\n", io.res);

    cleanupProtocol(&pd);
    return 0;
}
