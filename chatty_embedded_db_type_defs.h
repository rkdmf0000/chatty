//
// Created by my_fl on 2021-02-17.
//

#include <iostream>
#include <stdio.h>


#ifndef CHATTY_CHATTY_EMBEDDED_DB_TYPE_DEFS_H
#define CHATTY_CHATTY_EMBEDDED_DB_TYPE_DEFS_H

#include "chatty_embedded_db_includes.h"
/**
 * 단순히 코드의 간결성을 위해 정의됨
 * */
#define CHATTY_VIRTUAL virtual
#define CHATTY_STATIC static
#define CHATTY_CONST const
/**
 * common types 1
 * */
typedef std::string CHATTY_STRING;
typedef sqlite3* CHATTY_DB_LIB;
typedef sqlite3_stmt* CHATTY_DB_LIB_STMT;
typedef unsigned int CHATTY_ERROR_STATUS;
typedef void CHATTY_ANY;
typedef char CHATTY_CHAR;
typedef char* CHATTY_CHAR_PTR;
typedef unsigned char* CHATTY_UCHAR_PTR;
typedef unsigned char CHATTY_UCHAR;
typedef bool CHATTY_FLAG;

/**
 * common types 2
 * */
typedef int CHATTY_INT32;
typedef long long CHATTY_INT64;
typedef unsigned int CHATTY_UINT32;
typedef unsigned long long CHATTY_UINT64;

/**
 * unused
 * */
typedef CHATTY_INT32** CHATTY_NUMERIC_ARRAY_COLLECTOR;
typedef CHATTY_CHAR_PTR* CHATTY_CHAR_COLLECTOR;

/**
 * special types
 * */
typedef CHATTY_UINT32 CHATTY_SIZE;
typedef CHATTY_UINT32 CHATTY_ERROR_CODE;
typedef CHATTY_CHAR CHATTY_QUERY;
typedef CHATTY_CHAR_PTR CHATTY_QUERY_PTR;
typedef CHATTY_UCHAR CHATTY_JOIN_KEYWORD;
typedef CHATTY_UCHAR CHATTY_QUOTES_TYPE;
/**
 * 결과 조인용
 * */
#define CHATTY_KEYWORD_JOIN_NULL 0
#define CHATTY_KEYWORD_JOIN_AND 1
#define CHATTY_KEYWORD_JOIN_OR 2

/**
 * 연결자(조인)의 크기를 정의 (null byte 를 제외한 크기)
 * */
#define CHATTY_KEYWORD_JOIN_SIZE_NULL 0
#define CHATTY_CONST_KEYWORD_AND "AND"
#define CHATTY_KEYWORD_JOIN_SIZE_AND 4
#define CHATTY_CONST_KEYWORD_OR "OR"
#define CHATTY_KEYWORD_JOIN_SIZE_OR 3

/**
 * 표현자(behavior) 의 크기를 정의 (null byte 를 제외한 크기)
 * LETTER 를 LATTER 라고 잘못 썼다ㅋㅋ 돌리기에 너무 늦음
 * */
#define CHATTY_CONST_KEYWORD_NULL 0

#define CHATTY_CONST_KEYWORD_SELECT "SELECT"
#define CHATTY_CONST_KEYWORD_SIZE_SELECT 7

#define CHATTY_CONST_KEYWORD_INSERT "INSERT"
#define CHATTY_CONST_KEYWORD_SIZE_INSERT 7

#define CHATTY_CONST_KEYWORD_UPDATE "UPDATE"
#define CHATTY_CONST_KEYWORD_SIZE_UPDATE 7

#define CHATTY_CONST_KEYWORD_DELETE "DELETE"
#define CHATTY_CONST_KEYWORD_SIZE_DELETE 7

#define CHATTY_CONST_KEYWORD_CREATE "CREATE"
#define CHATTY_CONST_KEYWORD_SIZE_CREATE 7

#define CHATTY_CONST_KEYWORD_TABLE "TABLE"
#define CHATTY_CONST_KEYWORD_SIZE_TABLE 6

#define CHATTY_CONST_KEYWORD_FROM "FROM"
#define CHATTY_CONST_KEYWORD_SIZE_FROM 5

#define CHATTY_CONST_KEYWORD_WHERE "WHERE"
#define CHATTY_CONST_KEYWORD_SIZE_WHERE 6

#define CHATTY_CONST_KEYWORD_INTO "INTO"
#define CHATTY_CONST_KEYWORD_SIZE_INTO 5

#define CHATTY_CONST_KEYWORD_VALUE "VALUE"
#define CHATTY_CONST_KEYWORD_SIZE_VALUE 6

#define CHATTY_CONST_KEYWORD_VALUES "VALUES"
#define CHATTY_CONST_KEYWORD_SIZE_VALUES 7

#define CHATTY_CONST_KEYWORD_SET "SET"
#define CHATTY_CONST_KEYWORD_SIZE_SET 4

#define CHATTY_CONST_KEYWORD_EQUAL_LATTER "="
#define CHATTY_CONST_KEYWORD_SIZE_EQUAL_LATTER 2

#define CHATTY_CONST_KEYWORD_QUERY_END ";"
#define CHATTY_CONST_KEYWORD_SIZE_QUERY_END 2

#define CHATTY_CONST_KEYWORD_GROUP_OPEN_LATTER "("
#define CHATTY_CONST_KEYWORD_SIZE_OPEN_LATTER 2

#define CHATTY_CONST_KEYWORD_GROUP_CLOSE_LATTER ")"
#define CHATTY_CONST_KEYWORD_SIZE_CLOSE_LATTER 2

#define CHATTY_CONST_KEYWORD_SPOT_LATTER ","
#define CHATTY_CONST_KEYWORD_SIZE_SPOT_LATTER 2

#define CHATTY_CONST_KEYWORD_SPOT_LATTER_FOR_SINGLE ','
#define CHATTY_CONST_KEYWORD_SIZE_SPOT_LATTER 2

#define CHATTY_CONST_KEYWORD_SPACE_LATTER " "
#define CHATTY_CONST_KEYWORD_SIZE_SPACE_LATTER 2

#define CHATTY_CONST_KEYWORD_COLON_LATTER ":"
#define CHATTY_CONST_KEYWORD_SIZE_COLON_LATTER 2

/**
 * 공백크기 정의
 * */
#define CHATTY_CONST_SPACE 1

/**
 * 결과 요청용
 * */

#define CHATTY_KEYWORD_REQUEST_NULL 0
#define CHATTY_KEYWORD_REQUEST_SELECT 1
#define CHATTY_KEYWORD_REQUEST_INSERT 2
#define CHATTY_KEYWORD_REQUEST_UPDATE 3
#define CHATTY_KEYWORD_REQUEST_DELETE 4

/**
 * 검색값 따옴표 여부 정의
 * */
#define CHATTY_KEYWORD_QUOTE_TYPE_EMPTY 0
#define CHATTY_KEYWORD_QUOTE_TYPE_SINGLE 1
#define CHATTY_KEYWORD_QUOTE_TYPE_DOUBLE 2

/**
 * 검색값 따옴표 char 형에 의한 아스키 테이블 넘버 사전 정의
 * */
#define CHATTY_DATA_SPACE 32
#define CHATTY_DATA_QUOTE_SINGLE 39
#define CHATTY_DATA_QUOTE_DOUBLE 34
#define CHATTY_DATA_GRAVE 96
#define CHATTY_DATA_BACKSLASH 92


/**
 * 지금 구현된 변환 값
 * */
#define CHATTY_DB_DATA_TYPE_COUNT 3
#define CHATTY_DB_DATA_TYPE_VALUE_NULL 0
#define CHATTY_DB_DATA_TYPE_VALUE_DEFAULT 1
#define CHATTY_DB_DATA_TYPE_VALUE_INT32 2
#define CHATTY_DB_DATA_TYPE_VALUE_INT64 3
#define CHATTY_DB_DATA_TYPE_VALUE_TEXT 4


/**
 * hash key
 * */
#define CHATTY_DB_FN_HASH 0x55555555

/**
 * 단
 * 순-쿼리 생성용-데이터의 최소단위 구조체
 * */
struct CHATTY_QUERY_BLOCK {
    CHATTY_QUERY_PTR value{};
    CHATTY_SIZE value_size = 0;                                     //data size
    CHATTY_JOIN_KEYWORD join_keyword = CHATTY_KEYWORD_JOIN_NULL;    //join keyword
};


/**
 * 단순-쿼리 생성용-데이터의 최소단위를 묶는 그륩
 * */
struct CHATTY_QUERY_BOX {
    CHATTY_SIZE size = 0;               //collection size
    CHATTY_QUERY_BLOCK** value{};       //collections
};

/**
 * 쿼리를 구성하는 양식의 단위
 * */
struct CHATTY_QUERY_FORM {
    CHATTY_QUERY_BOX column_part_key;
    CHATTY_QUERY_BOX column_part;
    CHATTY_QUERY_BOX filter_part_key;
    CHATTY_QUERY_BOX filter_part;
    CHATTY_QUERY_BLOCK target_table_name;
    CHATTY_JOIN_KEYWORD request_type = CHATTY_KEYWORD_REQUEST_NULL;
};


/**
 * DB column info
 * */
struct CHATTY_DB_COLUMN_CONNECTION {
    CHATTY_UINT32 idx;
    CHATTY_UINT32 id;
    CHATTY_UINT32 latest_connect_date;
    CHATTY_FLAG is_online;
};

typedef CHATTY_DB_COLUMN_CONNECTION** CHATTY_DB_COLUMN_CONNECTION_GROUP;

struct CHATTY_DB_COLUMN_CONNECTION_GROUP_COLLECTION {
    CHATTY_DB_COLUMN_CONNECTION_GROUP value = nullptr;
    CHATTY_SIZE* size = nullptr;
};

struct CHATTY_DB_COLUMN_REGISTER_INFO {
    CHATTY_UINT32 chatty_id;
    CHATTY_UCHAR_PTR user_id;
    CHATTY_FLAG is_unsign;
    CHATTY_FLAG is_activated;
    CHATTY_UINT32 reg_date;
};

typedef CHATTY_DB_COLUMN_REGISTER_INFO** CHATTY_DB_COLUMN_REGISTER_INFO_GROUP;

struct CHATTY_DB_COLUMN_REGISTER_INFO_GROUP_COLLECTION {
    CHATTY_DB_COLUMN_REGISTER_INFO_GROUP value = nullptr;
    CHATTY_SIZE* size = nullptr;
};


struct CHATTY_DB_COLUMN_TOWER_STATUS {
    CHATTY_UINT32 idx;
    CHATTY_UCHAR_PTR hash;
    CHATTY_UINT32 id;
    CHATTY_FLAG is_open;
    CHATTY_UINT32 reg_date;
};

typedef CHATTY_DB_COLUMN_TOWER_STATUS** CHATTY_DB_COLUMN_TOWER_STATUS_GROUP;

struct CHATTY_DB_COLUMN_TOWER_STATUS_GROUP_COLLECTION {
    CHATTY_DB_COLUMN_TOWER_STATUS_GROUP value = nullptr;
    CHATTY_SIZE* size = nullptr;
};


struct CHATTY_DB_FETCH_RESULT {
    //생성자
    CHATTY_DB_FETCH_RESULT(CHATTY_SIZE cnt) : _defined_value{cnt} {};
    //소멸자
    ~CHATTY_DB_FETCH_RESULT() {
        std::cout << "→ deleted _defined_type and column_type" << '\n';
        delete[] _defined_type;
        delete[] _defined_column_type;
    };

    //템플릿으로 상수정의된 값
    CHATTY_UINT32 _defined_value = 0;
    //정의된 데이터타입과 그 값
    CHATTY_UINT32* _defined_type{nullptr};
    //정의된 메모리의 데이터 타입
    CHATTY_UINT32* _defined_column_type{nullptr};

    //결과값
    CHATTY_SIZE size = 0;
    //결과의 컬럼 갯수
    CHATTY_SIZE column_cnt = 0;
    //로우 및 컬럼별 메모리 값
    CHATTY_ANY** value = nullptr;
};

#endif //CHATTY_CHATTY_EMBEDDED_DB_TYPE_DEFS_H
