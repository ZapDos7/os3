#ifndef __HELP_H__
#define __HELP_H__
#include <openssl/md5.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>

// Print the MD5 sum as hex-digits.
void print_md5_sum(unsigned char* md);

// Get the size of the file by its file descriptor
unsigned long get_size_by_fd(int fd);
#endif