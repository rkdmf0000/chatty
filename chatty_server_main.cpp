#include "chatty_server_main.hpp"
#include <iostream>

int main() {
    chatty_db::print_library_version();
    chatty_db::print_version();
    chatty_db test_instance{};

    test_instance.use(CHATTY_DB_NAME);

    if (test_instance.get_error_status() != CHATTY_STATUS_OK) {
        std::cout << "- - -DB CONNECT IS FAILED" << '\n';
        std::cout << test_instance.get_error_msg() << '\n';
    } else {
        std::cout << "- - -DB CONNECT IS SUCCESS" << '\n';
        std::cout << test_instance.get_error_msg() << '\n';
    };

    //SELECT를 하겠다는 뜻 (이거에 따라 쿼리 구문이 달라짐)
    test_instance.behavior(CHATTY_KEYWORD_REQUEST_SELECT);

    //TABLE 이름 정의
    test_instance.select((unsigned char*)CHATTY_T_CONN);

    //컬럼 정의
    test_instance.column((CHATTY_UCHAR_PTR)"*");

    //복수 확인 (메모리 해제 필요)
    CHATTY_DB_COLUMN_CONNECTION_GROUP_COLLECTION test_result_fetch_array_group;
    test_instance.fetchall_connection(&test_result_fetch_array_group);
    chatty_embedded_db_controller::fetchall_connection_release(&test_result_fetch_array_group);

    //단수 확인 (메모리 해제 필요 없음)
    CHATTY_DB_COLUMN_CONNECTION* test_x;
    test_instance.fetchone_connection(&test_x);

    //연결 종료 (finalize and close)
    test_instance.close();

    return 0;
}
