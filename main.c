#include <stdio.h>
#include <string.h>
#include <errno.h>

/* (x*x+x)/2 */

char buf[1000][1000];
int prettyprint, display_code;

int getmax(int);
void space(int);
void readtri(FILE*,int);

extern void triangular(void);

int main(int argc, char **argv)
{
    if (argc < 2)
        return fprintf(stderr,"Usage: triangular source.tri\n");

    if (argc > 2) {
        int x;
        for (x = 2; x < argc; x++) {
            if (!strcmp(argv[x],"--pretty") || !strcmp(argv[x],"--prettyprint"))
                prettyprint = 1;
            else if (!strcmp(argv[x],"--display-code") || !strcmp(argv[x],"--debug") || !strcmp(argv[x],"--step"))
                display_code = 1;
        }
    }

    //if (argc > 2 && !strcmp(argv[2],"--verbose"))
    //    verbose = 1;
    //if (argc > 3 && !strcmp(argv[3],"--display-code"))
    //    display_code = 1;

    FILE *in = fopen(argv[1],"r");

    if (!in)
        return fprintf(stderr,"Error: Could not open file %s: %s\n",argv[1],strerror(errno));

    int size = 0, c, i;
    while ((c = getc(in)) != EOF)
        if (c != ' ' && c != '\n' && c != '\r')
            size++;
    rewind(in);

    int len = getmax(size) - 1;
    int l_cpy = len;

    if (prettyprint)
    {
        int j;
        for (i = 1; l_cpy; l_cpy--, i++) {
            space(l_cpy);
            j = i;
            while (j) {
                c = getc(in);
                if (c != ' ' && c != '\n' && c != '\r') {
                    printf("%c ",c);
                    j--;
                }
            }
            puts("");
        }
        puts("");
    }

    rewind(in);
    readtri(in,len);

    triangular();
}

void space(int s)
{
    while (s--)
        putchar(' ');
}

int getmax(int length)
{
    int i = 0, res = 0;
    while (1) {
        if (res >= length)
            return i;
        res += i++;
    }
}

void readtri(FILE *in, int len)
{
    int c, size, vals, x, y, changed;
    c = x = y = vals = 0, changed;

    for (size = 1; size <= len; size++) {
        changed = 0;
        vals = size;
        while (vals) {
            int c = getc(in);
            if (c != ' ' && c != '\n' && c != '\r') {
                changed = 1;
                buf[y][x++] = c;
                vals--;
            }
        }
        if (changed)
            y++;
        x = changed = 0;
    }
}
