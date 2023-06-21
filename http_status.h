#ifndef HTTP_STATUS_H
#define HTTP_STATUS_H

typedef struct {
    int code;
    char* description;
} HttpStatus;

// HTTPステータスを返す関数の宣言
HttpStatus get_status_by_code(int code);

#endif // HTTP_STATUS_H
