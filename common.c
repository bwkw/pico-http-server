#include "common.h"

// HTTPリクエストヘッダーの読み取り
int read_http_request_header(int sock_fd, char* request_header, int* total_received_length) {
    int received = 0;
    do {
        received = read(sock_fd, request_header + *total_received_length, MAX_HEADER_SIZE - *total_received_length);
        if (received < 0) {
            perror("Failed to receive bytes from client.");
            return -1;
        }
        *total_received_length += received;
        if (*total_received_length > MAX_HEADER_SIZE) {
            fprintf(stderr, "Error: Too large header_size.\n");
            return -1;
        }
        request_header[*total_received_length] = '\0';
        char* request_header_end_marker = strstr(request_header, "\r\n\r\n");

        if (request_header_end_marker) {
            return request_header_end_marker + 4 - request_header;
        }
    } while (*total_received_length < MAX_HEADER_SIZE);
    
    return -1;
}

// レスポンスの返却
void send_response(const int sock_fd, const char* status_line, char* response_header, const char* response_body, const int response_body_size) {
    write(sock_fd, status_line, strlen(status_line));

    char content_length_header[64];
    sprintf(content_length_header, "Content-Length: %d\r\n\r\n", response_body_size);
    strcat(response_header, content_length_header);
    write(sock_fd, response_header, strlen(response_header));

    if (response_body_size > 0) {
        write(sock_fd, response_body, response_body_size);
    }
}

// メソッドが有効化のチェック
int is_valid_request_method(char* method) {
    char* validMethods[] = { "HEAD", "GET", "POST" };
    int is_valid = 0;
    for (int i = 0; i < sizeof(validMethods) / sizeof(validMethods[0]); i++) {
        if (strcmp(method, validMethods[i]) == 0) {
            is_valid = 1;
            break;
        }
    }
    return is_valid;
}
