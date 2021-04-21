//
// Created by my_fl on 2021-04-21.
//아직 암것도 안함ㅋㅋ

#ifndef CHATTY_CHATTY_NETWORK_MASTER_H
#define CHATTY_CHATTY_NETWORK_MASTER_H

#include "chatty_embedded_db_includes.h"
#include "chatty_embedded_db_type_defs.h"

#include "chatty_embedded_db_master.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>


class chatty_network_master {
public:
    CHATTY_STATIC CHATTY_ANY open(CHATTY_UINT32 port);
};


#endif //CHATTY_CHATTY_NETWORK_MASTER_H
