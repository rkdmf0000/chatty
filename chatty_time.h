//
// Created by my_fl on 2021-04-27.
//

#ifndef CHATTY_CHATTY_TIME_MASTER_H
#define CHATTY_CHATTY_TIME_MASTER_H

#include "chatty_embedded_db_type_defs.h"
#include "chatty_time_defs.h"
#include "chatty_numeric_string.h"
#include <ctime> //time.h
#include <sys/time.h>
#include <chrono>
#include <cmath>
#include <memory>


class chatty_time_master {
public:
    CHATTY_STATIC unsigned char* get_millisec();

};



#endif //CHATTY_CHATTY_TIME_MASTER_H
