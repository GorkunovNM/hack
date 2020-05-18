#ifndef $FCOMMANDER$H
#define $FCOMMANDER$H

#include <stdio.h>
#include <sys/stat.h>
#include <cstdlib>

//====================FILE  FUNCTIONS====================//
size_t file_len(const char *file_name);
//=======================================================//

//=====================CYCLIC SHIFTS=====================//
size_t left_cycl_shft_m_to_l(size_t num);

size_t left_cycl_shft (size_t num, size_t sh_cnt);

size_t right_cycl_shft(size_t num, size_t sh_cnt);
//=======================================================//

size_t file_len(const char *file_name) {
	struct stat st;
	stat(file_name, &st);
	return st.st_size;
}

size_t left_cycl_shft_m_to_l(size_t num) {
	size_t itt = 0;
	size_t bit = num >> (sizeof(size_t) * 8 - 1);
	while (bit != 1 && itt < sizeof(size_t) * 8 - 1) {
		++itt;
		num = (num << 1) + bit;
		bit = num >> (sizeof(size_t) * 8 - 1);
	}
	num = (num << 1) + bit;
	return num;
}

size_t left_cycl_shft(size_t num, size_t sh_cnt) {
	for (size_t itt = 0; itt < sh_cnt; ++itt) {
		num = (num << 1) + (num >> (sizeof(size_t) * 8 - 1));
	}
	return num;
}

size_t right_cycl_shft(size_t num, size_t sh_cnt) {
	for (size_t itt = 0; itt < sh_cnt; ++itt) {
		num = (num >> 1) + (num << (sizeof(size_t) * 8 - 1));
	}
	return num;
}

#endif