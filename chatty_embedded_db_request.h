//
// Created by my_fl on 2021-02-17.
//

#ifndef CHATTY_CHATTY_EMBEDDED_DB_REQUEST_H
#define CHATTY_CHATTY_EMBEDDED_DB_REQUEST_H

#include "chatty_embedded_db_type_defs.h"

class chatty_embedded_db_request {
private:
    CHATTY_CHAR_COLLECTOR conditions{};
    CHATTY_INT32 conditions_idx{};
    CHATTY_NUMERIC_ARRAY_COLLECTOR condition_char_length_bukkit = new int*(nullptr);
    CHATTY_INT32 condition_char_length_bukkit_d1_idx{}, condition_char_length_bukkit_d2_idx{};
public:
    chatty_embedded_db_request();
    ~chatty_embedded_db_request();
    CHATTY_ANY init();
    CHATTY_ANY sel();
    CHATTY_ANY inst();
    CHATTY_ANY upd();
    CHATTY_ANY del();
};


#endif //CHATTY_CHATTY_EMBEDDED_DB_REQUEST_H
