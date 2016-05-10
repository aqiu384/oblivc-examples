#define SHA256_BLOCK_SIZE 32
#define MAXN 100
#define MAXL 100

typedef struct protocolIO { 
    int n;
    int lens[MAXL];
    char lines[MAXL][MAXN+1];
    char res[SHA256_BLOCK_SIZE];
} protocolIO;

void sha256(void* args);
