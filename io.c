#include "io.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static long get_file_size(FILE *file)
{
    long offset = ftell(file);
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, offset, SEEK_SET);
    return size;
}

struct io_buffer *io_buffer_create(size_t length)
{
    struct io_buffer *buffer = malloc(sizeof(*buffer));
    buffer->data = malloc(length);
    buffer->length = length;
    return buffer;
}

void io_buffer_destory(struct io_buffer *buffer)
{
    free(buffer->data);
    free(buffer);
}

struct io_buffer *io_readfile(const char *file_path)
{
    FILE *file = fopen(file_path, "rb");
    if (!file)
        return NULL;
    
    long file_size = get_file_size(file);
    
    struct io_buffer *buffer = io_buffer_create(file_size);
    
    size_t nitems = fread(buffer->data, file_size, 1, file);
    if (nitems != 1) {
        io_buffer_destory(buffer);
        buffer = NULL;
    }
    
    fclose(file);
    return buffer;
}

int io_path_exist(const char *path)
{
    return access(path, F_OK) == 0;
}
