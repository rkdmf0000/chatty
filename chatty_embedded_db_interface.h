//
// Created by my_fl on 2021-02-18.
//

#ifndef CHATTY_CHATTY_EMBEDDED_DB_INTERFACE_H
#define CHATTY_CHATTY_EMBEDDED_DB_INTERFACE_H

#include "chatty_embedded_db_includes.h"
#include "chatty_embedded_db_type_defs.h"
#include <iostream>

class chatty_embedded_db_interface {
public:
    CHATTY_STATIC CHATTY_SIZE controller_cnt;
public:
    CHATTY_SIZE strlen(CHATTY_UCHAR_PTR x);
};


#endif //CHATTY_CHATTY_EMBEDDED_DB_INTERFACE_H
