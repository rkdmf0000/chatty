//
// Created by my_fl on 2021-02-17.
//

#include "chatty_embedded_db_request.h"

chatty_embedded_db_request::chatty_embedded_db_request() {
    this->init();
};

chatty_embedded_db_request::~chatty_embedded_db_request() {
    delete[] this->conditions;
    delete[] this->condition_char_length_bukkit;
};

CHATTY_ANY chatty_embedded_db_request::init() {
    this->conditions = nullptr;
    this->conditions_idx = 0;
    this->condition_char_length_bukkit = nullptr;
    this->condition_char_length_bukkit_d1_idx = 0;
    this->condition_char_length_bukkit_d2_idx = 0;
};
