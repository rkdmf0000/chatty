//
// Created by my_fl on 2021-04-27.
//

#include "chatty_time.h"

using std::cout; using std::endl;
using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::seconds;
using std::chrono::system_clock;

CHATTY_CONST unsigned char chatty_time_master::in_month[12] = {31,30,31,30,31,31,30,31,30,31,31,29};

chatty_time_obj::numeric_string_object chatty_time_obj::numeric_string_object::operator+(numeric_string_object &ref) {
    return chatty_time_obj::numeric_string_object();
}

chatty_time_obj::numeric_string_object chatty_time_obj::numeric_string_object::operator-(numeric_string_object &ref) {
    return chatty_time_obj::numeric_string_object();
}

/*
 * 밀리초 값에 대한 정보를 chatty_time_obj::millisec_object 로 담는 함수입니다.
 * heap 영역에 메로리 하는 만드는데 value by reference 이고
 * chatty_time_obj::millisec_object 의 소멸자로 heap 영역 메모리가 제거됩니다.
 * */
chatty_time_obj::numeric_string_object* chatty_time_master::get_millisec() {
    auto millisec_since_epoch = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
    unsigned int count = log10(millisec_since_epoch) + 1;
    unsigned char* new_ = new unsigned char[count+1];
    unsigned int idx(0);
    while (millisec_since_epoch != 0) {
        unsigned char buff_data = (unsigned char)millisec_since_epoch % 10;
        new_[count - idx - 1] = buff_data;
        millisec_since_epoch /= 10;
        ++idx;
    };
    new_[count] = '\0';
    chatty_time_obj::numeric_string_object* ma = new chatty_time_obj::numeric_string_object();
    ma->size = count;
    ma->data = new_;
    //send by value
    return ma;
}

CHATTY_TIME_VOID chatty_time_master::print_millisec_object(chatty_time_obj::numeric_string_object* object_) {
    std::cout << "milli-sec size : " << object_->size << '\n';
    std::cout << "value is : ";
    char* b = (char*)object_->data;
    for(unsigned int i=0;i<object_->size;++i) {
        std::cout << (int)b[i];
    };
    std::cout << std::endl;
};

CHATTY_TIME_VOID chatty_time_master::conv2time_object(chatty_time_obj::ymdhis_object& time_object, CHATTY_TIME_MS ms) {

};