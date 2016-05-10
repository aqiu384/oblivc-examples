#define MAXN 100
#define SCALE (1 << 16)

typedef struct {
    int rows;
    int cols;
    int src[MAXN];
    int n;
    int resLen;
    float res[MAXN];
} protocolIO;

void linearRegression(void* args);
