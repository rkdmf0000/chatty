# chatty

날려먹는거 방지용

목적은 서버의 요청을 통한 상호 채팅서버 구현에 있음 

네트워크 프로그래밍 공부용으로 하고있는데

내부적으로 sqlite3 사용하는디

sql query helper 만들어서 쓰려고 하는중  


## 20210426 on proc - 쿼리 보내고 메모리 릴리즈하는 부붐까지 완성
```c++
    CHATTY_ANY* query_result(nullptr);
    CHATTY_ERROR_CODE ref_code(0);
    //SELECT * FROM CHATTY_CONNECTION
    //INSERT INTO CHATTY_CONNECTION (id, latest_connect_date, is_online) VALUES (7,5455612, 1)
    query_result = chatty_embedded_db_controller::fetch_request_exec(
            (CHATTY_UCHAR_PTR)CHATTY_DB_NAME,
            (CHATTY_UCHAR_PTR)"SELECT * FROM CHATTY_CONNECTION",
            (CHATTY_UCHAR_PTR)"INT32:int32:int64",
            &ref_code
            );

    std::cout << "fetch_request_exec returned value-addr : 0 to " << query_result << '\n';
    std::cout << "fetch_request_exec returned status : " << ref_code << '\n';

    //debug : 로우 찍히는지 테스트
    CHATTY_DB_FETCH_RESULT* _dummy_test_query_result = (CHATTY_DB_FETCH_RESULT*)query_result;
    for(int idx=0;idx<_dummy_test_query_result->size;++idx) {
        void** buffer_row = (void**)_dummy_test_query_result->value[idx];
        int* _column_idx = (int*)buffer_row[0];
        std::cout << "idx : " << *_column_idx << '\n';
    };

    //메로리 해제
    chatty_embedded_db_controller::fetch_request_exec_release(query_result);


```






## 20210424 on proc
```c++
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

```



```console

print_library_version : 3.35.4
print_version : 0.1.0.5
data_type str size : 17
data_type cnt : 2
data_type last thing size expected : 5
- - - - ready letter space : 5
- - - - ready letter space : 5
- - - - ready letter space : 5
(Notice) be setting a column to `int32`
(Notice) be setting a column to `int64`
(Notice) be setting a column to `UNDEFINED`
(Success) db open
(Success) prepare skipped
(Notice) statement is done (101)
(Notice) exed put on reference to status.
(Notice) ** reset (0), finalize (0), close (0)
fetch_request_exec returned value-addr : 0 to 0
fetch_request_exec returned status : 101
비정상적인 임베디드 데이터베이스 초기화로 인한 실행거부
```






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


    /** connection의 경우 아래 스트럭쳐 그대로 사용하면 됨
    *   struct CHATTY_DB_COLUMN_CONNECTION {
    *       CHATTY_UINT32   idx;
    *       CHATTY_UINT32   id;
    *       CHATTY_UINT32   latest_connect_date;
    *       CHATTY_FLAG     is_online;
    *   };
    */

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
