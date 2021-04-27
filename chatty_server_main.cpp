#include "chatty_server_main.hpp"
#include <iostream>

int main() {

    //sqlite3 라이브러리 버전
    chatty_db::print_library_version();
    //chatty db 버전
    chatty_db::print_version();

    CHATTY_ANY* query_result(nullptr);
    CHATTY_ERROR_CODE ref_code(0);

//    //insertion or execution
//    query_result = chatty_embedded_db_controller::fetch_request_exec(
//            (CHATTY_UCHAR_PTR)CHATTY_DB_NAME,
//            (CHATTY_UCHAR_PTR)"INSERT INTO CHATTY_CONNECTION (id,latest_connect_date,is_online) VALUES (5+"
//                              "5,45,0)",
//            nullptr,
//            &ref_code
//    );
//    if (query_result != nullptr) {
//        std::cout << "xczxcxz" << '\n';
//        chatty_embedded_db_controller::fetch_request_exec_release(query_result);
//    };

    std::cout << std::endl << std::endl << std::endl;
    std::cout << "#####################################" << '\n';
    std::cout << "#####################################" << '\n';
    std::cout << "#####################################" << '\n';
    std::cout << std::endl << std::endl << std::endl;

    //selection
    query_result = chatty_embedded_db_controller::fetch_request_exec(
            (CHATTY_UCHAR_PTR)CHATTY_DB_NAME,
            (CHATTY_UCHAR_PTR)"SELECT id,latest_connect_date,is_online FROM CHATTY_CONNECTION",
            (CHATTY_UCHAR_PTR)"text:int64:int32",
            &ref_code
            );

    //debug : 로우 찍히는지 테스트
    if (query_result != nullptr && ref_code != CHATTY_STATUS_ERROR) {
        CHATTY_DB_FETCH_RESULT* _dummy_test_query_result = (CHATTY_DB_FETCH_RESULT*)query_result;
        for(int idx=0;idx<_dummy_test_query_result->size;++idx) {
            void** buffer_row = (void**)_dummy_test_query_result->value[idx];
            unsigned char* _column_id = (unsigned char*)buffer_row[0];
            std::cout << "id : " << _column_id << '\n';
        };
    } else {
        std::cout << "결과값이 없어서 테스트 스킵!" << '\n';
    };

    if (query_result != nullptr) {
        chatty_embedded_db_controller::fetch_request_exec_release(query_result);
    };




    //5555포트로 연결테스트 tcp
    if (ref_code == CHATTY_STATUS_OK && false) {
        chatty_net::open(5555);
    } else {
        std::cout << "비정상적인 임베디드 데이터베이스 초기화로 인한 실행거부" << '\n';
    };

    //연결 종료 (finalize and close)
    //sub_db_instance.close();
    return 0;
}
