//
// Created by my_fl on 2021-04-27.
//

#ifndef CHATTY_CHATTY_TIME_DEFS_H
#define CHATTY_CHATTY_TIME_DEFS_H
#include "chatty_embedded_db_type_defs.h"
#include "chatty_defs.h"
struct chatty_time_object {
    CHATTY_UCHAR s;
    CHATTY_UCHAR mi;
    CHATTY_UCHAR h;
    CHATTY_UCHAR d;
    CHATTY_UCHAR mo;
    CHATTY_UCHAR y;
};

#endif //CHATTY_CHATTY_TIME_DEFS_H
