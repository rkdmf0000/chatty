//
// Created by my_fl on 2021-02-21.
//

#include "chatty_embedded_db_controller.h"
CHATTY_ANY chatty_embedded_db_controller::use(const CHATTY_STRING& filepath) {
    if (this->is_db_standby) {
        this->test_print((CHATTY_CHAR_PTR)"db connection has been connect");
        return;
    };
    this->test_print((CHATTY_CHAR_PTR)"open chatty db connection");
    this->latest_error_status = sqlite3_open(filepath.c_str(), &this->selected_db);
    if (this->latest_error_status == SQLITE_OK)
        this->is_db_standby = true;
};

CHATTY_ANY chatty_embedded_db_controller::close() {
    if (!this->is_db_standby) {
        this->test_print((CHATTY_CHAR_PTR)"db connection has been close");
        return;
    };
    this->test_print((CHATTY_CHAR_PTR)"close chatty db connection");
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
        this->test_print((CHATTY_CHAR_PTR)"ERROR OCCURRED - Duplicated behavior change");
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
        this->test_print((CHATTY_CHAR_PTR)"prepare_v2 failed");
        #endif
        this->__update_query_execution_status();
        sqlite3_finalize(this->selected_db_handle);
        return this->db_error_code;;
    } else {
        switch((CHATTY_UINT32)this->query_form.request_type) {
            case CHATTY_KEYWORD_REQUEST_SELECT:{
                if (sqlite3_step(this->selected_db_handle) != CHATTY_STATUS_ROW) {
#if defined(CHATTY_DO_BUILD_DEBUG)
                    this->test_print((CHATTY_CHAR_PTR)"step failed - select");
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
                    this->test_print((CHATTY_CHAR_PTR)"step failed - default");
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
};


