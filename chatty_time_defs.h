//
// Created by my_fl on 2021-04-27.
//

#ifndef CHATTY_CHATTY_TIME_DEFS_H
#define CHATTY_CHATTY_TIME_DEFS_H

#include "chatty_embedded_db_type_defs.h"
#include "chatty_defs.h"

#define CHATTY_TIME_MS unsigned long int
#define CHATTY_TIME_MS_LDOUBLE long double
#define CHATTY_TIME_MS_DOUBLE double
#define CHATTY_TIME_VOID void

#define CHATTY_YEAR_MS 31536000000
#define CHATTY_WEAK_MS 604800000
#define CHATTY_DAY_MS 86400000
#define CHATTY_HOUR_MS 3600000
#define CHATTY_MINUTE_MS 60000
#define CHATTY_SECOND_MS 1000

namespace chatty_time_obj{

    struct ymdhis_object {
        CHATTY_UCHAR s;
        CHATTY_UCHAR mi;
        CHATTY_UCHAR h;
        CHATTY_UCHAR d;
        CHATTY_UCHAR mo;
        CHATTY_UCHAR y;
    };


    /*
     * @title-en                numeric string object
     * @title-kr                문자형식 정수 오브젝트
     * @desc                    unix millisecond since.epoch 가 13 자리 임으로 long 으로 처리되야 계산이 되는게
     *                          매우 이쁘지 못해서 한계없이 증가 및 감소가 가능한 문자열을 만들기로 함.
     * @type_info   data        it is unsigned char pointer array
     * */
    struct numeric_string_object {
        ~numeric_string_object() {
            std::cout << "@millisec object child heap node deleted / " << &data << '\n';
            delete[] data;
        };
        virtual numeric_string_object operator+(numeric_string_object& ref); //숫자 넣기
        virtual numeric_string_object operator-(numeric_string_object& ref); //숫자 뺴기
        virtual unsigned char* c_str();
        unsigned char* data{nullptr};
        unsigned int size   = 0;
        bool is_under       = false;
    };

};



#endif //CHATTY_CHATTY_TIME_DEFS_H
