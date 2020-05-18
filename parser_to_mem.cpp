#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main() {
    FILE * in = fopen("texture.png", "rb");
    if (in == NULL) {
        printf("Input file not found\n");
        return 1;
    }
    FILE * out = fopen("tmp.h", "wt");
    if (out == NULL) {
        printf("Can not create output\n");
        return 1;
    }
    fprintf(out, "const unsigned char SOUND_BUF_MEM[SOUND_BUF_MEM_SIZE] = {\n");
    int copy = 0;
    for(int i = 0, c = fgetc(in); c != EOF; c = fgetc(in), ++i) {
        if (i % 16 == 0) fprintf(out, "    ");
        fprintf(out, "0x%02X,", c);
        if (i % 16 == 15) fputs("\n", out);
        copy = i;
    }
    fprintf(out, "0xFF };\n\n");
    fprintf(out, "%d\n", copy);
    fclose(in);
    fclose(out);
}