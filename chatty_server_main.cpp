#include "chatty_server_main.hpp"
#include <iostream>

int main() {

    //sqlite3 라이브러리 버전
    chatty_db::print_library_version();
    //chatty db 버전
    chatty_db::print_version();




    chatty_time_obj::numeric_string_object* hello;
    hello = chatty_time::get_millisec();
    chatty_time::print_millisec_object(hello);

    delete hello;

    exit(1);



    //5555포트로 연결테스트 tcp
    if (true) {
        chatty_net::open(5555);
    } else {
        std::cout << "비정상적인 임베디드 데이터베이스 초기화로 인한 실행거부" << '\n';
    };

    //연결 종료 (finalize and close)
    //sub_db_instance.close();
    return 0;
}
