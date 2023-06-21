#ifndef HTTP_HEAD_GET_REQUEST_H
#define HTTP_HEAD_GET_REQUEST_H

#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h> 

#define MAX_STRING_LEN 2048
#define MAX_ENTRIES 2048

typedef struct {
    char filename[MAX_STRING_LEN];
    char redirect_url[MAX_STRING_LEN];
    char type[MAX_STRING_LEN];
} RedirectEntry;

void handle_http_head_get_request(int sock_fd, const char* root_dir, const char* url, const char* method, char* status_line, char* response_header);

#endif /* HTTP_HEAD_GET_REQUEST_H */
