#include "FileManagement.h"

#include <stdio.h>
#include <stdlib.h>

#include <fcntl.h>
#include <unistd.h>

#include <sys/mman.h>


FileImpl*
open_file_read(const char* name)
{
    int fd = open(name, O_RDONLY);
    if (fd < 0) return NULL;
    void* mapped = mmap(NULL, 1024*1024, PROT_READ, MAP_PRIVATE, fd, 0);
    if (mapped == MAP_FAILED) return NULL;

    FileImpl* file = (FileImpl*) malloc(sizeof(FileImpl));
    file->fd = fd;
    file->mapped = mapped;
    return file;
}

void
close_file(const FileImpl* file)
{
    munmap(file->mapped, 1024*1024);
    close(file->fd);
    free((void*)file);
}
void *
read_file(const FileImpl* file, uint32_t offset, size_t len)
{
    return (file->mapped + offset);
}