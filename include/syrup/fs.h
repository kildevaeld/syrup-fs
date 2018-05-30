#pragma once
#include <stdbool.h>
#include <stddef.h>
#include <sys/stat.h>

#ifdef __cplusplus
extern "C" {
#endif

bool sy_file_exists(const char *filename);
bool sy_file_is_dir(const char *filename);
bool sy_file_is_regfile(const char *filename);

int sy_file_size(const char *filename);
char *sy_read_file(const char *filename, char *buffer, int maxlen, int *len);
int sy_write_file(const char *filename, const char *buffer, size_t len);

int sy_mkdir_p(const char *path, mode_t mode);

#ifdef __cplusplus
}
#endif