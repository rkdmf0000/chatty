//
// Created by my_fl on 2021-02-16.
//
#include "chatty_embedded_db_master.h"



CHATTY_ERROR_STATUS chatty_embedded_db_master::get_error_status() const {
    return this->latest_error_status;
};

chatty_embedded_db_master::chatty_embedded_db_master() {
    //db is connection flags
    this->is_db_standby = false;
    this->is_db_prepare_success = false;
    this->is_db_step_success = false;

    //db error status
    this->db_error_msg = nullptr;
    this->db_error_code = 0;

    //query inits
    this->query_str = nullptr;
    this->query_str_size = 0;

    //error code for low level status
    this->latest_error_status = 0;

    //member init
    //this->request = new chatty_embedded_db_request();
    //this->result = new chatty_embedded_db_result();
};

chatty_embedded_db_master::~chatty_embedded_db_master() {
    //delete this->request;
    //delete this->result;
    delete query_str;
    delete db_error_msg;
    delete this->query_form.column_part_key.value;
    delete this->query_form.column_part.value;
    delete this->query_form.filter_part_key.value;
    delete this->query_form.filter_part.value;
};

CHATTY_CHAR_PTR chatty_embedded_db_master::get_error_msg() {
    return const_cast<CHATTY_CHAR_PTR>(sqlite3_errmsg(this->selected_db));
}

void chatty_embedded_db_master::print_library_version(){
    printf("%s : %s\n", __FUNCTION__, sqlite3_libversion());
};

CHATTY_ANY chatty_embedded_db_master::print_version() {
    printf("%s : %s\n", __FUNCTION__, CHATTY_VERSION);
};

CHATTY_ANY chatty_embedded_db_master::__print_table_name() const {
    printf("TARGET TABLE NAME : %s\n", this->query_form.target_table_name.value);
};

CHATTY_ANY chatty_embedded_db_master::test_print(CHATTY_CHAR_PTR text) {
#if defined(CHATTY_DO_BUILD_DEBUG)
    std::cout << std::endl << std::endl;
    std::cout << "━━─━─━━─━─━━" << '\n';
    std::cout << " - FOR DEBUG" << '\n';
#endif
    std::cout << "━━─━─━━─━─━━" << '\n';
    std::cout << text << '\n';
    std::cout << "━━━━━━━━━━━━" << '\n';

};

CHATTY_ANY chatty_embedded_db_master::__update_query_execution_status() {
//    if (db_error_msg != nullptr)
//        sqlite3_free(this->db_error_msg);
    this->db_error_msg = get_error_msg();
    this->db_error_code = sqlite3_errcode(this->selected_db);
};

/**
 * 쿼리 문자열을 더하기 위해 메모리를 초기화하고 옮깁니다.
 * @param   str     등록할 쿼리 문자열
 * @param   size    쿼리 문자열의 길이, (널포인트크기를 포함합니다)
 * */
CHATTY_ANY chatty_embedded_db_master::__mem_query_add_str(CHATTY_UCHAR_PTR str, CHATTY_SIZE size) {
    //널바이트를 제거하기 위한 수
    CHATTY_UINT32 shift_n(0);
    CHATTY_SIZE movepoint(0);
    if (this->query_str_size != 0) ++shift_n;

    //최종적인 크기
    CHATTY_SIZE needs_size = this->query_str_size + size - shift_n;

    //널바이트가 중복되는 크기만금 뺀 나머지 크기만큼 동적 할당
    CHATTY_QUERY_PTR dummy = (CHATTY_QUERY_PTR)malloc(needs_size);

    //더미를 초기화
    for (int i=0;i<needs_size;++i)
        dummy[i] = 0;

    //널바이트 뺀 메모리를 복사
    if (this->query_str_size != 0) {
        std::memcpy(dummy, this->query_str, this->query_str_size-1);
        movepoint = this->query_str_size-1;
    };

    //추가 입력할 메모리 이동
    std::memcpy(dummy+movepoint, str, size);
    movepoint += size; //더 이상 쓸 일 없지만 변수의 의도를 완료하기 위해 불필요하게 적음. 이런게 인간적인거지ㅋㅋ.

    //메모리 해제
    free(this->query_str);

    //더미 대체 입력
    this->query_str = dummy;
    this->query_str_size = needs_size;
};

CHATTY_ANY chatty_embedded_db_master::__mem_query_str_flush(CHATTY_ANY *ref) {
    CHATTY_QUERY_BLOCK* r_ref   = (CHATTY_QUERY_BLOCK*)ref;
    r_ref->value                = this->query_str;
    r_ref->value_size           = this->query_str_size+1;

    //free-space
    this->query_str = 0;
    this->query_str_size = 0;
    delete this->query_str, this->query_str_size;
};

// note : 이거 다른 함수로 가면 값을 잃음

CHATTY_ANY chatty_embedded_db_master::__mem_query_box_insert(CHATTY_ANY* block, CHATTY_ANY* ref) {
    CHATTY_SIZE index(0);
    CHATTY_QUERY_BOX* r_ref = (CHATTY_QUERY_BOX*)ref;
    CHATTY_QUERY_BLOCK* r_block = (CHATTY_QUERY_BLOCK*)block;
    //std::cout << "- - - - - - - - - - - - - -INSERT ADDR : " << &r_block->value << '\n';

    const CHATTY_SIZE length = r_ref->size;
    CHATTY_SIZE block_size = sizeof(CHATTY_QUERY_BLOCK);
    CHATTY_SIZE box_size = sizeof(CHATTY_QUERY_BOX);
    CHATTY_SIZE now_size = block_size*length;
    CHATTY_SIZE needs_size = block_size*(length+1);

    CHATTY_QUERY_BLOCK** dummy_box = new CHATTY_QUERY_BLOCK*[length+1];
    for(index=0;index<length+1;++index) {
        dummy_box[index] = nullptr;
    };

    if (length != 0) {
        for (index = 0; index < length + 1; ++index) {
            dummy_box[index] = r_ref->value[index];
        };
    };
    dummy_box[length] = r_block;
    delete r_ref->value;
    r_ref->value = dummy_box;
    r_ref->size += 1;
};

CHATTY_ANY chatty_embedded_db_master::__print_query_block(CHATTY_ANY* block_ptr) {
    CHATTY_QUERY_BLOCK* r_ref = (CHATTY_QUERY_BLOCK*)block_ptr;
    if (r_ref->value_size != 0) {
        printf("DEBUG QUERY BLOCK BUFFER PRINT : %s / size : %d\n", r_ref->value, r_ref->value_size);
    } else {
        std::cout << "DEBUG QUERY BLOCK BUFFER PRINT : [IS NULL]" << '\n';
    }
};


CHATTY_ANY chatty_embedded_db_master::__print_query_box(CHATTY_ANY *box_ptr) {
    CHATTY_QUERY_BOX* r_ref = (CHATTY_QUERY_BOX*)box_ptr;
    if (r_ref->size != 0) {
        for(int i=0;i<r_ref->size;++i)
            printf("DEBUG QUERY BOX BUFFER PRINT : No : %d / P-ADDR : %p / V-ADDR : %p /  %s [size : %d]\n", i, &r_ref->value[i], &r_ref->value[i]->value, r_ref->value[i]->value, r_ref->value[i]->value_size);
    } else {
        std::cout << "DEBUG QUERY BOX BUFFER PRINT : [IS NULL]" << '\n';
    };

    std::cout << "- - - BOX LENGTH : " << r_ref->size << '\n';
};




CHATTY_ANY chatty_embedded_db_master::test() {
    CHATTY_QUERY_BOX box;
    CHATTY_QUERY_BLOCK block;

    CHATTY_UCHAR dummy_data[] = "a123456789"; //11
    CHATTY_SIZE dummy_data_size = sizeof(dummy_data) / sizeof(*dummy_data);

    this->__mem_query_add_str(dummy_data, dummy_data_size);
    this->__mem_query_add_str(dummy_data, dummy_data_size);
    this->__mem_query_add_str(dummy_data, dummy_data_size);
    this->__mem_query_add_str(dummy_data, dummy_data_size);
    this->__mem_query_add_str(dummy_data, dummy_data_size);
    this->__mem_query_add_str(dummy_data, dummy_data_size);
    this->__mem_query_add_str(dummy_data, dummy_data_size);
    this->__mem_query_str_flush(&block);
    this->__print_query_block(&block);

    this->__mem_query_box_insert(&block, &box);
    this->__print_query_box(&box);

    this->__print_table_name();
}

CHATTY_ANY chatty_embedded_db_master::__conv_query_quotes(CHATTY_ANY* block, CHATTY_QUOTES_TYPE type) {
    CHATTY_QUERY_BLOCK* cv_block = (CHATTY_QUERY_BLOCK*)block;
    CHATTY_CHAR_PTR copied_block    = (CHATTY_CHAR_PTR)cv_block->value;
    const CHATTY_SIZE copied_size   = (CHATTY_SIZE)cv_block->value_size;

    //early return
    if (copied_size == 0) return;

    CHATTY_INT32 idx(0);
    CHATTY_SIZE movepoint(0);
    CHATTY_FLAG is_ll_detach(false);

    /*
     * 정상적인 시작과 끝인지 확인
     * */
    CHATTY_FLAG it_correctly_open(false), it_correctly_close(false);

    CHATTY_SIZE  quote_point_idx(0);
    CHATTY_UCHAR quote_point_type[copied_size];
    CHATTY_UINT32 quote_point[copied_size];

    //따옴표를 추가하거나 제거함으로 발생하는 변동치를 저장하는 용도
    CHATTY_INT32 pad_n(0);
    CHATTY_INT32 pad_n2(0);


    //for init
    for(idx=0;idx<copied_size;++idx) {
        quote_point[idx]        = 0;
        quote_point_type[idx]   = 0;
    };

    //sort out quotes
    for(idx=0;idx<copied_size;++idx) {
        //std::cout << (CHATTY_UCHAR)copied_block[idx] << '\n';
        if ((CHATTY_UCHAR)copied_block[idx] == (CHATTY_UCHAR)CHATTY_DATA_QUOTE_SINGLE) {
            quote_point[quote_point_idx]        = movepoint;
            quote_point_type[quote_point_idx]   = CHATTY_KEYWORD_QUOTE_TYPE_SINGLE;

            ++quote_point_idx;
            if ((type == CHATTY_KEYWORD_QUOTE_TYPE_SINGLE || type == CHATTY_KEYWORD_QUOTE_TYPE_DOUBLE) &&
                (idx != 0 && idx != copied_size - 1))
                ++pad_n;

            if (type == CHATTY_KEYWORD_QUOTE_TYPE_SINGLE && idx == 0)
                it_correctly_open = true;
            if (type == CHATTY_KEYWORD_QUOTE_TYPE_SINGLE && idx+1 == copied_size-1)
                it_correctly_close = true;
        } else if((CHATTY_UCHAR)copied_block[idx] == (CHATTY_UCHAR)CHATTY_DATA_QUOTE_DOUBLE) {
            quote_point[quote_point_idx]        = movepoint;
            quote_point_type[quote_point_idx]   = CHATTY_KEYWORD_QUOTE_TYPE_DOUBLE;

            ++quote_point_idx;
            if ((type == CHATTY_KEYWORD_QUOTE_TYPE_SINGLE || type == CHATTY_KEYWORD_QUOTE_TYPE_DOUBLE) &&
                (idx != 0 && idx != copied_size - 1))
                ++pad_n;

            if (type == CHATTY_KEYWORD_QUOTE_TYPE_DOUBLE && idx == 0)
                it_correctly_open = true;

            if (type == CHATTY_KEYWORD_QUOTE_TYPE_DOUBLE && idx+1 == copied_size-1)
                it_correctly_close = true;
        };
        ++movepoint;
    };

    //따옴표가 있다면 무언가를 제거한다는 뜻
    if (type == CHATTY_KEYWORD_QUOTE_TYPE_EMPTY && quote_point_idx != 0)
        is_ll_detach = true;


    /*
     * 따옴표 넣어야 하는데 아무것도 없는 경우 좌, 우측에 대상을 넣기위해 크기를 n만큼 확장
     * */
    if (type != CHATTY_KEYWORD_QUOTE_TYPE_EMPTY) {
        if (it_correctly_open && it_correctly_close) {
            //둘다 있거나
            pad_n = 0; //찡긋 히히 봐줘 그냥 넘어가 보지마
        } else if (!it_correctly_open && !it_correctly_close) {
            //둘다 없거나
            pad_n = 2;
        } else {
            //둘중에 하나만 있거나
            pad_n = 1;
        };
    }

    //최종 문자열 공간 생성
    if (is_ll_detach) {
        pad_n = -(pad_n); //플+마=마 법칙으로 음수변환
    };

/*
    std::cout << "기본 사이즈 : " << copied_size-1 << '\n';
    std::cout << "따옴표 변수 : " << pad_n << '\n';
    std::cout << "시작 종료 사이즈 계산 : " << pad_n2 << '\n';
*/

    const CHATTY_SIZE result_size(copied_size - 1 + pad_n + 1 - pad_n2);

    //std::cout << "------------------------" << result_size << '\n';

    //CHATTY_QUERY_BLOCK* dummy_query_block = (CHATTY_QUERY_BLOCK*)malloc(sizeof(CHATTY_QUERY_BLOCK));
    //dummy_query_block->value[5] = {0};

    //어찌됬든 정량 크기로 할당된 스택변수 생성
    //CHATTY_UCHAR* result_str[result_size];
    CHATTY_UCHAR* result_str = (CHATTY_UCHAR*)malloc(sizeof(CHATTY_UCHAR)*result_size);
    for(idx=0;idx<result_size;++idx)
        result_str[idx] = 0;

    memcpy(result_str, copied_block, copied_size-1);

    CHATTY_SIZE buffer_current_size(0);
    CHATTY_SIZE weight_buffer(0);



    if (type == CHATTY_KEYWORD_QUOTE_TYPE_EMPTY) {
        buffer_current_size = this->strlen((unsigned char *) result_str);
        CHATTY_SIZE remove_dummy_idx(0);
        CHATTY_UINT32 n_start_at(0);
        CHATTY_UINT32 n_end_at(0);
        CHATTY_UINT32 nnn(0);

        if ((CHATTY_UINT32) result_str[0] == CHATTY_DATA_QUOTE_SINGLE ||
            (CHATTY_UINT32) result_str[0] == CHATTY_DATA_QUOTE_DOUBLE) {
            n_start_at = 1;
#if defined(CHATTY_DO_BUILD_DEBUG)
            chatty_embedded_db_master::test_print((CHATTY_CHAR_PTR)"first* section quote opened");
#endif
            ++nnn;
        } else {
            n_start_at = 0;
        };

#if defined(CHATTY_DO_BUILD_DEBUG)
        chatty_embedded_db_master::test_print((CHATTY_CHAR_PTR)"it debug for exception that occurred segment error when single character");
        int ____debug_test_1 = buffer_current_size;
        int ____debug_test_2 = (nnn == 0 ? 1 : 0);
        int ____debug_test_3 = ____debug_test_1 - (nnn == 0 ? 1 : 0) - 1;
        std::cout << "LAST QUOTE ARRAY AT : " << ____debug_test_1 << '\n';
#endif

        CHATTY_INT32 last_quote_section_at(buffer_current_size - 1 - (nnn == 0 ? 1 : 0));
        last_quote_section_at = (last_quote_section_at <= 0 ? 0 : last_quote_section_at);
        if ((CHATTY_UINT32) result_str[last_quote_section_at] == CHATTY_DATA_QUOTE_SINGLE ||
            (CHATTY_UINT32) result_str[last_quote_section_at] == CHATTY_DATA_QUOTE_DOUBLE) {
            n_end_at = buffer_current_size - 1 - (nnn == 0 ? 1 : 0);
#if defined(CHATTY_DO_BUILD_DEBUG)
            chatty_embedded_db_master::test_print((CHATTY_CHAR_PTR)"last* section quote opened");
#endif
            ++nnn;
        } else {
            n_end_at = buffer_current_size;
        };

        const CHATTY_SIZE remove_dummy_size(buffer_current_size - nnn +1);
        CHATTY_UCHAR remove_dummy[remove_dummy_size];

        for(idx=0;idx<remove_dummy_size;++idx)
            remove_dummy[idx] = 0;

        for(idx=n_start_at;idx<n_end_at;++idx) {
            remove_dummy[remove_dummy_idx] = (CHATTY_UCHAR)result_str[idx];
            ++remove_dummy_idx;
        };

        for(idx=0;idx<result_size;++idx)
            result_str[idx] = 0;

        for(idx=0;idx<remove_dummy_size;++idx)
            result_str[idx] = remove_dummy[idx];

        result_str[remove_dummy_size] = '\0';
    };



    CHATTY_UINT32 dummy_backslash = CHATTY_DATA_BACKSLASH;
    CHATTY_UINT32 buffer_quotes(CHATTY_DATA_SPACE);
    if (type == CHATTY_KEYWORD_QUOTE_TYPE_SINGLE)
        buffer_quotes = (CHATTY_UINT32)CHATTY_DATA_QUOTE_SINGLE;
    if (type == CHATTY_KEYWORD_QUOTE_TYPE_DOUBLE)
        buffer_quotes = (CHATTY_UINT32)CHATTY_DATA_QUOTE_DOUBLE;

    //std::cout << "TOTALLY SIZE : " << result_size << '\n';

    for (idx = 0; idx < quote_point_idx; ++idx) {
        CHATTY_UINT32 buffer_quotes_at = (CHATTY_UINT32)((quote_point[idx])+weight_buffer);
        buffer_current_size = this->strlen((unsigned char*)result_str);
        if ((buffer_quotes_at==0 || buffer_quotes_at == buffer_current_size-1))
            continue;
        this->__mem_injected_update_data((CHATTY_ANY*)result_str, result_size, buffer_current_size , buffer_quotes_at, (CHATTY_ANY*) &dummy_backslash);
        weight_buffer += 1;
    };

    buffer_current_size = this->strlen((unsigned char*)result_str);
    if (!it_correctly_open && type != CHATTY_KEYWORD_QUOTE_TYPE_EMPTY) {
        //std::cout << "open" << '\n';
        this->__mem_injected_update_data(result_str, result_size, buffer_current_size , 0, &buffer_quotes);
    };

    buffer_current_size = this->strlen((unsigned char*)result_str);
    if (!it_correctly_close && type != CHATTY_KEYWORD_QUOTE_TYPE_EMPTY) {
        //std::cout << "close" << '\n';
        this->__mem_injected_update_data(result_str, result_size, buffer_current_size , buffer_current_size, &buffer_quotes);
    };

    //delete &cv_block->value;


    cv_block->value_size = result_size + weight_buffer;
    cv_block->value = (CHATTY_QUERY_PTR)result_str;

};



/**
 * @param   unsigned char   pallet          참조 문자열 변수입니다
 * @param   unsigned int    pallet_size     널바이트 크기까지 포함된 참조 문자열의 최대 크기입니다
 * @param   unsigned int    current_size    널바이트가 제외된 만큼의 현재 입력된 크기입니다.
 * @param   unsigned int    where_at        끼워넣을 값의 위치입니다.
 * */
CHATTY_ANY chatty_embedded_db_master::__mem_injected_update_data(CHATTY_ANY* pallet, CHATTY_SIZE pallet_size, CHATTY_SIZE current_size, CHATTY_UINT32 where_at_ijt, CHATTY_ANY* ijt_data){
    CHATTY_UCHAR* cv_pallet = (CHATTY_UCHAR*)pallet;
    CHATTY_UCHAR* cv_ijt_data = (CHATTY_UCHAR*)ijt_data;

    const CHATTY_SIZE ijt_data_size = this->strlen((unsigned char*)cv_ijt_data);

    const CHATTY_SIZE left_side_n(where_at_ijt+ijt_data_size+1);
    const CHATTY_SIZE right_side_n(pallet_size-where_at_ijt+1);

    CHATTY_UINT32 idx(0), move_point(0);
    CHATTY_UCHAR left_side[left_side_n];
    CHATTY_UCHAR right_side[right_side_n];

    //std::cout << "ijt_data length!" << "[" << ijt_data_size << "]" << '\n';


    for(idx=0;idx<left_side_n-ijt_data_size-1;++idx) {
        left_side[idx] = cv_pallet[move_point];
        ++move_point;
    };
    for(idx=0;idx<ijt_data_size;++idx) {
        //std::cout << "insert ijt_data!" << "[" << cv_ijt_data[idx] << "]" << '\n';
        left_side[left_side_n-ijt_data_size-1+idx] = cv_ijt_data[idx];
    };
    for(idx=0;idx<right_side_n;++idx) {
        right_side[idx] = cv_pallet[move_point];
        ++move_point;
    };

    move_point = 0;
    for(idx=0;idx<pallet_size;++idx) {
        cv_pallet[idx] = 0;
    };
    for(idx=0;idx<left_side_n-ijt_data_size+ijt_data_size-1;++idx) {
        cv_pallet[move_point] = left_side[idx];
        ++move_point;
    };
    for(idx=0;idx<right_side_n;++idx) {
        cv_pallet[move_point] = right_side[idx];
        ++move_point;
    };

};


