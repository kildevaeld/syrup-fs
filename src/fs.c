#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syrup/fs.h>
#include <unistd.h>

bool sy_file_exists(const char *filename) {
  struct stat buffer;
  return (stat(filename, &buffer) == 0) ? true : false;
}

static bool check_path(const char *filename, int t) {
  struct stat buffer;
  if (stat(filename, &buffer) < 0)
    return false;
  return (buffer.st_mode & S_IFMT) == t;
}

bool sy_file_is_dir(const char *filename) {
  return check_path(filename, S_IFDIR);
}

bool sy_file_is_regfile(const char *filename) {
  return check_path(filename, S_IFREG);
}

int sy_file_size(const char *filename) {
  int string_size = -1;
  FILE *handler = fopen(filename, "r");

  if (handler) {
    // Seek the last byte of the file
    fseek(handler, 0, SEEK_END);
    // Offset from the first to the last byte, or in other words, filesize
    string_size = ftell(handler);
    // Always remember to close the file.
    fclose(handler);
  }

  return string_size;
}
char *sy_read_file(const char *filename, char *buffer, int maxlen, int *len) {
  // buffer = NULL;
  int c = 0;
  int string_size, read_size;
  FILE *handler = fopen(filename, "r");

  if (!handler) {
    return NULL;
  }

  // Seek the last byte of the file
  fseek(handler, 0, SEEK_END);
  // Offset from the first to the last byte, or in other words, filesize
  string_size = ftell(handler);
  // go back to the start of the file
  rewind(handler);

  if (buffer == NULL) {
    // Allocate a string that can hold it all
    buffer = (char *)malloc(sizeof(char) * (string_size));
    c = 1;
  } else if (string_size > maxlen) {
    return NULL;
  }

  // Read it all in one operation
  read_size = fread(buffer, sizeof(char), string_size, handler);

  // fread doesn't set it so put a \0 in the last position
  // and buffer is now officially a string
  // buffer[string_size] = '\0';

  if (string_size != read_size) {
    // Something went wrong, throw away the memory and set
    // the buffer to NULL
    if (c)
      free(buffer);
    buffer = NULL;
  }

  // Always remember to close the file.
  fclose(handler);

  if (len != NULL)
    *len = string_size;

  return buffer;
}

int sy_write_file(const char *filename, const char *buffer, size_t len) {

  FILE *file = fopen(filename, "wb");
  if (file == NULL) {
    return 0;
  }

  int ret = fwrite(buffer, sizeof(char), len, file);
  fclose(file);

  return ret;
}

int sy_mkdir_p(const char *path, mode_t mod) {
  /* Adapted from http://stackoverflow.com/a/2336245/119527 */
  const size_t len = strlen(path);
  char _path[PATH_MAX];
  char *p;

  errno = 0;

  /* Copy string so its mutable */
  if (len > sizeof(_path) - 1) {
    errno = ENAMETOOLONG;
    return -1;
  }
  strcpy(_path, path);

  /* Iterate the string */
  for (p = _path + 1; *p; p++) {
    if (*p == '/') {
      /* Temporarily truncate */
      *p = '\0';

      if (mkdir(_path, mod) != 0) {
        if (errno != EEXIST)
          return -1;
      }

      *p = '/';
    }
  }

  if (mkdir(_path, mod) != 0) {
    if (errno != EEXIST)
      return -1;
  }

  return 0;
}

// delimiter

#ifdef _WIN32
#define WHICH_DELIMITER ";"
#else
#define WHICH_DELIMITER ":"
#endif

/*
 * Lookup executable `name` within the PATH environment variable.
 */

char *sy_lookup(const char *name) {
  return sy_lookup_path(name, getenv("PATH"));
}

/*
 * Lookup executable `name` within `path`.
 */

char *sy_lookup_path(const char *name, const char *_path) {
  char *path = strdup(_path);
  if (NULL == path)
    return NULL;
  char *tok = strtok(path, WHICH_DELIMITER);

  while (tok) {
    // path
    int len = strlen(tok) + 2 + strlen(name);
    char *file = malloc(len);
    if (!file) {
      free(path);
      return NULL;
    }
    sprintf(file, "%s/%s", tok, name);

    // executable
    if (0 == access(file, X_OK)) {
      free(path);
      return file;
    }

    // next token
    tok = strtok(NULL, WHICH_DELIMITER);
    free(file);
  }

  free(path);

  return NULL;
}
