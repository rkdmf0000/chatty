//
// Created by my_fl on 2021-04-27.
//

#include "chatty_time.h"
#include "chatty_time_defs.h"


using std::cout; using std::endl;
using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::seconds;
using std::chrono::system_clock;

unsigned char* chatty_time_master::get_millisec() {
    auto millisec_since_epoch = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
    unsigned int count = log10(millisec_since_epoch) + 1;
    unsigned char* new_ = new unsigned char[count+1];
    unsigned int idx(0);
    while (millisec_since_epoch != 0) {
        unsigned char buff_data = (unsigned char)millisec_since_epoch % 10;
        new_[count - idx - 1] = (buff_data == 0 ? 48 : buff_data);
        millisec_since_epoch /= 10;
        ++idx;
    };
    new_[count] = '\0';
    return new_; //NRVO
};
