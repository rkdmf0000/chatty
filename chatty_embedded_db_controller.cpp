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
 * @param   group    CHATTY_DB_COLUMN_CONNECTIO***  structure heap ??? reference ??? ?????? pointer
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
            //?????? ????????? ??????
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
            std::cout << rf_collection->value[idx]
            << " / " << rf_collection->value[idx]->idx
            << " / " << rf_collection->value[idx]->id
            << " / " << rf_collection->value[idx]->latest_connect_date
            << " / " << rf_collection->value[idx]->is_online
            << '\n';
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
        CHATTY_UCHAR_PTR query,
        const CHATTY_UCHAR_PTR data_type,
        CHATTY_ERROR_CODE* return_error_code) {


    //return value type definition
    enum class type {INT32,INT64,TEXT};

    /**
     * @usage       internal function definition by upside
     * @desc        ????????? ??????
    * */
    auto get_str_len = [](unsigned char* char_txt)->const unsigned int {
        unsigned int size(0);
        if (char_txt != nullptr)
            while(char_txt[size])
                ++size;
        return (unsigned int)size;
    };


    /**
     * @usage       internal function definition by upside
     * @desc        ??????????????? ????????? ???????????? ?????????. ???????????? ??? ???????????? ?????? ??? ????????????.
     * @desc        ????????? ????????? ?????? ?????????
    * */
    auto ctoi_roll = [](unsigned char* char_txt)->const int {
        int i(0);
        int res(CHATTY_DB_FN_HASH);
        while(char_txt[i]) {
            unsigned int buff_n = (unsigned int)char_txt[i];
            res += buff_n;
            res *= (int)(buff_n + i);
            res &= CHATTY_DB_FN_HASH; //BIT CASING
            ++i;
        };
        return res;
    };

    /**
     * @usage       internal function definition by upside
     * @notice      It need to release after use!
     * @notice      It is utilize distance about between `A` and `a` and make epicenter.
     * @notice      ??????????????? ???????????? ??? ?????? ?????? ???????????????.
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

    //????????? ???????????? ???????????? ??????-??????
    CHATTY_SIZE loop_idx(0);

    //definition compatible data type
    const CHATTY_UINT32 _type_int         = ctoi_roll((CHATTY_UCHAR_PTR)"int32");
    const CHATTY_UINT32 _type_int64       = ctoi_roll((CHATTY_UCHAR_PTR)"int64");
    const CHATTY_UINT32 _type_text        = ctoi_roll((CHATTY_UCHAR_PTR)"text");

    //add type definition
    //?????? ????????? ?????????
    //is_row ??? false ?????? ??? ????????? ??????
    //memory-on : ????????? select ????????? ???????????? strcut self detroy ??? ??? ?????????. ???????????? ????????? is_row ?????? false ??? ?????? ??????
    CHATTY_UINT32* _defined_type = new CHATTY_UINT32[CHATTY_DB_DATA_TYPE_COUNT];
    _defined_type[0] = _type_int;
    _defined_type[1] = _type_int64;
    _defined_type[2] = _type_text;

    //is_row ??? true ??? ??? heap ????????? ?????????
    //???????????? ?????? ???????????? ?????? structure ??? destructure ?????? ????????? ??????
    //release ????????? self destruct ?????? ???????????? ???????????? ??????????????? ?????????
    //????????? ?????? ???????????? ???????????? ??????.
    CHATTY_UINT32* _defined_column_type;

    CHATTY_FLAG skip(false),exed(false),is_row(false);

    CHATTY_DB_LIB sqlite3_structure(nullptr);
    CHATTY_DB_LIB_STMT sqlite3_statement(nullptr);

    CHATTY_CHAR_PTR err_msg;
    CHATTY_ERROR_CODE err_code;


    //????????? ????????? ??? ??????
    CHATTY_SIZE row_cnt(0);

    //??? ????????? ????????? ?????? ??????
    //memory-on : ????????? ???????????? ???????????????? (y@/n)
    //memory-on : ???????????? ???????????? ???????????????? (y@/n)
    CHATTY_SIZE* data_type_letter_space = new CHATTY_SIZE[1];
    //??? ????????? ????????? ?????? ????????? ???????????? ?????? ???????????? ???????????? ????????? ??????. `data_type_cnt` ??? ????????? ??? 0?????? ????????? ??????.
    CHATTY_SIZE data_type_letter_move_n(0);
    //????????? ????????? ?????? ????????? ????????? ????????? ???????????? `data_type_size` - `data_type_last_colon_at` - 1 ?????? ?????????
    CHATTY_SIZE data_type_latest_colon_at(0);
    //????????? ????????? ????????? ??????
    CHATTY_SIZE data_type_cnt(0);
    //????????? ?????? ???????????? ??????
    CHATTY_SIZE data_type_size(0);
    //?????? ????????? select ?????? ??????????????? ???????????? ???
    CHATTY_FLAG is_select_syntax(false);
    //??? ?????? ????????? ????????? ?????? ??? ????????? ????????? ????????? ???????????????.
    CHATTY_FLAG is_selection_query_will_be_ok(false);
    //?????? ???????????? ??????
    CHATTY_SIZE query_size(0);

    //????????? ????????? ?????? ????????? ???????????? ?????? ??????
    //memory-on : ????????? ???????????? ???????????????? (y@/n)
    //memory-on : ???????????? ???????????? ???????????????? (y@/n)
    CHATTY_ANY** _mem_transport(nullptr);

    //????????? ?????? ????????? ????????????
    if (data_type != nullptr)
        data_type_size = get_str_len(data_type);

    //?????? ????????? ????????????
    if (query != nullptr)
        query_size = get_str_len(query);


    if (data_type_size != 0) {

        //?????? ????????? ?????? from ??? ???????????? ?????? ??????
        CHATTY_UCHAR_PTR _keyword_from = (CHATTY_UCHAR_PTR) CHATTY_CONST_KEYWORD_FROM;

        //????????? ?????? select ??? ????????? ???????????? ?????? ??????
        CHATTY_UCHAR_PTR _keyword_select = (CHATTY_UCHAR_PTR) CHATTY_CONST_KEYWORD_SELECT;

        //????????? select ?????? ????????? ??????????????? ?????? ????????? ??????
        if (query_size >= (CHATTY_SIZE)CHATTY_CONST_KEYWORD_SIZE_SELECT - 1) {

            //memory-on : ????????????
            CHATTY_UCHAR_PTR _keyword_select_lowscaled(nullptr);
            _keyword_select_lowscaled = lowscale_up_alphabet(_keyword_select, CHATTY_CONST_KEYWORD_SIZE_SELECT - 1);
            CHATTY_UINT32 _keyword_select_lowscaled_hash = ctoi_roll(_keyword_select_lowscaled);

            //????????? ???????????? ????????? ???????????? ??????
            CHATTY_SIZE _size_unsigned_char = sizeof(CHATTY_UCHAR);
            CHATTY_UCHAR_PTR _copied_select(nullptr);

            memcpy(&_copied_select, &query, _size_unsigned_char * (CHATTY_CONST_KEYWORD_SIZE_SELECT - 1));

            //memory-on : ????????????
            CHATTY_UCHAR_PTR _copied_select_lowscaled(nullptr);
            _copied_select_lowscaled = lowscale_up_alphabet(_copied_select, CHATTY_CONST_KEYWORD_SIZE_SELECT - 1);
            CHATTY_UINT32 _copied_select_lowscaled_hash = ctoi_roll(_copied_select_lowscaled);

            if (_copied_select_lowscaled_hash == _keyword_select_lowscaled_hash) {
                printf("%s\n", "(Notice) selection syntax detected");
                is_select_syntax = true;
            };

            //memory-off : ??????1 ?????? ??????
            delete _keyword_select_lowscaled;
            //memory-off : ??????2 ?????? ??????
            delete _copied_select_lowscaled;
        };

        if (is_select_syntax) {
            printf("%s\n", "(Notice) finding from keyword");

            //memory-on : ????????????
            CHATTY_UCHAR_PTR _keyword_from_lowscaled(nullptr);
            _keyword_from_lowscaled = lowscale_up_alphabet(_keyword_from, CHATTY_CONST_KEYWORD_SIZE_FROM - 1);

            //from ???????????? ????????? ???????????????
            CHATTY_SIZE _from_at(0);

            //?????? ??????
            for (loop_idx=0;loop_idx<query_size;++loop_idx) {

                //memory-on : ?????? ????????? ??? ?????? ??????
                CHATTY_UCHAR_PTR _buffer_low_scaled_a = lowscale_up_alphabet(&query[loop_idx], 1);
                CHATTY_UCHAR_PTR _buffer_low_scaled_b = lowscale_up_alphabet(&query[loop_idx+1], 1);
                CHATTY_UCHAR_PTR _buffer_low_scaled_c = lowscale_up_alphabet(&query[loop_idx+2], 1);
                CHATTY_UCHAR_PTR _buffer_low_scaled_d = lowscale_up_alphabet(&query[loop_idx+3], 1);

                if (loop_idx % 8 == 0 && loop_idx != 0)
                    printf("\r\n");

                printf("-%c%c%c%c-", *_buffer_low_scaled_a, *_buffer_low_scaled_b, *_buffer_low_scaled_c, *_buffer_low_scaled_d);

                if ((CHATTY_UINT32)*_buffer_low_scaled_a == (CHATTY_UINT32)_keyword_from_lowscaled[0]
                    && (CHATTY_UINT32)*_buffer_low_scaled_b == (CHATTY_UINT32)_keyword_from_lowscaled[1]
                    && (CHATTY_UINT32)*_buffer_low_scaled_c == (CHATTY_UINT32)_keyword_from_lowscaled[2]
                    && (CHATTY_UINT32)*_buffer_low_scaled_d == (CHATTY_UINT32)_keyword_from_lowscaled[3]
                        ) {
                    printf("\n%s\n", "(Notice) from keyword detected!");
                    _from_at = loop_idx;
                };

                //memory-off : ?????? ???????????? ?????? ??????
                delete _buffer_low_scaled_a;
                delete _buffer_low_scaled_b;
                delete _buffer_low_scaled_c;
                delete _buffer_low_scaled_d;

                if (query_size - (CHATTY_CONST_KEYWORD_SIZE_FROM - 1) <= loop_idx) {
                    std::cout << std::endl;
                    printf("%s\n", "(Alert) from keyword not exists. that will occurred error!");
                    break;
                };

                if (_from_at != 0)
                    break;

            };

            //memory-off : ?????? ??????
            delete _keyword_from_lowscaled;

            //????????? ?????? ??????
            for (loop_idx=0;loop_idx<data_type_size;++loop_idx) {

                //colon is numbered `58` by ascii table.
                if ((CHATTY_UINT32)data_type[loop_idx] == 58) {
                    //?????? ???????????? ?????? ????????? ???????????? (0`????????????` || 32`????????????` || 58`??????` ??? ??????) ?????? ????????????.
                    if ((CHATTY_UINT32)data_type[loop_idx + 1] != 0 && (CHATTY_UINT32)data_type[loop_idx + 1] != 32 && (CHATTY_UINT32)data_type[loop_idx + 1] != 58) {
                        ++data_type_cnt;

                        //??? ??? ????????? ?????? ????????? ??????
                        CHATTY_SIZE* new_data_type_letter_space = new CHATTY_SIZE[data_type_cnt + 1];

                        //?????? ????????? ????????? ?????? ?????? ?????????
                        if (data_type_cnt-1 != 0) {
                            CHATTY_SIZE loop_in_idx(0);
                            for(loop_in_idx=0;loop_in_idx<data_type_cnt;++loop_in_idx) {
                                new_data_type_letter_space[loop_in_idx] = data_type_letter_space[loop_in_idx];
                            };

                            //????????? ???????????? ??????????????? ???????????? ??????????????? ????????? ????????? ???????????? ??????
                            --data_type_letter_move_n;
                        };

                        //??? ????????? ??????
                        new_data_type_letter_space[data_type_cnt-1] = data_type_letter_move_n;

                        //delete array and children also for memory move.
                        //memory-off : ?????? ????????? ???????????? ?????????
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

            //????????? ?????? ??????
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
            //memory-off : ????????? ?????????
            delete[] data_type_letter_space;

            //memory copy for last
            data_type_letter_space = new_data_type_letter_space_for_last;

            //????????? ?????? ????????? ????????? ????????? ????????? ????????? ??????
            if (is_select_syntax && _from_at != 0 && data_type_cnt != 0) {
                printf("%s\n", "(Notice) now checking for query count of column part and count of data_type. that two will be must matched. if else occurred error.");


                CHATTY_SIZE start_point(CHATTY_CONST_KEYWORD_SIZE_SELECT - 1);
                CHATTY_SIZE end_point(_from_at - start_point - 1);
                CHATTY_SIZE _size_unsigned_char = sizeof(CHATTY_UCHAR);

                CHATTY_ANY* _part_of_column = (CHATTY_ANY*)malloc(_size_unsigned_char * end_point + 1);
                memcpy(_part_of_column, query+start_point + 1 , end_point);


                CHATTY_UCHAR_PTR cv_column_part = (CHATTY_UCHAR_PTR)_part_of_column;
                cv_column_part[end_point] = '\0';

                //??????
                CHATTY_UINT32 _tmp_spot_letter_n = (CHATTY_UINT32)CHATTY_CONST_KEYWORD_SPOT_LATTER_FOR_SINGLE;

                //?????? ??????
                CHATTY_SIZE spot_cnt(0);

                //????????? ????????? ????????? ?????????
                CHATTY_SIZE latest_spot_at(0);

                for(loop_idx=0;loop_idx<end_point;++loop_idx) {
                    CHATTY_UINT32 buffer_a = (CHATTY_UCHAR)cv_column_part[loop_idx];
                    if (buffer_a == _tmp_spot_letter_n) {
                        ++spot_cnt;
                        latest_spot_at = loop_idx;
                    };
                };

                if (end_point-1 > latest_spot_at+1) {
                    ++spot_cnt;
                };

                // ????????? ????????? ????????? ?????? ??????
                if (spot_cnt == data_type_cnt) {
                    printf("%s\n", "(Ok) column count matched!");
                    is_selection_query_will_be_ok = true;
                } else {
                    printf("%s\n", "(Alert) column count not matched!");
                };
                delete cv_column_part;
            };
        } else {
            printf("%s\n", "(Notice) is not selection query!");
        };

    } else {
        //memory-off : ???????????? ???????????? ??????????????????.
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

                //`data_type`??? ????????? `row` ??? ???????????? ??????
                if (data_type_size != 0 && is_select_syntax && is_selection_query_will_be_ok) {

                    //???????????? ????????? ??????
                    is_row = true;
                    //???????????? ??????
                    row_cnt = 1;

                    /**
                     * ##########################################
                     * @@@@@@@@@@@@@@@@@@@@@@@@@@@?????? ?????? ??? ??????
                     * ##########################################
                     * */
                    //cpp based syntax
                    //?????? ??????
                    std::cout << "- - row count (st) : " << row_cnt << '\n';
                    _mem_transport = new CHATTY_ANY*[row_cnt];
                    CHATTY_ANY** _mem_row = new CHATTY_ANY*[data_type_cnt];

                    CHATTY_SIZE stack(0);

                    //memory-on : ????????? struct ??? destruct ?????? ?????????
                    //???????????? nullptr ??? ???????????? ??????
                    _defined_column_type = new CHATTY_UINT32[data_type_cnt];

                    for(loop_idx=0;loop_idx<data_type_cnt;++loop_idx) {

                        CHATTY_SIZE idx(0);
                        CHATTY_SIZE uchar_size = sizeof(CHATTY_UCHAR);

                        CHATTY_SIZE letter_space = data_type_letter_space[loop_idx];

                        //memory-on : ???????????? ???????????????? (y@/n)
                        CHATTY_UCHAR_PTR buff_for_var = (CHATTY_UCHAR_PTR)malloc(letter_space+1);

                        for(idx=0;idx<letter_space+1;++idx) {
                            buff_for_var[idx] = (CHATTY_UCHAR) 0;
                        };

                        //memory-on : ???????????? ???????????????? (y@/n)
                        //?????? ???????????? ???????????? ??????
                        CHATTY_UCHAR_PTR buff_low_scaled_var = nullptr;

                        //????????? ?????? ??????
                        if (data_type_cnt == 1) {
                            memcpy(buff_for_var, data_type + stack, (letter_space * uchar_size) + 2 );
                            buff_low_scaled_var = lowscale_up_alphabet(buff_for_var, letter_space + 2);
                            stack = stack + letter_space + 1;
                        } else {
                            memcpy(buff_for_var, data_type + stack, letter_space * uchar_size);
                            buff_low_scaled_var = lowscale_up_alphabet(buff_for_var, letter_space+1);
                            stack = stack + letter_space + 1;
                        };

                        //?????? ????????? ?????? ???????????? ????????? ?????? ????????? ??????
                        const CHATTY_UINT32 buff_for_compare  = ctoi_roll((CHATTY_UCHAR_PTR)buff_low_scaled_var);

                        //????????? ????????? ??????
                        if (buff_for_compare == _type_int) {
                            CHATTY_INT32 _value = (CHATTY_INT32)sqlite3_column_int(sqlite3_statement, (CHATTY_INT32)loop_idx);
                            CHATTY_INT32* _basket = (CHATTY_INT32*)malloc(sizeof(CHATTY_INT32));
                            *_basket = _value;
                            _mem_row[loop_idx] = _basket;
                            _defined_column_type[loop_idx] = buff_for_compare;
                            printf("(Processing-st) be setting a (row : %d) %d column to `int32` - %p\n", row_cnt, loop_idx, _basket);
                        } else if(buff_for_compare == _type_int64) {
                            CHATTY_INT64 _value = (CHATTY_INT64)sqlite3_column_int64(sqlite3_statement, (CHATTY_INT32)loop_idx);
                            CHATTY_INT64* _basket = (CHATTY_INT64*)malloc(sizeof(CHATTY_INT64));
                            *_basket = _value;
                            _mem_row[loop_idx] = _basket;
                            _defined_column_type[loop_idx] = buff_for_compare;
                            printf("(Processing-st) be setting a (row : %d) %d column to `int64` - %p\n", row_cnt, loop_idx, _basket);
                        } else if(buff_for_compare == _type_text) {
                            CHATTY_SIZE text_i(0);
                            CHATTY_UCHAR_PTR _value = (CHATTY_UCHAR_PTR)sqlite3_column_text(sqlite3_statement, (CHATTY_INT32)loop_idx);
                            CHATTY_UINT32 _value_size(0);
                            CHATTY_SIZE uchar_size = sizeof(CHATTY_UCHAR);
                            _value_size = get_str_len(_value);
                            CHATTY_UCHAR_PTR _basket = (CHATTY_UCHAR_PTR)malloc(uchar_size * _value_size + 1);
                            for(text_i=0;text_i<uchar_size * _value_size + 1;++text_i)
                                _basket[text_i] = 0;
                            memmove(_basket, _value, _value_size * uchar_size);
                            _mem_row[loop_idx] = _basket;
                            _defined_column_type[loop_idx] = buff_for_compare;
                            printf("(Processing-st) be setting a (row : %d) %d column to `text` - %p\n", row_cnt, loop_idx, _basket);
                        } else {
                            CHATTY_INT32 _value = sqlite3_column_int(sqlite3_statement, (CHATTY_INT32)loop_idx);
                            CHATTY_INT32* _basket = (CHATTY_INT32*)malloc(sizeof(CHATTY_INT32));
                            *_basket = _value;
                            _mem_row[loop_idx] = _basket;
                            _defined_column_type[loop_idx] = buff_for_compare;
                            printf("(Processing-st) be setting a (row : %d) %d column to `int32 as DEFAULT.` - %p\n", row_cnt, loop_idx, _basket);
                        };

                        //??? ????????????
                        //memory-off : ??? ??? ??? ????????? ???????????? ?????????
                        delete buff_for_var;
                        //memory-off : ??? ??? ??? ????????? ???????????? ?????????
                        delete buff_low_scaled_var;
                    };

                    _mem_transport[row_cnt] = _mem_row;
                    std::cout << "- - - - " << _mem_transport[row_cnt] << " row is now made (st)" << '\n';

                    std::cout << std::endl << std::endl;

                    /**
                     * ##########################################
                     * @@@@@@@@@@@@@@@@@@@@@@@@@@@?????? ??? ?????? ??????
                     * ##########################################
                     * */
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

                            //memory-on : ???????????? ???????????????? (y@/n)
                            CHATTY_UCHAR_PTR buff_for_var = (CHATTY_UCHAR_PTR)malloc(letter_space+1);

                            for(idx=0;idx<letter_space+1;++idx) {
                                buff_for_var[idx] = (CHATTY_UCHAR) 0;
                            };

                            //memory-on : ???????????? ???????????????? (y@/n)
                            //?????? ???????????? ???????????? ??????
                            CHATTY_UCHAR_PTR buff_low_scaled_var = nullptr;

                            //????????? ?????? ??????
                            if (data_type_cnt == 1) {
                                memcpy(buff_for_var, data_type + stack_nd, (letter_space * uchar_size) + 2 );
                                buff_low_scaled_var = lowscale_up_alphabet(buff_for_var, letter_space + 2);
                                stack_nd = stack_nd + letter_space + 1;
                            } else {
                                memcpy(buff_for_var, data_type + stack_nd, letter_space * uchar_size);
                                buff_low_scaled_var = lowscale_up_alphabet(buff_for_var, letter_space+1);
                                stack_nd = stack_nd + letter_space + 1;
                            };

                            //?????? ????????? ?????? ???????????? ????????? ?????? ????????? ??????
                            const CHATTY_UINT32 buff_for_compare  = ctoi_roll((CHATTY_UCHAR_PTR)buff_low_scaled_var);

                            //????????? ????????? ??????
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
                                CHATTY_SIZE text_i(0);
                                CHATTY_UCHAR_PTR _value = (CHATTY_UCHAR_PTR)sqlite3_column_text(sqlite3_statement, (CHATTY_INT32)loop_idx);
                                CHATTY_UINT32 _value_size(0);
                                CHATTY_SIZE uchar_size = sizeof(CHATTY_UCHAR);
                                _value_size = get_str_len(_value);
                                CHATTY_UCHAR_PTR _basket = (CHATTY_UCHAR_PTR)malloc(uchar_size * _value_size + 1);
                                for(text_i=0;text_i<uchar_size * _value_size + 1;++text_i)
                                    _basket[text_i] = 0;
                                memmove(_basket, _value, _value_size * uchar_size);
                                _mem_row_nd[loop_idx] = _basket;
                                printf("(Processing-nd) be setting a (row : %d) %d column to `text` - %p\n", row_cnt+1, loop_idx, _value);
                            } else {
                                CHATTY_INT32 _value = sqlite3_column_int(sqlite3_statement, (CHATTY_INT32)loop_idx);
                                CHATTY_INT32* _basket = (CHATTY_INT32*)malloc(sizeof(CHATTY_INT32));
                                *_basket = _value;
                                _mem_row_nd[loop_idx] = _basket;
                                printf("(Processing-nd) be setting a (row : %d) %d column to `int32 as DEFAULT.` - %p\n", row_cnt+1, loop_idx, _basket);
                            };
                            //??? ????????????
                            //memory-off : ??? ??? ??? ????????? ???????????? ?????????
                            delete buff_for_var;
                            //memory-off : ??? ??? ??? ????????? ???????????? ?????????
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
        printf("%s\n", "(Success) row returned");
        CHATTY_DB_FETCH_RESULT* returns = new CHATTY_DB_FETCH_RESULT(data_type_cnt);
        returns->_defined_type          = _defined_type;
        returns->_defined_column_type   = _defined_column_type;
        returns->value                  = _mem_transport;
        returns->size                   = row_cnt;
        returns->column_cnt             = data_type_cnt;
        return returns;
    } else {
        printf("%s\n", "(Success) is notting returned (1)");
        delete[] _defined_type;
        //delete[] _defined_column_type;
        printf("%s\n", "(Success) is notting returned (2)");
        return nullptr;
    }
}

CHATTY_ANY *chatty_embedded_db_controller::fetch_request_exec(CHATTY_UCHAR_PTR db_path, CHATTY_UCHAR_PTR query, CHATTY_ERROR_CODE *return_error_code) {
    return chatty_embedded_db_controller::fetch_request_exec(db_path, query, nullptr, return_error_code);
};

CHATTY_ANY chatty_embedded_db_controller::fetch_request_exec_release(CHATTY_DB_FETCH_RESULT *addr) {

    /**
     * @usage       internal function definition by upside
     * @desc        It need to release after use!
     * @desc        ??????????????? ????????? ???????????? ?????????. ???????????? ??? ???????????? ?????? ??? ????????????.
     * @desc        ????????? ????????? ?????? ?????????
    * */
    auto ctoi_roll = [](unsigned char* char_txt)->const int {
        int i(0);
        int res(CHATTY_DB_FN_HASH);
        while(char_txt[i]) {
            unsigned int buff_n = (unsigned int)char_txt[i];
            res += buff_n;
            res *= (int)(buff_n + i);
            res &= CHATTY_DB_FN_HASH; //BIT CASING
            ++i;
        };
        return res;
    };

    //definition compatible data type
    const CHATTY_UINT32 _type_int         = ctoi_roll((CHATTY_UCHAR_PTR)"int32");
    const CHATTY_UINT32 _type_int64       = ctoi_roll((CHATTY_UCHAR_PTR)"int64");
    const CHATTY_UINT32 _type_text        = ctoi_roll((CHATTY_UCHAR_PTR)"text");

    printf("%s\n", "(Notice) release-ment on start");

    std::cout << "(Info) column count : " << addr->column_cnt << " / row count : " << addr->size << '\n';

    CHATTY_SIZE i(0),ii(0);
    for(i=0;i<addr->size;++i) {
        void** buffer_row = (void**)addr->value[i];
        for(ii=0;ii<addr->column_cnt;++ii) {
            void* buffer_column_value = (void*)buffer_row[ii];
            if (addr->_defined_column_type[ii] == _type_int) {
                CHATTY_INT32* buff = (CHATTY_INT32*)buffer_column_value;
                delete buff;
                std::cout << "- - - - - - - - column delete : " << buffer_column_value << " / data_type : int32" << '\n';
            } else if (addr->_defined_column_type[ii] == _type_int64) {
                CHATTY_INT64* buff = (CHATTY_INT64*)buffer_column_value;
                delete buff;
                std::cout << "- - - - - - - - column delete : " << buffer_column_value << " / data_type : int64" << '\n';
            } else if (addr->_defined_column_type[ii] == _type_text) {
                CHATTY_UCHAR_PTR buff = (CHATTY_UCHAR_PTR)buffer_column_value;
                delete[] buff;
                std::cout << "- - - - - - - - column delete : " << buffer_column_value << " / data_type : text" << '\n';
            } else {
                CHATTY_INT32* buff = (CHATTY_INT32*)buffer_column_value;
                delete buff;
                std::cout << "- - - - - - - - column delete : " << buffer_column_value << " / data_type : undefined(int32)" << '\n';
            };
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
}



