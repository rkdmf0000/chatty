//
// Created by my_fl on 2021-02-16.
//

#ifndef CHATTY_CHATTY_EMBEDDED_DB_MASTER_H
#define CHATTY_CHATTY_EMBEDDED_DB_MASTER_H

#include "chatty_defs.h"

#include "chatty_embedded_db_includes.h"
#include "chatty_embedded_db_type_defs.h"

#include "chatty_embedded_db_interface.h"

//#include "chatty_embedded_db_request.h"
//#include "chatty_embedded_db_result.h"

class chatty_embedded_db_master : public chatty_embedded_db_interface {
protected:
    //friend class chatty_embedded_db_interface;

    //chatty_embedded_db_request* request;
    //chatty_embedded_db_result* result;

    //for buffer data insertion
    CHATTY_DB_LIB selected_db{};            //sqlite3 디비 인스턴스 (dependencies by library)
    CHATTY_DB_LIB_STMT selected_db_handle{};     //sqlite3 디비 핸들러 인스턴스 (dependencies by library)

    /*
     * ※ 주의
     * 아래 변수는 __mem_query_add_str 에서 사용하는 직렬 변수입니다
     * __mem_query_str_flush 를 하기 전까지 중복해서 사용할 수 없습니다.
     * */
    CHATTY_QUERY_PTR query_str;             //memory 문자열 구문
    CHATTY_SIZE query_str_size;             //memory 문자열 길이

    //pre-finished request
    CHATTY_QUERY_FORM query_form;

    //flags
    CHATTY_FLAG is_db_standby;
    CHATTY_FLAG is_db_prepare_success;
    CHATTY_FLAG is_db_step_success;

    CHATTY_CHAR_PTR db_error_msg;
    CHATTY_UCHAR db_error_code; //it is different `latest_error_status` because it doesn't change at connection level works

    // it'll change at db connection level proceed
    CHATTY_ERROR_STATUS latest_error_status;

protected:
    static CHATTY_ANY test_print(CHATTY_CHAR_PTR text);
    CHATTY_ANY __print_query_block(CHATTY_ANY* block_ptr);
    CHATTY_ANY __mem_query_add_str(CHATTY_UCHAR_PTR str, CHATTY_SIZE size);
    CHATTY_ANY __mem_query_str_flush(CHATTY_ANY* ref);
    CHATTY_ANY __mem_query_box_insert(CHATTY_ANY* block, CHATTY_ANY* ref);
    CHATTY_ANY __print_query_box(CHATTY_ANY* box_ptr);
    CHATTY_ANY __print_table_name() const;
    CHATTY_ANY __conv_query_quotes(CHATTY_ANY* block, CHATTY_QUOTES_TYPE type = CHATTY_KEYWORD_QUOTE_TYPE_DOUBLE);
    CHATTY_ANY __mem_injected_update_data(CHATTY_ANY* pallet, CHATTY_SIZE pallet_size, CHATTY_SIZE current_size, CHATTY_UINT32 where_at_ijt, CHATTY_ANY* ijt_data);

    CHATTY_ANY __update_query_execution_status();

//    CHATTY_ANY query();
//    CHATTY_ANY fetch_one();
//    CHATTY_ANY fetch_all();

public:
    chatty_embedded_db_master();
    ~chatty_embedded_db_master();

    CHATTY_STATIC CHATTY_ANY print_library_version();
    CHATTY_STATIC CHATTY_ANY print_version();

    CHATTY_ERROR_STATUS get_error_status() const;
    CHATTY_CHAR_PTR get_error_msg();

    CHATTY_ANY test();
};


#endif //CHATTY_CHATTY_EMBEDDED_DB_MASTER_H
