//
// Created by my_fl on 2021-04-21.
//아직 암것도 안함ㅋㅋ2

#include "chatty_network_master.h"


CHATTY_ANY chatty_network_master::open(CHATTY_UINT32 port) {
    //?
    CHATTY_INT32 serv_sock;
    CHATTY_INT32 clnt_sock;

    //?
    sockaddr_in serv_addr;
    sockaddr_in clnt_addr; //코드 어두에 struct 빼고 씀

    CHATTY_UINT32 clnt_addr_size; //socklen_t = unsigned int 대신에 chatty_uint32를 씀

#if defined(CHATTY_DO_BUILD_DEBUG)
    chatty_embedded_db_master::test_print((CHATTY_CHAR_PTR)"SERVER SOCKET OPEN");
    std::cout << "at :" << port << " port" << '\n';
#endif


    serv_sock = socket(PF_INET, SOCK_STREAM, 0); //pf_inet ipv4 의미 sock_stream tcp communicate 를 의미
    if (serv_sock == -1) {
#if defined(CHATTY_DO_BUILD_DEBUG)
        chatty_embedded_db_master::test_print((CHATTY_CHAR_PTR)"SERVER SOCKET GENERIC ERROR");
#endif
        abort();
        return;
    };

    memset(&serv_addr,0 ,sizeof(serv_addr)); //0으로 초기화
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(static_cast<CHATTY_INT32>(port));


    if(bind(serv_sock, (sockaddr*) &serv_addr, sizeof(serv_addr)) == -1) {
#if defined(CHATTY_DO_BUILD_DEBUG)
        chatty_embedded_db_master::test_print((CHATTY_CHAR_PTR)"SERVER SOCKET BINDING ERROR");
#endif
        abort();
        return;
    };

    if (listen(serv_sock, 5) == -1) {
#if defined(CHATTY_DO_BUILD_DEBUG)
        chatty_embedded_db_master::test_print((CHATTY_CHAR_PTR)"SERVER SOCKET LISTENING ERROR");
#endif
        abort();
        return;
    };

    clnt_addr_size = sizeof(clnt_addr);
    clnt_sock = accept(serv_sock, (sockaddr*)&clnt_addr, &clnt_addr_size);
    if (clnt_sock == -1) {
#if defined(CHATTY_DO_BUILD_DEBUG)
        chatty_embedded_db_master::test_print((CHATTY_CHAR_PTR)"SERVER SOCKET ACCEPTING ERROR");
#endif
        abort();
        return;
    };


    CHATTY_CHAR msg[] = "Hello this is server message!\r\n";
    write(clnt_sock, msg, sizeof(msg));

    close(clnt_sock);
    close(serv_sock);
};
