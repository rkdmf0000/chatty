# chatty chatting server

날려먹는거 방지용

목적은 서버의 요청을 통한 상호 채팅서버 구현에 있음 

네트워크 프로그래밍 공부용으로 하고있는데

내부적으로 sqlite3 사용하는디

sql query helper 만들어서 쓰려고 하는중  


## 20210502 시간 관련 함수 작업중
> 1 unix millisecond timestamp 가 너무 수치가 커서 계산이 틈만 나면 계산이 꼬임.
> 
> 2 그래서 제한 없는 특수 정수 하나 만들고 사칙연산 및 mod 연산기능까지 넣고 사용할 예정 
```console
milli-sec size : 13
value is : 1613141870975
@millisec object child heap node deleted / 0x559be838b2e8

```

## 20210427 서버 통신 파트랑 시간관련 클레스 작업시작
> - JSON PARSER && JSON GENERATOR 작업 확인중 - `라이브러리 쓸지 만들어 쓸지 고민중`
```c++
작업중
```


## 20210427 on proc - 쿼리 부족한 부분까지 완성

```c++

    CHATTY_ANY* query_result(nullptr);
    CHATTY_ERROR_CODE ref_code(0);

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

```
```console
(Notice) it's on type casting to `CHATTY_DB_FETCH_RESULT*` by language syntax overload for release-ment process
(Notice) release-ment on start
(Info) column count : 3 / row count : 7
- - - - - - - - column delete : 0x5651377adb80 / data_type : text
- - - - - - - - column delete : 0x5651377adac0 / data_type : int64
- - - - - - - - column delete : 0x5651377ada00 / data_type : int32
- - - - - - row delete : 0x565137796a40
- - - - - - - - column delete : 0x5651377ae4b0 / data_type : text
- - - - - - - - column delete : 0x5651377ae4d0 / data_type : int64
- - - - - - - - column delete : 0x5651377ae4f0 / data_type : int32
- - - - - - row delete : 0x565137796870
- - - - - - - - column delete : 0x5651377ae530 / data_type : text
- - - - - - - - column delete : 0x5651377ae550 / data_type : int64
- - - - - - - - column delete : 0x5651377ae570 / data_type : int32
- - - - - - row delete : 0x5651377ae490
- - - - - - - - column delete : 0x5651377ae590 / data_type : text
- - - - - - - - column delete : 0x5651377ae5b0 / data_type : int64
- - - - - - - - column delete : 0x5651377ae5d0 / data_type : int32
- - - - - - row delete : 0x5651377ad800
- - - - - - - - column delete : 0x5651377ae5f0 / data_type : text
- - - - - - - - column delete : 0x5651377ae610 / data_type : int64
- - - - - - - - column delete : 0x5651377ae630 / data_type : int32
- - - - - - row delete : 0x565137796ae0
- - - - - - - - column delete : 0x5651377ae670 / data_type : text
- - - - - - - - column delete : 0x5651377ae690 / data_type : int64
- - - - - - - - column delete : 0x5651377ae6b0 / data_type : int32
- - - - - - row delete : 0x5651377ae510
- - - - - - - - column delete : 0x5651377ae730 / data_type : text
- - - - - - - - column delete : 0x5651377ae750 / data_type : int64
- - - - - - - - column delete : 0x5651377ae770 / data_type : int32
- - - - - - row delete : 0x5651377ae650
- - - - value delete : 0x5651377ae6d0
- - self delete : 0x5651377adc40
→ deleted _defined_type and column_type

```

## 20210426 on proc - 쿼리 보내고 메모리 릴리즈하는 부붐까지 완성

`query`파라미터에 `SELECT id,id,latest_connect_date FROM CHATTY_CONNECTION` 구문을 입력하는 경우
`id,id,latest_connect_date` 3개의 컬럼에 맞게
`data_type` 파라미터에 `int64:int64:int64` 3개를 입력해줘야 에러가 안난다

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

```console
print_library_version : 3.35.4
print_version : 0.3.0.1
(Success) DB open
(Success) prepare skipped
(QUERY) got a rows (100)
(Notice) going on `row` refining proceeds
- - row count (st) : 1
(Processing-st) be setting a (row : 1) 0 column to `int32` - 0x55bdad32b2f0
(Processing-st) be setting a (row : 1) 1 column to `int32` - 0x55bdad32b660
(Processing-st) be setting a (row : 1) 2 column to `int64` - 0x55bdad32b770
- - - - 0x55bdad32b400 row is now made (st)


- - row count (nd) : 2
- - - - 0x55bdad32b400 row is now copied! (nd)
(Processing-nd) be setting a (row : 2) 0 column to `int32` - 0x55bdad32b7d0
(Processing-nd) be setting a (row : 2) 1 column to `int32` - 0x55bdad32b7f0
(Processing-nd) be setting a (row : 2) 2 column to `int64` - 0x55bdad32b810
- - - - 0x55bdad32b3e0 row is now made (nd)


- - row count (nd) : 3
- - - - 0x55bdad32b400 row is now copied! (nd)
- - - - 0x55bdad32b3e0 row is now copied! (nd)
(Processing-nd) be setting a (row : 3) 0 column to `int32` - 0x55bdad32b850
(Processing-nd) be setting a (row : 3) 1 column to `int32` - 0x55bdad32b870
(Processing-nd) be setting a (row : 3) 2 column to `int64` - 0x55bdad32b890
- - - - 0x55bdad32b7b0 row is now made (nd)


- - row count (nd) : 4
- - - - 0x55bdad32b400 row is now copied! (nd)
- - - - 0x55bdad32b3e0 row is now copied! (nd)
- - - - 0x55bdad32b7b0 row is now copied! (nd)
(Processing-nd) be setting a (row : 4) 0 column to `int32` - 0x55bdad32b8e0
(Processing-nd) be setting a (row : 4) 1 column to `int32` - 0x55bdad32b900
(Processing-nd) be setting a (row : 4) 2 column to `int64` - 0x55bdad32b920
- - - - 0x55bdad32b350 row is now made (nd)


(Notice) exed put on reference to status.
(Notice) ** reset (0), finalize (0), close (0)
(Success) row returned
fetch_request_exec returned value-addr : 0 to 0x55bdad32cb60
fetch_request_exec returned status : 101
idx : 1
idx : 2
idx : 3
idx : 4
(Notice) it's on type casting to `CHATTY_DB_FETCH_RESULT*` by language syntax overload for release-ment process
(Notice) release-ment on start
(Info) column count : 3 / row count : 4
- - - - - - - - column delete : 0x55bdad32b2f0
- - - - - - - - column delete : 0x55bdad32b660
- - - - - - - - column delete : 0x55bdad32b770
- - - - - - row delete : 0x55bdad32b400
- - - - - - - - column delete : 0x55bdad32b7d0
- - - - - - - - column delete : 0x55bdad32b7f0
- - - - - - - - column delete : 0x55bdad32b810
- - - - - - row delete : 0x55bdad32b3e0
- - - - - - - - column delete : 0x55bdad32b850
- - - - - - - - column delete : 0x55bdad32b870
- - - - - - - - column delete : 0x55bdad32b890
- - - - - - row delete : 0x55bdad32b7b0
- - - - - - - - column delete : 0x55bdad32b8e0
- - - - - - - - column delete : 0x55bdad32b900
- - - - - - - - column delete : 0x55bdad32b920
- - - - - - row delete : 0x55bdad32b350
- - - - value delete : 0x55bdad32b8b0
- - self delete : 0x55bdad32cb60

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
