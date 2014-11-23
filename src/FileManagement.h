#ifndef __FILE_MANAGEMENT__
#define __FILE_MANAGEMENT__

#include <stdint.h>
#include <string.h>

typedef struct {
    int fd;
    void * mapped;
} FileImpl;

FileImpl*
open_file_read(const char* name);

void
close_file(const FileImpl* file);

void *
read_file(const FileImpl* file, uint32_t offset, size_t len);

#endif