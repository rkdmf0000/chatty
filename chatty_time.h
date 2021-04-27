//
// Created by my_fl on 2021-04-27.
//

#ifndef CHATTY_CHATTY_TIME_H
#define CHATTY_CHATTY_TIME_H

#include "chatty_embedded_db_type_defs.h"
#include "chatty_defs.h"
#include <ctime> //time.h

class chatty_time {
    CHATTY_CONST CHATTY_UINT32 get_millisec();
};


#endif //CHATTY_CHATTY_TIME_H
