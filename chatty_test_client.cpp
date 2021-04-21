//
// Created by my_fl on 2021-04-21.
//
#include "chatty_test_client.hpp"
int main(int argc, char* argv[]){
    int clnt_sock;

    sockaddr_in serv_addr;
    char message[1204] = {0x00, };

    clnt_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (clnt_sock == -1) {
        std::cout << "client socket generic error occurred!" << '\n';
        abort();
    };

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if(connect(clnt_sock, (sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) {
        std::cout << "client connection error occurred!" << '\n';
        abort();
    };

    if (read(clnt_sock, message, sizeof(message)-1) == -1) {
        std::cout << "client read error occurred!" << '\n';
        abort();
    };

    std::cout << "Message received!" << '\n';
    std::cout << "D : " << message << '\n';

    close(clnt_sock);
    return 0;
}