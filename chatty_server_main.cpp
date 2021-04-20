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

    test_instance.behavior(CHATTY_KEYWORD_REQUEST_SELECT);
    test_instance.select((unsigned char*)CHATTY_T_CONN);
    test_instance.column((CHATTY_UCHAR_PTR)"*");

    CHATTY_DB_COLUMN_CONNECTION_GROUP_COLLECTION test_result_fetch_array_group;
    test_instance.fetchall_connection(&test_result_fetch_array_group);
    chatty_embedded_db_controller::fetchall_connection_release(&test_result_fetch_array_group);

    CHATTY_DB_COLUMN_CONNECTION* test_x;
    test_instance.fetchone_connection(&test_x);

    //test_instance.test();
    test_instance.close();

    return 0;
}
