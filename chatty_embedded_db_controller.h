//
// Created by my_fl on 2021-02-21.
//

#ifndef CHATTY_CHATTY_EMBEDDED_DB_CONTROLLER_H
#define CHATTY_CHATTY_EMBEDDED_DB_CONTROLLER_H

#include "chatty_embedded_db_master.h"

class chatty_embedded_db_controller : public chatty_embedded_db_master {
public:
    chatty_embedded_db_controller() = default;
    ~chatty_embedded_db_controller() = default;

    /**
     * CONNECTIONS
     * */
    CHATTY_ANY use(const CHATTY_STRING&);
    CHATTY_ANY close();

    CHATTY_ANY handler();
    CHATTY_ANY handle_finalize();


    /**
     * COMMON
     * */


    /**
     * @title   : fetch_request_exec
     * @desc    : 쿼리를 보내는 함수
     * -----------------------------
     * @param   unsigned char* db_path          데이터베이스 파일의 위치와 파일명 없으면 sqlite3 라이브러리 특성상 자동으로 생성된다.
     *                                          sqlite3의 예약어인 :memory: 사용 가능하다.
     * @param   unsigned char* query            sqlite3의 sql 구문
     * @param   unsigned char* data_type        출력될 각 컬럼의 데이터타입을 정의한다
     *                                          구문 예시) int32:text:int64
     *                                          예시와 같이 3개의 컬럼으로 정의된다. access 는 void* 로 0, 1, 2 로 들어간다.
     * @param   unsigned int return_error_code  실행시 발생하는 성공이나 에러에 해당하는 정수를 반환한다.
     * */
    CHATTY_STATIC CHATTY_ANY* fetch_request_exec(CHATTY_UCHAR_PTR db_path,
                                                 CHATTY_UCHAR_PTR query,
                                                 CHATTY_UCHAR_PTR data_type,
                                                 CHATTY_ERROR_CODE* return_error_code);
    /**
     * @title   : fetch_request_exec_release
     * @desc    : fetch_request_exec - CHATTY_DB_FETCH_RESULT structure 를 해제하는 함수입니다.
     *            옳바른 구조체의 경우 해제에 필요한 데이터는 다 안에 포함되있다.
     * -----------------------------
     * @param   unsignedCHATTY_DB_FETCH_RESULT* addr    해제할 변수의 주소
     * */
    CHATTY_STATIC CHATTY_ANY fetch_request_exec_release(CHATTY_ANY* addr);
    CHATTY_STATIC CHATTY_ANY fetch_request_exec_release(CHATTY_DB_FETCH_RESULT* addr);




    CHATTY_ANY behavior(CHATTY_UCHAR type);
    CHATTY_ANY column(CHATTY_UCHAR_PTR column);
    CHATTY_ANY column(CHATTY_UCHAR_PTR column, CHATTY_UCHAR_PTR value);

    CHATTY_ERROR_CODE exec();
    CHATTY_ERROR_CODE query(CHATTY_CHAR_PTR query);
    CHATTY_SIZE count();

    CHATTY_FLAG fetchall_connection(CHATTY_ANY* group);
    CHATTY_FLAG fetchone_connection(CHATTY_ANY* article);
    CHATTY_STATIC CHATTY_FLAG fetchall_connection_release(CHATTY_ANY* pointer);


    CHATTY_FLAG get_count();


    /**
     * REQUESTS-SEARCH
     * */
    CHATTY_ANY select(CHATTY_UCHAR_PTR table);
    CHATTY_ANY where_and(CHATTY_UCHAR_PTR column, CHATTY_UCHAR_PTR value);
    CHATTY_ANY where_or(CHATTY_UCHAR_PTR column, CHATTY_UCHAR_PTR value);


    /**
     * REQUESTS-ACTION
     * */
    //CHATTY_ANY delete_search(const CHATTY_UCHAR_PTR column, const CHATTY_UCHAR_PTR value);

private:
    /**
     * generate query function
     * */
    CHATTY_ANY _generate_select_column_common();
    CHATTY_ANY _generate_where_common();
    CHATTY_ANY _generate_non_value_grouping_column_key();
    CHATTY_ANY _generate_non_value_grouping_column();
    CHATTY_ANY _generate_query_block(CHATTY_ANY* block);


};



#endif //CHATTY_CHATTY_EMBEDDED_DB_CONTROLLER_H
