//
// Created by my_fl on 2021-04-27.
//

#ifndef CHATTY_CHATTY_TIME_MASTER_H
#define CHATTY_CHATTY_TIME_MASTER_H

#include "chatty_embedded_db_type_defs.h"
#include "chatty_time_defs.h"
#include <ctime> //time.h
#include <sys/time.h>
#include <chrono>
#include <cmath>

class chatty_time_master {
public:
    CHATTY_STATIC CHATTY_CONST unsigned char in_month[];

    CHATTY_STATIC chatty_time_obj::numeric_string_object* get_millisec();

    CHATTY_STATIC CHATTY_TIME_VOID print_millisec_object(chatty_time_obj::numeric_string_object* object_);

    CHATTY_STATIC CHATTY_TIME_VOID conv2time_object(chatty_time_obj::ymdhis_object& time_object, CHATTY_TIME_MS ms);

};


#endif //CHATTY_CHATTY_TIME_MASTER_H
