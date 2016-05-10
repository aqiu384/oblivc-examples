#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <obliv.h>
#include <time.h>

#include "sha256.h"
#include "../common/util.h"

void hextostr(char *str, const char *hex, int len) {    
    int i;
    for (i = 0; i < len; i++) {
        sprintf(str + 2*i, "%02hhx", hex[i]);
    }
    str[2*len] = '\0';
}

void strtohex(char *hex, const char *pos, int len) {
    int i;
    for(i = 0; i < len; i++) {
        sscanf(pos, "%2hhx", &hex[i]);
        pos += 2;
    }
}

void readLines(protocolIO *io, const char *filePath) {
    FILE * fp;
    
    if ((fp = fopen(filePath, "r")) == NULL) {
        fprintf(stderr, "FILE %s not found.\n", filePath);
        exit(1);
    }

    int i;
    char * line = NULL;
    size_t read, len = 0;

    for (i = 0; i < MAXL && (read = getline(&line, &len, fp)) != -1; i++) {
        io->lens[i] = (read < MAXN ? read : MAXN);
        strncpy(io->lines[i], line, io->lens[i]);
        io->lines[i][io->lens[i]] = 0;
    }
    
    io->n = i;
    
    if (line) {
        free(line);
    }
    
    fclose(fp);
}

int main(int argc, char *argv[]) {
    ProtocolDesc pd;
    protocolIO io;
    int i, index = 0;
    char output[2*SHA256_BLOCK_SIZE+1];

    if(argc < 3) {
        fprintf(stderr, "Not enough args\n");
        return 1;
    }

    int party = (argv[1][0] == '1' ? 1 : 2);
    readLines(&io, argv[2]);
    
    protocolUseStdio(&pd);
    setCurrentParty(&pd, party);
    
    double lap = wallClock();
    execYaoProtocol(&pd, sha256, &io);
    hextostr(output, io.res, SHA256_BLOCK_SIZE);
    
    fprintf(stderr,"Party: %d\n", party);
    fprintf(stderr,"Total time: %lf s\n", wallClock() - lap);
    fprintf(stderr,"Result: %s\n\n", output);

    cleanupProtocol(&pd);
    return 0;
}
