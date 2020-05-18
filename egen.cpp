#include <stdio.h>
#include <ctype.h>

const int HASH_4BIT_SIZE = 16;

int main() {
	char from_edb[] = "0000000000000603";
	char expected[] = "0000000000000000";
	for (size_t i = 0; i < HASH_4BIT_SIZE / 2; ++i) {
		expected[HASH_4BIT_SIZE - 1 - 2 * i] = from_edb[2 * i + 1];
		expected[HASH_4BIT_SIZE - 2 - 2 * i] = from_edb[2 * i];
	}
	FILE *egen = fopen("egen.txt", "w");
	//fputc(0xa, egen);
	//fputc(0xd, egen);
	//char pass[] = "qsqwqsqvy{\"&&\"\"\'K";
	//fputs(pass, egen);
	//fputc(0xa, egen);
	for (size_t i = 0; i < /*0x32*/0x15; ++i) fputc(0, egen);
	fputc(0xa5, egen);
	for (size_t i = 0; i < HASH_4BIT_SIZE / 2; ++i) fputc((expected[2 * i] - '0' - (isalpha(expected[2 * i]) ? 'a' - '9' - 1 : 0)) * 16 + expected[2 * i + 1] - '0' - (isalpha(expected[2 * i + 1]) ? 'a' - '9' - 1 : 0), egen);
	//for (size_t i = 0; i < 666; ++i) fputc(0x0, egen);
	//fputc(0xa, egen);
	//fputc(0xd, egen);
	fclose(egen);
	return 0;
}