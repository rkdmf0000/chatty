//
// Created by my_fl on 2021-02-21.
//

#ifndef CHATTY_CHATTY_EMBEDDED_DB_CONTROLLER_H
#define CHATTY_CHATTY_EMBEDDED_DB_CONTROLLER_H

#include "chatty_embedded_db_master.h"

class chatty_embedded_db_controller : public chatty_embedded_db_master {
public:
    chatty_embedded_db_controller() = default;
    ~chatty_embedded_db_controller() = default;

    /**
     * CONNECTIONS
     * */
    CHATTY_ANY use(const CHATTY_STRING&);
    CHATTY_ANY close();

    CHATTY_ANY handler();
    CHATTY_ANY handle_finalize();


    /**
     * COMMON
     * */
    CHATTY_ANY behavior(CHATTY_UCHAR type);
    CHATTY_ANY column(CHATTY_UCHAR_PTR column);
    CHATTY_ANY column(CHATTY_UCHAR_PTR column, CHATTY_UCHAR_PTR value);

    CHATTY_ERROR_CODE exec();

    /**
     * REQUESTS-SEARCH
     * */
    CHATTY_ANY select(CHATTY_UCHAR_PTR table);
    CHATTY_ANY where_and(CHATTY_UCHAR_PTR column, CHATTY_UCHAR_PTR value);
    CHATTY_ANY where_or(CHATTY_UCHAR_PTR column, CHATTY_UCHAR_PTR value);


    /**
     * REQUESTS-ACTION
     * */
    CHATTY_ANY delete_search(const CHATTY_UCHAR_PTR column, const CHATTY_UCHAR_PTR value);

private:
    /**
     * generate query function
     * */
    CHATTY_ANY _generate_select_column_common();
    CHATTY_ANY _generate_where_common();
    CHATTY_ANY _generate_non_value_grouping_column_key();
    CHATTY_ANY _generate_non_value_grouping_column();


};


#endif //CHATTY_CHATTY_EMBEDDED_DB_CONTROLLER_H
