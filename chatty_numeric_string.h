//
// Created by my_fl on 2021-05-10.
//

#ifndef CHATTY_CHATTY_NUMERIC_STRING_H
#define CHATTY_CHATTY_NUMERIC_STRING_H

typedef unsigned char* chatty_ns_data;
typedef unsigned int chatty_ns_size;

class chatty_numeric_string {
public:
    chatty_numeric_string(void) = default;

    //정수를 unsigned char 로 변환하고 heap 영역에 저장
    chatty_numeric_string(int);
    chatty_numeric_string(unsigned int);

    //stack 값을 heap 영역에 복사
    chatty_numeric_string(char);
    chatty_numeric_string(unsigned char);

    //값을 복사
    chatty_numeric_string(char*);
    chatty_numeric_string(unsigned char*);

    //소멸자
    ~chatty_numeric_string();

    //값을 더하고 빼고
    virtual chatty_numeric_string operator+(); // plus
    virtual chatty_numeric_string operator-(); // minus

    //get, set
    void set_size(chatty_ns_size size);
    void set_data(chatty_ns_data data);



private:
    //옳바르지 않은 문자열을 검증 및 초기화
    void _mem_validate_up();

private:
    chatty_ns_size size;
    chatty_ns_size part_length;

    chatty_ns_data data;
    chatty_ns_data* part;
};


#endif //CHATTY_CHATTY_NUMERIC_STRING_H
