# chatty

날려먹는거 방지용

목적은 서버의 요청을 통한 상호 채팅서버 구현에 있음 

네트워크 프로그래밍 공부용으로 하고있는데

내부적으로 sqlite3 사용하는디

sql query helper 만들어서 쓰려고 하는중  


```c++
    //sqlite3 라이브러리 버전
    chatty_db::print_library_version();

    //chatty db 버전
    chatty_db::print_version();

    chatty_db test_instance{};
    chatty_net test_net_instance;

    //사용하는 db
    //sqlite3 예약어 :memory: 동일하게 작동함
    test_instance.use(CHATTY_DB_NAME);

    //연결확인
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
    if (test_instance.fetchall_connection(&test_result_fetch_array_group)) {
        chatty_embedded_db_controller::fetchall_connection_release(&test_result_fetch_array_group);
    };

    //단수 확인 (메모리 해제 필요 없음)
    CHATTY_DB_COLUMN_CONNECTION* test_x;
    if (test_instance.fetchone_connection(&test_x)) {
        //Something to do
    };

    //5555포트로 연결테스트 tcp
    chatty_net::open(5555);

    //연결 종료 (finalize and close)
    test_instance.close();
```

요런 느낌의 helper
