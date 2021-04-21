//
// Created by my_fl on 2021-02-21.
//

#include "chatty_embedded_db_controller.h"
CHATTY_ANY chatty_embedded_db_controller::use(const CHATTY_STRING& filepath) {
    if (this->is_db_standby) {
        chatty_embedded_db_master::test_print((CHATTY_CHAR_PTR)"db connection has been connect");
        return;
    };
    chatty_embedded_db_master::test_print((CHATTY_CHAR_PTR)"open chatty db connection");
    this->latest_error_status = sqlite3_open(filepath.c_str(), &this->selected_db);
    if (this->latest_error_status == SQLITE_OK)
        this->is_db_standby = true;
};

CHATTY_ANY chatty_embedded_db_controller::close() {
    if (!this->is_db_standby) {
        chatty_embedded_db_master::test_print((CHATTY_CHAR_PTR)"db connection has been close");
        return;
    };

    chatty_embedded_db_master::test_print((CHATTY_CHAR_PTR)"finalize chatty handle");
    sqlite3_finalize(this->selected_db_handle);

    chatty_embedded_db_master::test_print((CHATTY_CHAR_PTR)"close chatty db connection");
    this->latest_error_status = sqlite3_close(this->selected_db);
    if (this->latest_error_status == CHATTY_STATUS_OK)
        this->is_db_standby = false;


};

CHATTY_ANY chatty_embedded_db_controller::select(CHATTY_UCHAR_PTR table) {
    //this->query_form.target_table_name
    CHATTY_QUERY_BLOCK o;
    CHATTY_SIZE len = this->strlen(table);
    this->__mem_query_add_str(table, len);
    this->__mem_query_str_flush(&o);
    this->query_form.target_table_name = o; //ref-by-val?
}
/**
 * CHATTY_KEYWORD_REQUEST_NULL
 * CHATTY_KEYWORD_REQUEST_SELECT
 * CHATTY_KEYWORD_REQUEST_INSERT
 * CHATTY_KEYWORD_REQUEST_UPDATE
 * CHATTY_KEYWORD_REQUEST_DELETE
 * */
CHATTY_ANY chatty_embedded_db_controller::behavior(CHATTY_UCHAR type) {
    if ((CHATTY_UINT32)this->query_form.request_type == CHATTY_KEYWORD_REQUEST_NULL) {
        switch (static_cast<CHATTY_UINT32>(type)) {
            case (CHATTY_UINT32) CHATTY_KEYWORD_REQUEST_SELECT:
                this->query_form.request_type = CHATTY_KEYWORD_REQUEST_SELECT;
                break;
            case (CHATTY_UINT32) CHATTY_KEYWORD_REQUEST_INSERT:
                this->query_form.request_type = CHATTY_KEYWORD_REQUEST_INSERT;
                break;
            case (CHATTY_UINT32) CHATTY_KEYWORD_REQUEST_UPDATE:
                this->query_form.request_type = CHATTY_KEYWORD_REQUEST_UPDATE;
                break;
            case (CHATTY_UINT32) CHATTY_KEYWORD_REQUEST_DELETE:
                this->query_form.request_type = CHATTY_KEYWORD_REQUEST_DELETE;
                break;
            case (CHATTY_UINT32) CHATTY_KEYWORD_REQUEST_NULL:
                this->query_form.request_type = CHATTY_KEYWORD_REQUEST_NULL;
                break;
            default: //exception
                this->query_form.request_type = CHATTY_KEYWORD_REQUEST_NULL;
                break;
        };
    } else {
        chatty_embedded_db_master::test_print((CHATTY_CHAR_PTR)"ERROR OCCURRED - Duplicated behavior change");
        printf("by [%s] function on %s:%d\n",__FUNCTION__ ,__FILE__, __LINE__);
        abort();
    };
};

CHATTY_ANY chatty_embedded_db_controller::column(CHATTY_UCHAR_PTR column) {
    CHATTY_INT32 idx(0);

    CHATTY_UCHAR value_c = '\0';
    const CHATTY_UCHAR_PTR value = &value_c;

    CHATTY_QUERY_BLOCK* column_box = (CHATTY_QUERY_BLOCK*)malloc(sizeof(CHATTY_QUERY_BLOCK));
    const CHATTY_SIZE column_size = this->strlen(column)+1;
    CHATTY_CHAR column_of_part[column_size]; //null byte calculated

    //init
    for(idx=0;idx<column_size;++idx)
        column_of_part[idx] = 0;
    //value-insert
    for(idx=0;idx<column_size;++idx)
        column_of_part[idx] = column[idx];

    column_box->value = column_of_part;
    column_box->value_size = column_size;
    column_box->join_keyword = CHATTY_KEYWORD_JOIN_NULL;

    this->__conv_query_quotes(column_box, CHATTY_KEYWORD_QUOTE_TYPE_EMPTY);
    this->__mem_query_box_insert(column_box, &this->query_form.column_part_key);
#if defined(CHATTY_DO_BUILD_DEBUG)
    this->__print_query_box(&this->query_form.column_part_key);
#endif
    CHATTY_QUERY_BLOCK* value_box = (CHATTY_QUERY_BLOCK*)malloc(sizeof(CHATTY_QUERY_BLOCK));
    const CHATTY_SIZE value_size = this->strlen(value)+1;
    CHATTY_CHAR value_of_part[value_size]; //null byte calculated

    //init
    for(idx=0;idx<value_size;++idx)
        value_of_part[idx] = 0;
    //value-insert
    for(idx=0;idx<value_size;++idx)
        value_of_part[idx] = value[idx];

    value_box->value = value_of_part;
    value_box->value_size = value_size;
    value_box->join_keyword = CHATTY_KEYWORD_JOIN_NULL;
    this->__conv_query_quotes(value_box, CHATTY_KEYWORD_QUOTE_TYPE_DOUBLE);
    this->__mem_query_box_insert(value_box, &this->query_form.column_part);
#if defined(CHATTY_DO_BUILD_DEBUG)
    this->__print_query_box(&this->query_form.column_part);
#endif
};

CHATTY_ANY chatty_embedded_db_controller::column(CHATTY_UCHAR_PTR column, CHATTY_UCHAR_PTR value) {
    CHATTY_INT32 idx(0);

    CHATTY_QUERY_BLOCK* column_box = (CHATTY_QUERY_BLOCK*)malloc(sizeof(CHATTY_QUERY_BLOCK));
    const CHATTY_SIZE column_size = this->strlen(column)+1;
    CHATTY_CHAR column_of_part[column_size]; //null byte calculated

    //init
    for(idx=0;idx<column_size;++idx)
        column_of_part[idx] = 0;
    //value-insert
    for(idx=0;idx<column_size;++idx)
        column_of_part[idx] = column[idx];

    column_box->value = column_of_part;
    column_box->value_size = column_size;
    column_box->join_keyword = CHATTY_KEYWORD_JOIN_NULL;

    this->__conv_query_quotes(column_box, CHATTY_KEYWORD_QUOTE_TYPE_EMPTY);
    this->__mem_query_box_insert(column_box, &this->query_form.column_part_key);
#if defined(CHATTY_DO_BUILD_DEBUG)
    this->__print_query_box(&this->query_form.column_part_key);
#endif
    CHATTY_QUERY_BLOCK* value_box = (CHATTY_QUERY_BLOCK*)malloc(sizeof(CHATTY_QUERY_BLOCK));
    const CHATTY_SIZE value_size = this->strlen(value)+1;
    CHATTY_CHAR value_of_part[value_size]; //null byte calculated

    //init
    for(idx=0;idx<value_size;++idx)
        value_of_part[idx] = 0;
    //value-insert
    for(idx=0;idx<value_size;++idx)
        value_of_part[idx] = value[idx];

    value_box->value = value_of_part;
    value_box->value_size = value_size;
    value_box->join_keyword = CHATTY_KEYWORD_JOIN_NULL;

    this->__conv_query_quotes(value_box, CHATTY_KEYWORD_QUOTE_TYPE_DOUBLE);
    this->__mem_query_box_insert(value_box, &this->query_form.column_part);
#if defined(CHATTY_DO_BUILD_DEBUG)
    this->__print_query_box(&this->query_form.column_part);
#endif
};

CHATTY_ANY chatty_embedded_db_controller::where_and(CHATTY_UCHAR_PTR column, CHATTY_UCHAR_PTR value) {
    CHATTY_INT32 idx(0);

    CHATTY_QUERY_BLOCK* column_box = (CHATTY_QUERY_BLOCK*)malloc(sizeof(CHATTY_QUERY_BLOCK));
    const CHATTY_SIZE column_size = this->strlen(column)+1;
    CHATTY_CHAR column_of_part[column_size]; //null byte calculated

    //init
    for(idx=0;idx<column_size;++idx)
        column_of_part[idx] = 0;
    //value-insert
    for(idx=0;idx<column_size;++idx)
        column_of_part[idx] = column[idx];

    column_box->value = column_of_part;
    column_box->value_size = column_size;
    column_box->join_keyword = CHATTY_KEYWORD_JOIN_NULL;

    this->__conv_query_quotes(column_box, CHATTY_KEYWORD_QUOTE_TYPE_EMPTY);
    this->__mem_query_box_insert(column_box, &this->query_form.filter_part_key);
#if defined(CHATTY_DO_BUILD_DEBUG)
    this->__print_query_box(&this->query_form.filter_part_key);
#endif

    CHATTY_QUERY_BLOCK* value_box = (CHATTY_QUERY_BLOCK*)malloc(sizeof(CHATTY_QUERY_BLOCK));
    const CHATTY_SIZE value_size = this->strlen(value)+1;
    CHATTY_CHAR value_of_part[value_size]; //null byte calculated

    //init
    for(idx=0;idx<value_size;++idx)
        value_of_part[idx] = 0;
    //value-insert
    for(idx=0;idx<value_size;++idx)
        value_of_part[idx] = value[idx];

    value_box->value = value_of_part;
    value_box->value_size = value_size;
    value_box->join_keyword = CHATTY_KEYWORD_JOIN_AND;

    this->__conv_query_quotes(value_box, CHATTY_KEYWORD_QUOTE_TYPE_DOUBLE);
    this->__mem_query_box_insert(value_box, &this->query_form.filter_part);
#if defined(CHATTY_DO_BUILD_DEBUG)
    this->__print_query_box(&this->query_form.filter_part);
#endif
};

CHATTY_ANY chatty_embedded_db_controller::where_or(CHATTY_UCHAR_PTR column, CHATTY_UCHAR_PTR value) {
    CHATTY_INT32 idx(0);

    CHATTY_QUERY_BLOCK* column_box = (CHATTY_QUERY_BLOCK*)malloc(sizeof(CHATTY_QUERY_BLOCK));
    const CHATTY_SIZE column_size = this->strlen(column)+1;
    CHATTY_CHAR column_of_part[column_size]; //null byte calculated

    //init
    for(idx=0;idx<column_size;++idx)
        column_of_part[idx] = 0;
    //value-insert
    for(idx=0;idx<column_size;++idx)
        column_of_part[idx] = column[idx];

    column_box->value = column_of_part;
    column_box->value_size = column_size;
    column_box->join_keyword = CHATTY_KEYWORD_JOIN_NULL;

    this->__conv_query_quotes(column_box, CHATTY_KEYWORD_QUOTE_TYPE_EMPTY);
    this->__mem_query_box_insert(column_box, &this->query_form.filter_part_key);
#if defined(CHATTY_DO_BUILD_DEBUG)
    this->__print_query_box(&this->query_form.filter_part_key);
#endif
    CHATTY_QUERY_BLOCK* value_box = (CHATTY_QUERY_BLOCK*)malloc(sizeof(CHATTY_QUERY_BLOCK));
    const CHATTY_SIZE value_size = this->strlen(value)+1;
    CHATTY_CHAR value_of_part[value_size]; //null byte calculated

    //init
    for(idx=0;idx<value_size;++idx)
        value_of_part[idx] = 0;
    //value-insert
    for(idx=0;idx<value_size;++idx)
        value_of_part[idx] = value[idx];

    value_box->value = value_of_part;
    value_box->value_size = value_size;
    value_box->join_keyword = CHATTY_KEYWORD_JOIN_OR;

    this->__conv_query_quotes(value_box, CHATTY_KEYWORD_QUOTE_TYPE_DOUBLE);
    this->__mem_query_box_insert(value_box, &this->query_form.filter_part);
#if defined(CHATTY_DO_BUILD_DEBUG)
    this->__print_query_box(&this->query_form.filter_part);
#endif
};

CHATTY_ANY chatty_embedded_db_controller::handler() {
    //CHATTY_DB_LIB_STMT
};

CHATTY_ANY chatty_embedded_db_controller::handle_finalize() {
    sqlite3_finalize(this->selected_db_handle);
}

CHATTY_ANY chatty_embedded_db_controller::_generate_where_common() {
    CHATTY_SIZE idx(0);
    if (query_form.filter_part_key.size != 0) {
        this->__mem_query_add_str((CHATTY_UCHAR_PTR) CHATTY_CONST_KEYWORD_SPACE_LATTER, CHATTY_CONST_KEYWORD_SIZE_SPACE_LATTER);
        this->__mem_query_add_str((CHATTY_UCHAR_PTR) CHATTY_CONST_KEYWORD_WHERE, CHATTY_CONST_KEYWORD_SIZE_WHERE);
        this->__mem_query_add_str((CHATTY_UCHAR_PTR) CHATTY_CONST_KEYWORD_SPACE_LATTER, CHATTY_CONST_KEYWORD_SIZE_SPACE_LATTER);
        for(idx=0;idx<this->query_form.filter_part_key.size;++idx) {
            this->__mem_query_add_str((CHATTY_UCHAR_PTR)this->query_form.filter_part_key.value[idx]->value, this->query_form.filter_part_key.value[idx]->value_size);
            this->__mem_query_add_str((CHATTY_UCHAR_PTR)CHATTY_CONST_KEYWORD_EQUAL_LATTER, CHATTY_CONST_KEYWORD_SIZE_EQUAL_LATTER);
            this->__mem_query_add_str((CHATTY_UCHAR_PTR)this->query_form.filter_part.value[idx]->value, this->query_form.filter_part.value[idx]->value_size);
            if (idx != this->query_form.filter_part_key.size-1) {
                this->__mem_query_add_str((CHATTY_UCHAR_PTR) CHATTY_CONST_KEYWORD_SPACE_LATTER, CHATTY_CONST_KEYWORD_SIZE_SPACE_LATTER);
                if ((CHATTY_UINT32)this->query_form.filter_part.value[idx]->join_keyword == CHATTY_KEYWORD_JOIN_AND) {
                    this->__mem_query_add_str((CHATTY_UCHAR_PTR) CHATTY_CONST_KEYWORD_AND, CHATTY_KEYWORD_JOIN_SIZE_AND);
                } else {
                    this->__mem_query_add_str((CHATTY_UCHAR_PTR) CHATTY_CONST_KEYWORD_OR, CHATTY_KEYWORD_JOIN_SIZE_OR);
                };
                this->__mem_query_add_str((CHATTY_UCHAR_PTR) CHATTY_CONST_KEYWORD_SPACE_LATTER, CHATTY_CONST_KEYWORD_SIZE_SPACE_LATTER);
            };
        };
    };
}

CHATTY_ANY chatty_embedded_db_controller::_generate_select_column_common() {
    CHATTY_SIZE idx(0);
    if (query_form.column_part_key.size != 0) {
        this->__mem_query_add_str((CHATTY_UCHAR_PTR) CHATTY_CONST_KEYWORD_SPACE_LATTER, CHATTY_CONST_KEYWORD_SIZE_SPACE_LATTER);
        for(idx=0;idx<this->query_form.column_part_key.size;++idx) {
            this->__mem_query_add_str((CHATTY_UCHAR_PTR) this->query_form.column_part_key.value[idx]->value, this->query_form.column_part_key.value[idx]->value_size);
            if (idx != this->query_form.column_part_key.size-1)
                this->__mem_query_add_str((CHATTY_UCHAR_PTR) CHATTY_CONST_KEYWORD_SPOT_LATTER, CHATTY_CONST_KEYWORD_SIZE_SPOT_LATTER);
        };
    };
}

CHATTY_ANY chatty_embedded_db_controller::_generate_non_value_grouping_column_key() {
    CHATTY_SIZE idx(0);
    if (query_form.column_part_key.size != 0) {
        this->__mem_query_add_str((CHATTY_UCHAR_PTR) CHATTY_CONST_KEYWORD_SPACE_LATTER, CHATTY_CONST_KEYWORD_SIZE_SPACE_LATTER);
        this->__mem_query_add_str((CHATTY_UCHAR_PTR) CHATTY_CONST_KEYWORD_GROUP_OPEN_LATTER, CHATTY_CONST_KEYWORD_SIZE_OPEN_LATTER);
        for(idx=0;idx<this->query_form.column_part_key.size;++idx) {
            this->__mem_query_add_str((CHATTY_UCHAR_PTR) this->query_form.column_part_key.value[idx]->value, this->query_form.column_part_key.value[idx]->value_size);
            if (idx != this->query_form.column_part_key.size-1)
                this->__mem_query_add_str((CHATTY_UCHAR_PTR) CHATTY_CONST_KEYWORD_SPOT_LATTER, CHATTY_CONST_KEYWORD_SIZE_SPOT_LATTER);
        };
        this->__mem_query_add_str((CHATTY_UCHAR_PTR) CHATTY_CONST_KEYWORD_GROUP_CLOSE_LATTER, CHATTY_CONST_KEYWORD_SIZE_CLOSE_LATTER);
    };
}

CHATTY_ANY chatty_embedded_db_controller::_generate_non_value_grouping_column() {
    CHATTY_SIZE idx(0);
    if (query_form.column_part.size != 0) {
        this->__mem_query_add_str((CHATTY_UCHAR_PTR) CHATTY_CONST_KEYWORD_SPACE_LATTER, CHATTY_CONST_KEYWORD_SIZE_SPACE_LATTER);
        this->__mem_query_add_str((CHATTY_UCHAR_PTR) CHATTY_CONST_KEYWORD_GROUP_OPEN_LATTER, CHATTY_CONST_KEYWORD_SIZE_OPEN_LATTER);
        for(idx=0;idx<this->query_form.column_part.size;++idx) {
            this->__mem_query_add_str((CHATTY_UCHAR_PTR) this->query_form.column_part.value[idx]->value, this->query_form.column_part.value[idx]->value_size);
            if (idx != this->query_form.column_part.size-1)
                this->__mem_query_add_str((CHATTY_UCHAR_PTR) CHATTY_CONST_KEYWORD_SPOT_LATTER, CHATTY_CONST_KEYWORD_SIZE_SPOT_LATTER);
        };
        this->__mem_query_add_str((CHATTY_UCHAR_PTR) CHATTY_CONST_KEYWORD_GROUP_CLOSE_LATTER, CHATTY_CONST_KEYWORD_SIZE_CLOSE_LATTER);
    };

}


CHATTY_ERROR_CODE chatty_embedded_db_controller::exec(CHATTY_FLAG row_proceed) {
    CHATTY_QUERY_BLOCK result_query_stack_block;

    switch((CHATTY_UINT32)this->query_form.request_type) {
        case (CHATTY_UINT32)CHATTY_KEYWORD_REQUEST_SELECT:
            this->__mem_query_add_str((CHATTY_UCHAR_PTR) CHATTY_CONST_KEYWORD_SELECT, CHATTY_CONST_KEYWORD_SIZE_SELECT);
            this->_generate_select_column_common();
            this->__mem_query_add_str((CHATTY_UCHAR_PTR) CHATTY_CONST_KEYWORD_SPACE_LATTER, CHATTY_CONST_KEYWORD_SIZE_SPACE_LATTER);
            this->__mem_query_add_str((CHATTY_UCHAR_PTR) CHATTY_CONST_KEYWORD_FROM, CHATTY_CONST_KEYWORD_SIZE_FROM);
            this->__mem_query_add_str((CHATTY_UCHAR_PTR) CHATTY_CONST_KEYWORD_SPACE_LATTER, CHATTY_CONST_KEYWORD_SIZE_SPACE_LATTER);
            this->__mem_query_add_str((CHATTY_UCHAR_PTR) this->query_form.target_table_name.value, this->query_form.target_table_name.value_size);
            this->_generate_where_common();
            break;
        case (CHATTY_UINT32)CHATTY_KEYWORD_REQUEST_INSERT:
            this->__mem_query_add_str((CHATTY_UCHAR_PTR) CHATTY_CONST_KEYWORD_INSERT, CHATTY_CONST_KEYWORD_SIZE_INSERT);
            this->__mem_query_add_str((CHATTY_UCHAR_PTR) CHATTY_CONST_KEYWORD_SPACE_LATTER, CHATTY_CONST_KEYWORD_SIZE_SPACE_LATTER);
            this->__mem_query_add_str((CHATTY_UCHAR_PTR) CHATTY_CONST_KEYWORD_INTO, CHATTY_CONST_KEYWORD_SIZE_INTO);
            this->__mem_query_add_str((CHATTY_UCHAR_PTR) CHATTY_CONST_KEYWORD_SPACE_LATTER, CHATTY_CONST_KEYWORD_SIZE_SPACE_LATTER);
            this->__mem_query_add_str((CHATTY_UCHAR_PTR) this->query_form.target_table_name.value, this->query_form.target_table_name.value_size);
            this->_generate_non_value_grouping_column_key();
            if (query_form.column_part_key.size != 0 && query_form.column_part.size != 0) {
                this->__mem_query_add_str((CHATTY_UCHAR_PTR) CHATTY_CONST_KEYWORD_SPACE_LATTER, CHATTY_CONST_KEYWORD_SIZE_SPACE_LATTER);
                this->__mem_query_add_str((CHATTY_UCHAR_PTR) CHATTY_CONST_KEYWORD_VALUES, CHATTY_CONST_KEYWORD_SIZE_VALUES);
            };
            this->_generate_non_value_grouping_column();
            break;
        case (CHATTY_UINT32)CHATTY_KEYWORD_REQUEST_UPDATE:
            this->__mem_query_add_str((CHATTY_UCHAR_PTR) CHATTY_CONST_KEYWORD_UPDATE, CHATTY_CONST_KEYWORD_SIZE_UPDATE);
            break;
        case (CHATTY_UINT32)CHATTY_KEYWORD_REQUEST_DELETE:
            this->__mem_query_add_str((CHATTY_UCHAR_PTR) CHATTY_CONST_KEYWORD_DELETE, CHATTY_CONST_KEYWORD_SIZE_DELETE);
            break;
    };
    this->__mem_query_str_flush(&result_query_stack_block);

#if defined(CHATTY_DO_BUILD_DEBUG)
    std::cout << std::endl;
    std::cout << "[ " << "@@ QUERY @@" << " ]" << '\n';
    std::cout << "# TABLE NAME : " << this->query_form.target_table_name.value << "  sz(" << this->query_form.target_table_name.value_size << ")" << '\n';
    std::cout << "# BEHAVIOR : " << (unsigned int)this->query_form.request_type << '\n';
    std::cout << "# QUERY : " << result_query_stack_block.value << "#(" << result_query_stack_block.value_size << "/" << 0 << ")" << '\n';
    std::cout << std::endl;
#endif

    if (sqlite3_prepare_v2(this->selected_db, (CHATTY_CHAR_PTR)result_query_stack_block.value, -1, &this->selected_db_handle, nullptr) != CHATTY_STATUS_OK) {
        #if defined(CHATTY_DO_BUILD_DEBUG)
        chatty_embedded_db_master::test_print((CHATTY_CHAR_PTR)"prepare_v2 failed");
        #endif
        this->__update_query_execution_status();
        sqlite3_finalize(this->selected_db_handle);
        return this->db_error_code;;
    } else {
        switch((CHATTY_UINT32)this->query_form.request_type) {
            case CHATTY_KEYWORD_REQUEST_SELECT:{
                if (sqlite3_step(this->selected_db_handle) != CHATTY_STATUS_ROW) {
#if defined(CHATTY_DO_BUILD_DEBUG)
                    chatty_embedded_db_master::test_print((CHATTY_CHAR_PTR)"step failed - select");
#endif
                    this->__update_query_execution_status();
                    sqlite3_finalize(this->selected_db_handle);
                    return this->db_error_code;
                } else {
                    sqlite3_column_text(this->selected_db_handle,0);
                    sqlite3_finalize(this->selected_db_handle);
                    return CHATTY_STATUS_ROW;
                };
            };
            default:{
                if (sqlite3_step(this->selected_db_handle) != CHATTY_STATUS_DONE) {
#if defined(CHATTY_DO_BUILD_DEBUG)
                    chatty_embedded_db_master::test_print((CHATTY_CHAR_PTR)"step failed - default");
#endif
                    this->__update_query_execution_status();
                    sqlite3_finalize(this->selected_db_handle);
                    return this->db_error_code;
                } else {
                    sqlite3_finalize(this->selected_db_handle);
                    return CHATTY_STATUS_OK;
                };
            };
        };


    };
}



CHATTY_ANY chatty_embedded_db_controller::_generate_query_block(CHATTY_ANY *block) {
    CHATTY_QUERY_BLOCK* rf_block = (CHATTY_QUERY_BLOCK*)block;
    CHATTY_QUERY_BLOCK result_query_stack_block;

    switch((CHATTY_UINT32)this->query_form.request_type) {
        case (CHATTY_UINT32)CHATTY_KEYWORD_REQUEST_SELECT:
            this->__mem_query_add_str((CHATTY_UCHAR_PTR) CHATTY_CONST_KEYWORD_SELECT, CHATTY_CONST_KEYWORD_SIZE_SELECT);
            this->_generate_select_column_common();
            this->__mem_query_add_str((CHATTY_UCHAR_PTR) CHATTY_CONST_KEYWORD_SPACE_LATTER, CHATTY_CONST_KEYWORD_SIZE_SPACE_LATTER);
            this->__mem_query_add_str((CHATTY_UCHAR_PTR) CHATTY_CONST_KEYWORD_FROM, CHATTY_CONST_KEYWORD_SIZE_FROM);
            this->__mem_query_add_str((CHATTY_UCHAR_PTR) CHATTY_CONST_KEYWORD_SPACE_LATTER, CHATTY_CONST_KEYWORD_SIZE_SPACE_LATTER);
            this->__mem_query_add_str((CHATTY_UCHAR_PTR) this->query_form.target_table_name.value, this->query_form.target_table_name.value_size);
            this->_generate_where_common();
            break;
        case (CHATTY_UINT32)CHATTY_KEYWORD_REQUEST_INSERT:
            this->__mem_query_add_str((CHATTY_UCHAR_PTR) CHATTY_CONST_KEYWORD_INSERT, CHATTY_CONST_KEYWORD_SIZE_INSERT);
            this->__mem_query_add_str((CHATTY_UCHAR_PTR) CHATTY_CONST_KEYWORD_SPACE_LATTER, CHATTY_CONST_KEYWORD_SIZE_SPACE_LATTER);
            this->__mem_query_add_str((CHATTY_UCHAR_PTR) CHATTY_CONST_KEYWORD_INTO, CHATTY_CONST_KEYWORD_SIZE_INTO);
            this->__mem_query_add_str((CHATTY_UCHAR_PTR) CHATTY_CONST_KEYWORD_SPACE_LATTER, CHATTY_CONST_KEYWORD_SIZE_SPACE_LATTER);
            this->__mem_query_add_str((CHATTY_UCHAR_PTR) this->query_form.target_table_name.value, this->query_form.target_table_name.value_size);
            this->_generate_non_value_grouping_column_key();
            if (query_form.column_part_key.size != 0 && query_form.column_part.size != 0) {
                this->__mem_query_add_str((CHATTY_UCHAR_PTR) CHATTY_CONST_KEYWORD_SPACE_LATTER, CHATTY_CONST_KEYWORD_SIZE_SPACE_LATTER);
                this->__mem_query_add_str((CHATTY_UCHAR_PTR) CHATTY_CONST_KEYWORD_VALUES, CHATTY_CONST_KEYWORD_SIZE_VALUES);
            };
            this->_generate_non_value_grouping_column();
            break;
        case (CHATTY_UINT32)CHATTY_KEYWORD_REQUEST_UPDATE:
            this->__mem_query_add_str((CHATTY_UCHAR_PTR) CHATTY_CONST_KEYWORD_UPDATE, CHATTY_CONST_KEYWORD_SIZE_UPDATE);
            break;
        case (CHATTY_UINT32)CHATTY_KEYWORD_REQUEST_DELETE:
            this->__mem_query_add_str((CHATTY_UCHAR_PTR) CHATTY_CONST_KEYWORD_DELETE, CHATTY_CONST_KEYWORD_SIZE_DELETE);
            break;
    };
    this->__mem_query_str_flush(&result_query_stack_block);

    rf_block->value = result_query_stack_block.value;
    rf_block->value_size = result_query_stack_block.value_size;
    rf_block->join_keyword = result_query_stack_block.join_keyword;
};




/**
 * @param   group    CHATTY_DB_COLUMN_CONNECTIO***  structure heap 의 reference 의 참조 pointer
 * */
CHATTY_FLAG chatty_embedded_db_controller::fetchall_connection(CHATTY_ANY* group) {
    CHATTY_DB_COLUMN_CONNECTION_GROUP_COLLECTION* rf_collection = (CHATTY_DB_COLUMN_CONNECTION_GROUP_COLLECTION*)group;
    CHATTY_SIZE idx(0);
    CHATTY_SIZE row_length(0);
    CHATTY_QUERY_BLOCK _block;
#if defined(CHATTY_DO_BUILD_DEBUG)
    chatty_embedded_db_master::test_print((CHATTY_CHAR_PTR)"internal fetchall function");
#endif
    this->_generate_query_block(&_block);

    if (sqlite3_prepare_v2(this->selected_db, (CHATTY_CHAR_PTR)_block.value, -1, &this->selected_db_handle, nullptr) != CHATTY_STATUS_OK) {
        sqlite3_finalize(this->selected_db_handle);
        return false;
    } else {
        CHATTY_INT32 status(1);
        while (status = sqlite3_step(this->selected_db_handle) == CHATTY_STATUS_ROW)
        {
            if (status != CHATTY_STATUS_ROW)
                return false;

            ++row_length;
            CHATTY_DB_COLUMN_CONNECTION** new_group = (CHATTY_DB_COLUMN_CONNECTION**)malloc(sizeof(CHATTY_DB_COLUMN_CONNECTION**) * row_length);
            CHATTY_DB_COLUMN_CONNECTION* new_heap = (CHATTY_DB_COLUMN_CONNECTION*)malloc(sizeof(CHATTY_DB_COLUMN_CONNECTION));
            new_heap->idx                   = sqlite3_column_int64(this->selected_db_handle, 0);
            new_heap->id                    = sqlite3_column_int64(this->selected_db_handle, 1);
            new_heap->latest_connect_date   = sqlite3_column_int64(this->selected_db_handle, 2);
            new_heap->is_online             = sqlite3_column_int(this->selected_db_handle, 3);
            //group initialization
            for(idx=0;idx<row_length;++idx)
                new_group[idx] = nullptr;
            //최초 실행만 거름
            if (row_length-1 != 0) {
                //re-fill history
                for(idx=0;idx<row_length;++idx)
                    new_group[idx] = rf_collection->value[idx];
            };
            //group fill-up
            new_group[row_length-1] = new_heap;
            delete rf_collection->value;
            rf_collection->value = new_group;
        };

        CHATTY_SIZE* new_size = (CHATTY_SIZE*)malloc(sizeof(CHATTY_SIZE));
        *new_size = row_length;
        delete rf_collection->size;
        rf_collection->size = new_size;

#if defined(CHATTY_DO_BUILD_DEBUG)
        chatty_embedded_db_master::test_print((CHATTY_CHAR_PTR)"PRINT TEST FETCH ALL RESULT");
        std::cout << "ROWS ARE TOTALLY EXISTS : " << row_length << '\n';
        for(idx=0;idx<row_length;++idx) {
            std::cout << rf_collection->value[idx] << " / " << rf_collection->value[idx]->idx << " / " << rf_collection->value[idx]->id << " / " << rf_collection->value[idx]->latest_connect_date << " / " << rf_collection->value[idx]->is_online << '\n';
        };
#endif

    };

    delete _block.value;
    return true;
};

CHATTY_FLAG chatty_embedded_db_controller::fetchone_connection(CHATTY_ANY *article) {
    CHATTY_DB_COLUMN_CONNECTION* rf_article = (CHATTY_DB_COLUMN_CONNECTION*)article;
    CHATTY_QUERY_BLOCK _block;

#if defined(CHATTY_DO_BUILD_DEBUG)
    chatty_embedded_db_master::test_print((CHATTY_CHAR_PTR)"internal fetchone function");
#endif

    this->_generate_query_block(&_block);
    if (sqlite3_prepare_v2(this->selected_db, (CHATTY_CHAR_PTR)_block.value, -1, &this->selected_db_handle, nullptr) != CHATTY_STATUS_OK) {
        sqlite3_finalize(this->selected_db_handle);
        return false;
    } else {
        while (sqlite3_step(this->selected_db_handle) == CHATTY_STATUS_ROW)
        {
            rf_article->idx                   = sqlite3_column_int64(this->selected_db_handle, 0);
            rf_article->id                    = sqlite3_column_int64(this->selected_db_handle, 1);
            rf_article->latest_connect_date   = sqlite3_column_int64(this->selected_db_handle, 2);
            rf_article->is_online             = sqlite3_column_int(this->selected_db_handle, 3);
            return true;
        };
    };
    delete _block.value;
    return false;
};


CHATTY_FLAG chatty_embedded_db_controller::fetchall_connection_release(CHATTY_ANY *pointer) {
    CHATTY_SIZE idx(0);
    CHATTY_DB_COLUMN_CONNECTION_GROUP_COLLECTION* rf_pointer = (CHATTY_DB_COLUMN_CONNECTION_GROUP_COLLECTION*)pointer;

#if defined(CHATTY_DO_BUILD_DEBUG)
    chatty_embedded_db_master::test_print((CHATTY_CHAR_PTR)"RELEASE GROUP COLLECTION");
    std::cout << "RELEASE TARGET COUNT : " << *rf_pointer->size << '\n';
#endif

    for(idx=0;idx<*rf_pointer->size;++idx) {
#if defined(CHATTY_DO_BUILD_DEBUG)
        std::cout << "DELETE A VALUE : " << rf_pointer->value[idx] << '\n';
#endif
        delete rf_pointer->value[idx];
    };
#if defined(CHATTY_DO_BUILD_DEBUG)
    std::cout << "DELETE A SIZE : " << rf_pointer->size << '\n';
#endif
    delete rf_pointer->size;

    return false;
}


CHATTY_FLAG chatty_embedded_db_controller::initialization_table_connection() {
    return 0;
}


