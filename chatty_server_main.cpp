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
    test_instance.where_and((CHATTY_UCHAR_PTR)"id",(CHATTY_UCHAR_PTR)"11");

    std::cout << "QUERY RUN RESULT : " << test_instance.exec() << '\n';

    /*
     * //for select behavior
    test_instance.where_and((CHATTY_UCHAR_PTR)"test_aa", (CHATTY_UCHAR_PTR)"가\'나");
    test_instance.where_or((CHATTY_UCHAR_PTR)"4564", (CHATTY_UCHAR_PTR)"7\"8\"9");
    test_instance.where_and((CHATTY_UCHAR_PTR)"description", (CHATTY_UCHAR_PTR)"홍콩을 가자!!");
    */


    //test_instance.test();
    test_instance.close();

    return 0;
}
