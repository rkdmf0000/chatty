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
            (CHATTY_UCHAR_PTR)"SELECT id,id,latest_connect_date FROM CHATTY_CONNECTION",
            (CHATTY_UCHAR_PTR)"int64:int64:int64",
            &ref_code
            );

    //debug : 로우 찍히는지 테스트
    CHATTY_DB_FETCH_RESULT* _dummy_test_query_result = (CHATTY_DB_FETCH_RESULT*)query_result;
    for(int idx=0;idx<_dummy_test_query_result->size;++idx) {
        void** buffer_row = (void**)_dummy_test_query_result->value[idx];
        int* _column_id = (int*)buffer_row[0];
        std::cout << "id : " << *_column_id << '\n';
    };

    //메로리 해제
    chatty_embedded_db_controller::fetch_request_exec_release(query_result);


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
