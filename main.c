#include <stdio.h>
#include "http_server.h"
#include "validation.h"

int main(int argc, char *argv[]) {
    char* ip_address = "127.0.0.1";
    int port = 8080;
    char* root_dir = ".";
    parseArguments(argc, argv, &ip_address, &port, &root_dir);

    return run_http_server(ip_address, port, root_dir);
}
