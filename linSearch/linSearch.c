#include <stdio.h>
#include <obliv.h>
#include <time.h>

#include "linSearch.h"
#include "../common/util.h"

void loadList(protocolIO *io) {
    char *SOURCE = "./intLists/intList.txt";
    FILE *inputFile = fopen(SOURCE, "r");
    
    if (inputFile == NULL) {
        fprintf(stderr, "FILE '%s' not found.\n", SOURCE);
        exit(1);
    }
    
    io->nList = malloc(sizeof(int) * MAXN);
    
    int i, n;
    for (i = 0; i < MAXN && fscanf(inputFile, "%d", &n) == 1; i++) {
        io->nList[i] = n;
    }
        
    io->n = i;
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
    protocolUseStdio(&pd);
    
    if (party == 1) {
        loadList(&io);
    }
    else {
        io.n = strtol(argv[2], NULL, 10);
    }
    
    setCurrentParty(&pd, party);

    double lap = wallClock();
    execYaoProtocol(&pd, linSearch, &io);

    fprintf(stderr,"Party: %d\n", party);
    fprintf(stderr,"Total time: %lf s\n", wallClock() - lap);
    
    if (io.res) {
        fprintf(stderr,"Item found.\n\n");
    }
    else {
        fprintf(stderr,"Item not found.\n\n");
    }

    cleanupProtocol(&pd);
    
    if (party == 1) {
        free(io.nList);
    }
    
    return 0;
}
