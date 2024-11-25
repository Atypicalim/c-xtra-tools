//TODO : needs """VERY HEAVY""" heavy refactoring
//TODO : maybe cache actual character count

#ifndef UTF8String_HEADER
#define UTF8String_HEADER

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>

void utf8string_to_32(const char* char_array, size_t array_size, uint32_t* ret_array, size_t* ret_array_size);
void utf16string_to_32(const uint16_t* char_array, size_t array_size, uint32_t* ret_array, size_t* ret_array_size);

void utf32string_to_8(const uint32_t* char_array, size_t array_size, uint8_t* ret_array, size_t* ret_array_size);
void utf32string_to_16(const uint32_t* char_array, size_t array_size, uint16_t* ret_array, size_t* ret_array_size);

void utf8string_to_16(const uint8_t* char_array, size_t array_size, uint16_t* ret_array, size_t* ret_array_size);
void utf16string_to_8(const uint16_t* char_array, size_t array_size, uint8_t* ret_array, size_t* ret_array_size);

typedef struct UTF8String {
    char* data;
    size_t raw_size;
    size_t data_size; //does not include null terminated character
    size_t count;
}UTF8String;

typedef struct UTF8StringView {
    char* data;
    size_t data_size; //does not include null terminated character
    size_t count;
}UTF8StringView;

size_t utf8string_count_to_byte(UTF8String* str, size_t count);
size_t utf8string_byte_to_count(UTF8String* str, size_t byte);

UTF8String* utf8string_from_cstr(const char* str);
UTF8String* utf8string_from_sv(UTF8StringView sv);

UTF8String* utf8string_sub_str(UTF8String* str, size_t from, size_t to);
UTF8String* utf8string_sub_sv(UTF8StringView sv, size_t from, size_t to);
UTF8String* utf8string_copy(UTF8String* str);

void utf8string_destroy(UTF8String* str);

size_t utf8string_count(UTF8String* str);
size_t utf8string_count_left_from(UTF8String* str, size_t from);
size_t utf8string_count_right_from(UTF8String* str, size_t from);

size_t utf8string_next(UTF8String* str, size_t pos);
size_t utf8string_prev(UTF8String* str, size_t pos);

void utf8string_set_cstr(UTF8String* str, const char* to_set);
void utf8string_set_str(UTF8String* str, UTF8String* to_set);
void utf8string_set_sv(UTF8String* str, UTF8StringView to_set);

void utf8string_append_cstr(UTF8String* str, const char* to_append);
void utf8string_append_str(UTF8String* str, UTF8String* to_append);
void utf8string_append_sv(UTF8String* str, UTF8StringView to_append);


void utf8string_insert_cstr(UTF8String* str, size_t at, const char* to_insert);
void utf8string_insert_str(UTF8String* str, size_t at, UTF8String* to_insert);
void utf8string_insert_sv(UTF8String* str, size_t at, UTF8StringView to_insert);

void utf8string_erase_range(UTF8String* str, size_t from, size_t to);
void utf8string_erase_right(UTF8String* str, size_t how_many);
void utf8string_erase_left(UTF8String* str, size_t how_many);



size_t utf8string_sv_count_to_byte(UTF8StringView sv, size_t index);
size_t utf8string_sv_byte_to_count(UTF8StringView sv, size_t index);

UTF8StringView utf8string_sv_from_cstr(const char * str);
UTF8StringView utf8string_sv_from_str(UTF8String* str);
UTF8StringView utf8string_sv_sub_str(UTF8String* str, size_t from, size_t to);
UTF8StringView utf8string_sv_sub_sv(UTF8StringView str, size_t from, size_t to);
UTF8StringView utf8string_sv_copy(UTF8StringView str);

size_t utf8string_sv_count(UTF8StringView sv);
size_t utf8string_sv_count_left_from(UTF8StringView sv, size_t from);
size_t utf8string_sv_count_right_from(UTF8StringView sv, size_t from);

size_t utf8string_sv_next(UTF8StringView sv, size_t pos);
size_t utf8string_sv_prev(UTF8StringView sv, size_t pos);

UTF8StringView utf8string_sv_trim_left(UTF8StringView sv, size_t how_many);
UTF8StringView utf8string_sv_trim_right(UTF8StringView sv, size_t how_many);

bool utf8string_sv_cmp(UTF8StringView str1, UTF8StringView str2);

int utf8string_sv_find(UTF8StringView str, UTF8StringView to_find);
int utf8string_sv_find_last(UTF8StringView str, UTF8StringView to_find);
int utf8string_sv_find_left_from(UTF8StringView str, UTF8StringView to_find, size_t from);
int utf8string_sv_find_right_from(UTF8StringView str, UTF8StringView to_find, size_t from);

bool utf8string_sv_starts_with(UTF8StringView sv, UTF8StringView with);
bool utf8string_sv_ends_with(UTF8StringView sv, UTF8StringView with);

void utf8string_sv_fprint(UTF8StringView sv, FILE* file);
void utf8string_sv_fprintln(UTF8StringView sv, FILE* file);

bool utf8string_test();

#endif
