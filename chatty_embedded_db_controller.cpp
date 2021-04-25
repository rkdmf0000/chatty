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


CHATTY_ERROR_CODE chatty_embedded_db_controller::exec() {
    CHATTY_QUERY_BLOCK _block;
    this->_generate_query_block(&_block);

#if defined(CHATTY_DO_BUILD_DEBUG)
    std::cout << std::endl;
    std::cout << "[ " << "@@ QUERY @@" << " ]" << '\n';
    std::cout << "# TABLE NAME : " << this->query_form.target_table_name.value << "  sz(" << this->query_form.target_table_name.value_size << ")" << '\n';
    std::cout << "# BEHAVIOR : " << (unsigned int)this->query_form.request_type << '\n';
    std::cout << "# QUERY : " << _block.value << "#(" << _block.value_size << "/" << 0 << ")" << '\n';
    std::cout << std::endl;
#endif


    if (sqlite3_prepare_v2(this->selected_db, (CHATTY_CHAR_PTR)_block.value, -1, &this->selected_db_handle, nullptr) != CHATTY_STATUS_OK) {
        #if defined(CHATTY_DO_BUILD_DEBUG)
        chatty_embedded_db_master::test_print((CHATTY_CHAR_PTR)"prepare_v2 failed");
        #endif
        this->__update_query_execution_status();
        sqlite3_finalize(this->selected_db_handle);
        delete _block.value;
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
                    delete _block.value;
                    return this->db_error_code;
                } else {
                    sqlite3_column_text(this->selected_db_handle,0);
                    sqlite3_finalize(this->selected_db_handle);
                    delete _block.value;
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
                    delete _block.value;
                    return this->db_error_code;
                } else {
                    this->__update_query_execution_status();
                    sqlite3_finalize(this->selected_db_handle);
                    delete _block.value;
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

    std::cout << "-------- QUERY : " << rf_block->value << '\n';

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

    this->__update_query_execution_status();
    sqlite3_finalize(this->selected_db_handle);
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
        this->__update_query_execution_status();
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
    this->__update_query_execution_status();
    sqlite3_finalize(this->selected_db_handle);
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



CHATTY_ERROR_CODE chatty_embedded_db_controller::query(CHATTY_CHAR_PTR query) {

    if (sqlite3_prepare_v2(this->selected_db, query, -1, &this->selected_db_handle,nullptr) != CHATTY_STATUS_OK) {
#if defined(CHATTY_DO_BUILD_DEBUG)
        chatty_embedded_db_master::test_print((CHATTY_CHAR_PTR)"CHATTY DB INIT PREPARE FAILED");
#endif
        sqlite3_finalize(this->selected_db_handle);
        this->__update_query_execution_status();
        return CHATTY_STATUS_ERROR;
    } else {
        if (sqlite3_step(this->selected_db_handle) != CHATTY_STATUS_OK) {
#if defined(CHATTY_DO_BUILD_DEBUG)
            chatty_embedded_db_master::test_print((CHATTY_CHAR_PTR)"CHATTY DB INIT STEP FAILED");
#endif
            sqlite3_finalize(this->selected_db_handle);
            this->__update_query_execution_status();
            return CHATTY_STATUS_ERROR;
        } else {
            sqlite3_finalize(this->selected_db_handle);
            this->__update_query_execution_status();
            return CHATTY_STATUS_OK;
        };
    };
};

CHATTY_SIZE chatty_embedded_db_controller::count() {
    CHATTY_SIZE result(0);
    CHATTY_QUERY_BLOCK _block;
    this->_generate_query_block(&_block);

#if defined(CHATTY_DO_BUILD_DEBUG)
    chatty_embedded_db_master::test_print((CHATTY_CHAR_PTR)"CHATTY TABLE COUNT ON");
#endif

    if (sqlite3_prepare_v2(this->selected_db, _block.value, -1, &this->selected_db_handle,nullptr) != CHATTY_STATUS_OK) {
#if defined(CHATTY_DO_BUILD_DEBUG)
        chatty_embedded_db_master::test_print((CHATTY_CHAR_PTR)"CHATTY DB INIT PREPARE FAILED");
#endif
        this->__update_query_execution_status();
        sqlite3_finalize(this->selected_db_handle);

    } else {
        std::cout << "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx : " << _block.value << '\n';
        sqlite3_step(this->selected_db_handle);
        int test_cnt = sqlite3_column_int(this->selected_db_handle, 0);

        this->__update_query_execution_status();
        std::cout << this->db_error_code << " / " << this->db_error_msg << " / " << test_cnt << '\n';

        sqlite3_finalize(this->selected_db_handle);
    };
    delete _block.value;
    return result;
}

CHATTY_ANY* chatty_embedded_db_controller::fetch_request_exec(
        CHATTY_UCHAR_PTR db_path,
        const CHATTY_UCHAR_PTR query,
        const CHATTY_UCHAR_PTR data_type,
        CHATTY_ERROR_CODE* return_error_code) {


    //return value type definition
    enum class type {INT32,INT64,TEXT};


    /**
     * @usage       internal function definition by upside
     * @desc        It need to release after use!
     * @desc        문자조합을 숫자로 해시변환 해준다. 중복률은 잘 모르겠고 아주 잘 동작한다.
     * @desc        문자열 비교를 위해 사용됨
    * */
    auto ctoi_roll = [](unsigned char* char_txt)->const int {
        int i(0);
        int res(0x55555555);
        while(char_txt[i]) {
            unsigned int buff_n = (unsigned int)char_txt[i];
            res += buff_n;
            res *= (int)(buff_n + i);
            res &= 0x55555555; //BIT CASING
            ++i;
        };
        return res;
    };

    /**
     * @usage       internal function definition by upside
     * @notice      It need to release after use!
     * @notice      It is utilize distance about between `A` and `a` and make epicenter.
     * @notice      대문자보다 소문자가 더 높은 수에 배정되있다.
     * */
    auto lowscale_up_alphabet = [](const unsigned char* char_txt, const unsigned int size)->unsigned char* {
        unsigned char* fc = (unsigned char*)malloc(sizeof(unsigned char) * size);
        unsigned int idx(0);

        unsigned int _dummy_a = (unsigned int)'A';
        unsigned int _dummy_b = (unsigned int)'a';
        unsigned int epicenter(_dummy_b - _dummy_a);

        for(idx=0;idx<size;++idx) {
            unsigned int buff_n = (unsigned int)char_txt[idx];
            unsigned int buff_res(0);
            if (buff_n >= 65 && buff_n <= 90) {
                buff_res = buff_n + epicenter;
            } else {
                buff_res = buff_n;
            };
            fc[idx] = buff_res;
        };
        return fc;
    };

    CHATTY_FLAG skip(false),exed(false),is_row(false);

    CHATTY_DB_LIB sqlite3_structure(nullptr);
    CHATTY_DB_LIB_STMT sqlite3_statement(nullptr);

    CHATTY_CHAR_PTR err_msg;
    CHATTY_ERROR_CODE err_code;

    //루프시 공용으로 사용하는 더미-정수
    CHATTY_SIZE loop_idx(0);

    //로우가 있으면 그 갯수
    CHATTY_SIZE row_cnt(0);

    //각 입력된 타입별 문자 길이
    //memory-on : 사용시 메모리를 제거했나요? (y@/n)
    //memory-on : 비사용시 메모리를 제거했나요? (y@/n)
    CHATTY_SIZE* data_type_letter_space = new CHATTY_SIZE[1];
    //각 입력된 타입별 문자 길이를 입력하기 위한 진행도를 저장하는 정수형 변수. `data_type_cnt` 가 증가할 때 0으로 초기화 된다.
    CHATTY_SIZE data_type_letter_move_n(0);
    //마지막 배열의 갯수 확인은 마지막 콜론의 위치에서 `data_type_size` - `data_type_last_colon_at` - 1 으로 합니다
    CHATTY_SIZE data_type_latest_colon_at(0);
    //데이터 타입의 갯수를 의미
    CHATTY_SIZE data_type_cnt(0);
    //글자수를 의미
    CHATTY_SIZE data_type_size(0);



    //정제된 데이터 타입 묶음을 가져오는 필수 요소
    //memory-on : 사용시 메모리를 제거했나요? (y@/n)
    //memory-on : 비사용시 메모리를 제거했나요? (y@/n)
    CHATTY_ANY** _mem_transport(nullptr);

    //글자수 가져오기
    if (data_type != nullptr)
        while(data_type[data_type_size])
            ++data_type_size;

    if (data_type_size != 0) {
        for (loop_idx=0;loop_idx<data_type_size;++loop_idx) {

            //colon is numbered `58` by ascii table.
            if ((CHATTY_UINT32)data_type[loop_idx] == 58) {
                //콜론 다음으로 오는 문자가 존재하는 (0`널바이트` || 32`스페이스` || 58`콜론` 가 아닌) 경우 확정한다.
                if ((CHATTY_UINT32)data_type[loop_idx + 1] != 0 && (CHATTY_UINT32)data_type[loop_idx + 1] != 32 && (CHATTY_UINT32)data_type[loop_idx + 1] != 58) {
                    ++data_type_cnt;

                    //더 큰 크기의 동적 배열을 준비
                    CHATTY_SIZE* new_data_type_letter_space = new CHATTY_SIZE[data_type_cnt + 1];

                    //값이 있으면 기존에 있던 값을 옮기기
                    if (data_type_cnt-1 != 0) {
                        CHATTY_SIZE loop_in_idx(0);
                        for(loop_in_idx=0;loop_in_idx<data_type_cnt;++loop_in_idx) {
                            new_data_type_letter_space[loop_in_idx] = data_type_letter_space[loop_in_idx];
                        };


                        //계산상 콜론까지 카운트되니 이후부턴 콜론만큼을 제외한 크기를 계산하기 위해
                        --data_type_letter_move_n;
                    };

                    //새 항목을 입력
                    new_data_type_letter_space[data_type_cnt-1] = data_type_letter_move_n;

                    //delete array and children also for memory move.
                    //memory-off : 배열 삭제로 루프중에 제거됨
                    delete[] data_type_letter_space;

                    //memory move
                    data_type_letter_space = new_data_type_letter_space;

                    //reset move_n
                    data_type_letter_move_n = 0;

                    //set latest colon at
                    data_type_latest_colon_at = loop_idx;
                };
            };
            ++data_type_letter_move_n;
        };

        //마지막 요소 삽입
        ++data_type_cnt;
        CHATTY_SIZE extra_size(0);
        CHATTY_SIZE* new_data_type_letter_space_for_last = new CHATTY_SIZE[data_type_cnt];

        //value copy
        for (loop_idx = 0; loop_idx < data_type_cnt; ++loop_idx) {
            new_data_type_letter_space_for_last[loop_idx] = data_type_letter_space[loop_idx];
        };

        //put at last
        if((CHATTY_INT32)data_type[data_type_size-1] == 58) {
            CHATTY_SIZE dummy_idx(0);
            while(true) {
                CHATTY_UINT32 buffer_n = (CHATTY_INT32)data_type[data_type_size + dummy_idx - 1];
                std::cout << buffer_n << '\n';
                if (buffer_n == 58) {
                    ++extra_size;
                } else {
                    break;
                };
                ++dummy_idx;
            };
        };
        new_data_type_letter_space_for_last[data_type_cnt-1] = data_type_size - data_type_latest_colon_at - extra_size - 1;

        //delete array and children also for memory move.
        //memory-off : 메모리 제거됨
        delete[] data_type_letter_space;

        //memory copy for last
        data_type_letter_space = new_data_type_letter_space_for_last;

    } else {
        //memory-off : 비사용시 메모리를 제거했습니다.
        delete[] data_type_letter_space;
        printf("(Alert) Behavior expectation : may isn't selection query for you meaning. because it is not filled `data_type` argument.`\n");
    };

    if (sqlite3_open((CHATTY_CHAR_PTR)db_path, &sqlite3_structure) != CHATTY_STATUS_OK) {
        err_msg     = (CHATTY_CHAR_PTR)sqlite3_errmsg(sqlite3_structure);
        err_code    = (CHATTY_ERROR_CODE) sqlite3_errcode(sqlite3_structure);
        printf("(Alert) Error at DB opening %d - %s\n", err_code, err_msg);
        skip = true;
    } else {
        printf("%s\n", "(Success) DB open");
    };

    if (sqlite3_prepare_v2(sqlite3_structure, (CHATTY_CHAR_PTR)query, -1, &sqlite3_statement, nullptr) != CHATTY_STATUS_OK && !skip) {
        err_msg     = (CHATTY_CHAR_PTR)sqlite3_errmsg(sqlite3_structure);
        err_code    = (CHATTY_ERROR_CODE) sqlite3_errcode(sqlite3_structure);
        printf("(Alert) Error at query preparing %d - %s\n", err_code, err_msg);
        sqlite3_reset(sqlite3_statement);
        sqlite3_close(sqlite3_structure);
        skip = true;
    } else {
        printf("%s\n", "(Success) prepare skipped");
    };

    CHATTY_ERROR_CODE step_result(0);
    if (!skip) {
        step_result = sqlite3_step(sqlite3_statement);
        exed = true;
        switch(step_result) {
            case CHATTY_STATUS_OK:
                printf("%s (%d)\n", "(QUERY) request is ok", step_result);
                break;
            case CHATTY_STATUS_DONE:
                printf("%s (%d)\n", "(QUERY) statement is done", step_result);
                break;
            case CHATTY_STATUS_CONSTRAINT:
                printf("%s (%d)\n", "(QUERY-ERROR) failed to execute by table rules and constraints", step_result);
                break;
            case CHATTY_STATUS_ROW:
                printf("%s (%d)\n", "(QUERY) got a rows", step_result);
                printf("%s \n", "(Notice) going on `row` refining proceeds");
                //`data_type`이 있어서 `row` 를 정제하는 경우
                if (data_type_size != 0) {
                    //결과값이 있음을 의미
                    is_row = true;
                    //글자수를 추가
                    row_cnt = 1;

                    //definition compatible data type
                    const CHATTY_UINT32 _type_int         = ctoi_roll((CHATTY_UCHAR_PTR)"int32");
                    const CHATTY_UINT32 _type_int64       = ctoi_roll((CHATTY_UCHAR_PTR)"int64");
                    const CHATTY_UINT32 _type_text        = ctoi_roll((CHATTY_UCHAR_PTR)"text");


                    //최초 실행 처리
                    //cpp based syntax
                    //공간 확장
                    std::cout << "- - row count (st) : " << row_cnt << '\n';
                    _mem_transport = new CHATTY_ANY*[row_cnt];
                    CHATTY_ANY** _mem_row = new CHATTY_ANY*[data_type_cnt];

                    CHATTY_SIZE stack(0);
                    for(loop_idx=0;loop_idx<data_type_cnt;++loop_idx) {
                        CHATTY_SIZE idx(0);
                        CHATTY_SIZE uchar_size = sizeof(CHATTY_UCHAR);

                        CHATTY_SIZE letter_space = data_type_letter_space[loop_idx];

                        //memory-on : 메모리를 제거했나요? (y@/n)
                        CHATTY_UCHAR_PTR buff_for_var = (CHATTY_UCHAR_PTR)malloc(letter_space+1);

                        for(idx=0;idx<letter_space+1;++idx) {
                            buff_for_var[idx] = (CHATTY_UCHAR) 0;
                        };

                        //memory-on : 메모리를 제거했나요? (y@/n)
                        //영어 대문자를 소문자로 변경
                        CHATTY_UCHAR_PTR buff_low_scaled_var = nullptr;

                        //단수의 값인 경우
                        if (data_type_cnt == 1) {
                            memcpy(buff_for_var, data_type + stack, (letter_space * uchar_size) + 2 );
                            buff_low_scaled_var = lowscale_up_alphabet(buff_for_var, letter_space + 2);
                            stack = stack + letter_space + 1;
                        } else {
                            memcpy(buff_for_var, data_type + stack, letter_space * uchar_size);
                            buff_low_scaled_var = lowscale_up_alphabet(buff_for_var, letter_space+1);
                            stack = stack + letter_space + 1;
                        };

                        //쉬운 비교를 위해 단순구조 해시로 정수 변경후 비교
                        const CHATTY_UINT32 buff_for_compare  = ctoi_roll((CHATTY_UCHAR_PTR)buff_low_scaled_var);

                        //타입별 메모리 할당
                        if (buff_for_compare == _type_int) {
                            CHATTY_INT32 _value = (CHATTY_INT32)sqlite3_column_int(sqlite3_statement, (CHATTY_INT32)loop_idx);
                            CHATTY_INT32* _basket = (CHATTY_INT32*)malloc(sizeof(CHATTY_INT32));
                            *_basket = _value;
                            _mem_row[loop_idx] = _basket;
                            printf("(Processing-st) be setting a (row : %d) %d column to `int32` - %p\n", row_cnt, loop_idx, _basket);
                        } else if(buff_for_compare == _type_int64) {
                            CHATTY_INT64 _value = (CHATTY_INT64)sqlite3_column_int64(sqlite3_statement, (CHATTY_INT32)loop_idx);
                            CHATTY_INT64* _basket = (CHATTY_INT64*)malloc(sizeof(CHATTY_INT64));
                            *_basket = _value;
                            _mem_row[loop_idx] = _basket;
                            printf("(Processing-st) be setting a (row : %d) %d column to `int64` - %p\n", row_cnt, loop_idx, _basket);
                        } else if(buff_for_compare == _type_text) {
                            CHATTY_UCHAR_PTR _basket = (CHATTY_UCHAR_PTR)sqlite3_column_text(sqlite3_statement, (CHATTY_INT32)loop_idx);
                            _mem_row[loop_idx] = _basket;
                            printf("(Processing-st) be setting a (row : %d) %d column to `text` - %p\n", row_cnt, loop_idx, _basket);
                        } else {
                            CHATTY_INT32 _value = sqlite3_column_int(sqlite3_statement, (CHATTY_INT32)loop_idx);
                            CHATTY_INT32* _basket = (CHATTY_INT32*)malloc(sizeof(CHATTY_INT32));
                            *_basket = _value;
                            _mem_row[loop_idx] = _basket;
                            printf("(Processing-st) be setting a (row : %d) %d column to `int32 as DEFAULT.` - %p\n", row_cnt, loop_idx, _basket);
                        };
                        //업 스케일의
                        //memory-off : 할 일 다 끝내고 루프중에 제거됨
                        delete buff_for_var;
                        //memory-off : 할 일 다 끝내고 루프중에 제거됨
                        delete buff_low_scaled_var;
                    };

                    _mem_transport[row_cnt] = _mem_row;
                    std::cout << "- - - - " << _mem_transport[row_cnt] << " row is now made (st)" << '\n';

                    std::cout << std::endl << std::endl;

                    //나머지 동일 처리
                    while(sqlite3_step(sqlite3_statement) != CHATTY_STATUS_DONE) {

                        std::cout << "- - row count (nd) : " << row_cnt+1 << '\n';

                        CHATTY_ANY** _new_mem_transport = new CHATTY_ANY*[row_cnt+1];

                        for (loop_idx = 0; loop_idx < row_cnt; ++loop_idx) {
                            std::cout << "- - - - " << _mem_transport[loop_idx] << " row is now copied! (nd)" << '\n';
                            _new_mem_transport[loop_idx] = _mem_transport[loop_idx];
                        };


                        CHATTY_ANY** _mem_row_nd = new CHATTY_ANY*[data_type_cnt];

                        CHATTY_SIZE stack_nd(0);
                        for(loop_idx=0;loop_idx<data_type_cnt;++loop_idx) {
                            CHATTY_SIZE idx(0);
                            CHATTY_SIZE uchar_size = sizeof(CHATTY_UCHAR);

                            CHATTY_SIZE letter_space = data_type_letter_space[loop_idx];

                            //memory-on : 메모리를 제거했나요? (y@/n)
                            CHATTY_UCHAR_PTR buff_for_var = (CHATTY_UCHAR_PTR)malloc(letter_space+1);

                            for(idx=0;idx<letter_space+1;++idx) {
                                buff_for_var[idx] = (CHATTY_UCHAR) 0;
                            };

                            //memory-on : 메모리를 제거했나요? (y@/n)
                            //영어 대문자를 소문자로 변경
                            CHATTY_UCHAR_PTR buff_low_scaled_var = nullptr;

                            //단수의 값인 경우
                            if (data_type_cnt == 1) {
                                memcpy(buff_for_var, data_type + stack_nd, (letter_space * uchar_size) + 2 );
                                buff_low_scaled_var = lowscale_up_alphabet(buff_for_var, letter_space + 2);
                                stack_nd = stack_nd + letter_space + 1;
                            } else {
                                memcpy(buff_for_var, data_type + stack_nd, letter_space * uchar_size);
                                buff_low_scaled_var = lowscale_up_alphabet(buff_for_var, letter_space+1);
                                stack_nd = stack_nd + letter_space + 1;
                            };

                            //쉬운 비교를 위해 단순구조 해시로 정수 변경후 비교
                            const CHATTY_UINT32 buff_for_compare  = ctoi_roll((CHATTY_UCHAR_PTR)buff_low_scaled_var);

                            //타입별 메모리 할당
                            if (buff_for_compare == _type_int) {
                                CHATTY_INT32 _value = (CHATTY_INT32)sqlite3_column_int(sqlite3_statement, (CHATTY_INT32)loop_idx);
                                CHATTY_INT32* _basket = (CHATTY_INT32*)malloc(sizeof(CHATTY_INT32));
                                *_basket = _value;
                                _mem_row_nd[loop_idx] = _basket;
                                printf("(Processing-nd) be setting a (row : %d) %d column to `int32` - %p\n", row_cnt+1, loop_idx, _basket);
                            } else if(buff_for_compare == _type_int64) {
                                CHATTY_INT64 _value = (CHATTY_INT64)sqlite3_column_int64(sqlite3_statement, (CHATTY_INT32)loop_idx);
                                CHATTY_INT64* _basket = (CHATTY_INT64*)malloc(sizeof(CHATTY_INT64));
                                *_basket = _value;
                                _mem_row_nd[loop_idx] = _basket;
                                printf("(Processing-nd) be setting a (row : %d) %d column to `int64` - %p\n", row_cnt+1, loop_idx, _basket);
                            } else if(buff_for_compare == _type_text) {
                                CHATTY_UCHAR_PTR _basket = (CHATTY_UCHAR_PTR)sqlite3_column_text(sqlite3_statement, (CHATTY_INT32)loop_idx);
                                _mem_row_nd[loop_idx] = _basket;
                                printf("(Processing-nd) be setting a (row : %d) %d column to `text` - %p\n", row_cnt+1, loop_idx, _basket);
                            } else {
                                CHATTY_INT32 _value = sqlite3_column_int(sqlite3_statement, (CHATTY_INT32)loop_idx);
                                CHATTY_INT32* _basket = (CHATTY_INT32*)malloc(sizeof(CHATTY_INT32));
                                *_basket = _value;
                                _mem_row_nd[loop_idx] = _basket;
                                printf("(Processing-nd) be setting a (row : %d) %d column to `int32 as DEFAULT.` - %p\n", row_cnt+1, loop_idx, _basket);
                            };
                            //업 스케일의
                            //memory-off : 할 일 다 끝내고 루프중에 제거됨
                            delete buff_for_var;
                            //memory-off : 할 일 다 끝내고 루프중에 제거됨
                            delete buff_low_scaled_var;
                        };

                        _new_mem_transport[row_cnt] = _mem_row_nd;
                        std::cout << "- - - - " << _new_mem_transport[row_cnt] << " row is now made (nd)" << '\n';


                        delete[] _mem_transport;
                        _mem_transport = _new_mem_transport;
                        ++row_cnt;
                        std::cout << std::endl << std::endl;
                    };
                };
                break;

            default:
                printf("%s (%d)\n", "(QUERY) is not defined exception", step_result);
                break;
        }
    } else {
        printf("%s\n", "(Success) step skipped");
    };

    if (exed) {
        printf("%s\n", "(Notice) exed put on reference to status.");
        *return_error_code = (CHATTY_ERROR_CODE) sqlite3_errcode(sqlite3_structure);
    } else {
        *return_error_code = CHATTY_STATUS_ERROR;
    };

    //garbage collect
    CHATTY_UINT32 stat_reset(0),stat_finalize(0),stat_close(0);
    stat_reset      = sqlite3_reset(sqlite3_statement);
    stat_finalize   = sqlite3_finalize(sqlite3_statement);
    stat_close      = sqlite3_close(sqlite3_structure);

    //print closing
    printf("(Notice) ** reset (%d), finalize (%d), close (%d)\n", stat_reset, stat_finalize, stat_close);

    if (is_row) {
        CHATTY_DB_FETCH_RESULT* returns = new CHATTY_DB_FETCH_RESULT;
        returns->value      = _mem_transport;
        returns->size       = row_cnt;
        returns->column_cnt = data_type_cnt;
        printf("%s\n", "(Success) row returned");
        return returns;
    } else {
        printf("%s\n", "(Success) is notting returned");
        return nullptr;
    }
}

CHATTY_ANY chatty_embedded_db_controller::fetch_request_exec_release(CHATTY_DB_FETCH_RESULT *addr) {
    printf("%s\n", "(Notice) release-ment on start");
    std::cout << "(Info) column count : " << addr->column_cnt << " / row count : " << addr->size << '\n';
    CHATTY_SIZE i(0),ii(0);
    for(i=0;i<addr->size;++i) {
        void** buffer_row = (void**)addr->value[i];
        for(ii=0;ii<addr->column_cnt;++ii) {
            void* buffer_column_value = (void*)buffer_row[ii];
            std::cout << "- - - - - - - - column delete : " << buffer_column_value << '\n';
            delete buffer_column_value;
        };
        std::cout << "- - - - - - row delete : " << buffer_row << '\n';
        delete[] buffer_row;
    };
    std::cout << "- - - - value delete : " << addr->value << '\n';
    delete[] addr->value;
    std::cout << "- - self delete : " << addr << '\n';
    delete addr;
}

CHATTY_ANY chatty_embedded_db_controller::fetch_request_exec_release(CHATTY_ANY *addr) {
    printf("%s\n", "(Notice) it's on type casting to `CHATTY_DB_FETCH_RESULT*` by language syntax overload for release-ment process");
    chatty_embedded_db_controller::fetch_request_exec_release((CHATTY_DB_FETCH_RESULT*)addr);
};


