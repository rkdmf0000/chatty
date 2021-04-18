//
// Created by my_fl on 2021-02-18.
//

#include "chatty_embedded_db_interface.h"

/**
 * Use by maintenance.
 * this will must be zero when something activated off.
 * */
CHATTY_SIZE chatty_embedded_db_interface::controller_cnt = 0;

CHATTY_SIZE chatty_embedded_db_interface::strlen(CHATTY_UCHAR_PTR x) {
    CHATTY_INT32 i(0);
    while(x[i])
        ++i;
    return i;
}
