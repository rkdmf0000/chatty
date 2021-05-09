#include "chatty_server_main.hpp"
#include <iostream>

int main(void) {

    for(int zzz(0);zzz<100;++zzz)
        std::cout << std::endl << std::endl << std::endl << std::endl << std::endl;

    std::cout << "■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■" << '\n';
    std::cout << "□□□□□□□□□□□□□□□□□[ program start ]□□□□□□□□□□□□□□□□□" << '\n';
    std::cout << "□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□" << '\n';
    std::cout << '\n';

    //sqlite3 라이브러리 버전
    chatty_db::print_library_version();
    //chatty db 버전
    chatty_db::print_version();





    //5555포트로 연결테스트 tcp
    if (true) {
        chatty_net::open(5555);
    } else {
        std::cout << "비정상적인 임베디드 데이터베이스 초기화로 인한 실행거부" << '\n';
    };


    std::cout << '\n';
    std::cout << "□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□" << '\n';
    std::cout << "□□□□□□□□□□□□□□□□[ program is over ]□□□□□□□□□□□□□□□□" << '\n';
    std::cout << "■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■" << '\n';
    return 0;
};
