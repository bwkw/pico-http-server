#include "http_status.h"

// HTTPステータスを定義
HttpStatus statuses[] = {
    {200, "OK"},
    {201, "Created"},
    {307, "Temporary Redirect"},
    {308, "Permanent Redirect"},
    {403, "Forbidden"},
    {404, "Not Found"},
    {405, "Method Not Allowed"},
    {500, "Internal Server Error"}
};

// HTTPステータスを返す関数の定義
HttpStatus get_status_by_code(int code) {
    int num_statuses = sizeof(statuses) / sizeof(HttpStatus);
    for (int i = 0; i < num_statuses; i++) {
        if (statuses[i].code == code) {
            return statuses[i];
        }
    }

    HttpStatus default_status = {500, "Internal Server Error"};
    return default_status;
}
