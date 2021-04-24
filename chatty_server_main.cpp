#include "chatty_server_main.hpp"
#include <iostream>

int main() {

    //sqlite3 라이브러리 버전
    chatty_db::print_library_version();
    //chatty db 버전
    chatty_db::print_version();

    CHATTY_ANY* query_result(nullptr);
    CHATTY_ERROR_CODE ref_code(0);
    query_result = chatty_embedded_db_controller::fetch_request_exec(
            (CHATTY_UCHAR_PTR)CHATTY_DB_NAME,
            (CHATTY_UCHAR_PTR)"SELECT * FROM CHATTY_CONNECTION",
            (CHATTY_UCHAR_PTR)"int32:int64:inT32",
            &ref_code
            );

    std::cout << "fetch_request_exec returned value-addr : 0 to " << query_result << '\n';
    std::cout << "fetch_request_exec returned status : " << ref_code << '\n';



    //5555포트로 연결테스트 tcp
    if (ref_code == CHATTY_STATUS_OK) {
        chatty_net::open(5555);
    } else {
        std::cout << "비정상적인 임베디드 데이터베이스 초기화로 인한 실행거부" << '\n';
    };

    //연결 종료 (finalize and close)
    //sub_db_instance.close();
    return 0;
}
