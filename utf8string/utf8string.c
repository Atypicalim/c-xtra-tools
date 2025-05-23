﻿#include "UTF8String.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

///////////////////////
// utf8 utills
///////////////////////

void dump_binary(uint32_t to_dump) {
    for (int i = 32-1; i >= 0; i--) {
        uint32_t copy = to_dump >> i;
        copy = copy & 1;
        if (copy == 1) {
            putchar('1');
        }
        else  {
            putchar('0');
        }
    }
    putchar('\n');
}


void utf8string_to_32(const char* char_array, size_t array_size, uint32_t* ret_array, size_t* ret_array_size)
{
    //if ret_array_size is zero or ret_array is NULL, then calculate the array size
    if (*ret_array_size == 0 || ret_array == NULL) {
        size_t ret_size = 0;

        for (size_t i = 0; i < array_size;) {
            uint8_t byte = char_array[i];
            if ((byte & 0b10000000) == 0) {
                i+=1;
            }
            else if ((byte & 0b11100000) == 0b11000000) {
                i += 2;
            }
            else if ((byte & 0b11110000) == 0b11100000) {
                i += 3;
            }
            else  {
                i += 4;
            }
            ret_size++;
        }

        *ret_array_size = ret_size;
        return;
    }

    size_t ret_arr_index = 0;

    for (size_t i = 0; i < array_size;) {
        uint8_t byte = char_array[i];
        uint32_t character = 0;

        if ((byte & 0b10000000) == 0) {
            character = byte;
            i += 1;
        }
        else if ((byte & 0b11100000) == 0b11000000) {
            character |= (char_array[i + 0] & 0b00011111) << 6;
            character |= (char_array[i + 1] & 0b00111111) << 0;
            i += 2;
        }
        else if ((byte & 0b11110000) == 0b11100000) {
            character |= (char_array[i + 0] & 0b00001111) << 12;
            character |= (char_array[i + 1] & 0b00111111) << 6;
            character |= (char_array[i + 2] & 0b00111111) << 0;
            i += 3;
        }
        else {
            character |= (char_array[i + 0] & 0b00000111) << 18;
            character |= (char_array[i + 1] & 0b00111111) << 12;
            character |= (char_array[i + 2] & 0b00111111) << 6;
            character |= (char_array[i + 3] & 0b00111111) << 0;
            i += 4;
        }
        ret_array[ret_arr_index] = character;
        ret_arr_index++;
        if (ret_arr_index >= *ret_array_size) {
            break;
        }
    }
}

void utf16string_to_32(const uint16_t* char_array, size_t array_size, uint32_t* ret_array, size_t* ret_array_size)
{
    //if ret_array_size is zero or ret_array is NULL, then calculate the array size
    if (*ret_array_size == 0 || ret_array == NULL) {
        size_t ret_size = 0;

        for (size_t i = 0; i < array_size;) {
            uint16_t code = char_array[i];
            if ((code & 0b1111110000000000) == 0b1101100000000000) { //check if it's leading surrogate
                i += 2;
            }
            else {
                i += 1;
            }
            ret_size++;
        }

        *ret_array_size = ret_size;
        return;
    }

    size_t ret_arr_index = 0;

    for (size_t i = 0; i < array_size;) {

        uint16_t code = char_array[i];
        uint32_t character = 0;

        if ((code & 0b1111110000000000) == 0b1101100000000000) { //check if it's surrogate pair
            character |= (char_array[i] - 0xD800) << 10;
            character |= (char_array[i+1] - 0xDC00);
            character += 0x10000;
            i += 2;
        }
        else {
            character = char_array[i];
            i += 1;
        }
        ret_array[ret_arr_index] = character;

        ret_arr_index++;
        if (ret_arr_index >= *ret_array_size) {
            break;
        }
    }
}

void utf32string_to_8(const uint32_t* char_array, size_t array_size, uint8_t* ret_array, size_t* ret_array_size)
{
    //if ret_array_size is zero or ret_array is NULL, then calculate the array size
    if (*ret_array_size == 0 || ret_array == NULL) {
        size_t ret_size = 0;

        for (size_t i = 0; i < array_size; i++) {
            uint32_t code = char_array[i];
            if (code >= 0x10000) {
                ret_size += 4;
            }
            else if (code >= 0x800) {
                ret_size += 3;
            }
            else if (code >= 0x80) {
                ret_size += 2;
            }
            else {
                ret_size += 1;
            }
        }

        *ret_array_size = ret_size;
        return;
    }

    size_t ret_arr_index = 0;

    for (size_t i = 0; i < array_size; i++) {
        uint32_t code = char_array[i];

        if (code >= 0x10000) {
            ret_array[ret_arr_index++] = 0b11110000 | ((code >> 18) & 0b00000111);
            ret_array[ret_arr_index++] = 0b10000000 | ((code >> 12) & 0b00111111);
            ret_array[ret_arr_index++] = 0b10000000 | ((code >> 6)  & 0b00111111);
            ret_array[ret_arr_index++] = 0b10000000 | ((code >> 0)  & 0b00111111);
        }
        else if (code >= 0x800) {
            ret_array[ret_arr_index++] = 0b11100000 | ((code >> 12) & 0b00001111);
            ret_array[ret_arr_index++] = 0b10000000 | ((code >> 6)  & 0b00111111);
            ret_array[ret_arr_index++] = 0b10000000 | ((code >> 0)  & 0b00111111);
        }
        else if (code >= 0x80) {
            ret_array[ret_arr_index++] = 0b11000000 | ((code >> 6) & 0b00011111);
            ret_array[ret_arr_index++] = 0b10000000 | ((code >> 0) & 0b00111111);
        }
        else {
            ret_array[ret_arr_index++] = code;
        }
        if (ret_arr_index >= *ret_array_size) {
            break;
        }
    }

    return;
}
void utf32string_to_16(const uint32_t* char_array, size_t array_size, uint16_t* ret_array, size_t* ret_array_size)
{
    //if ret_array_size is zero or ret_array is NULL, then calculate the array size

    //you might notice that we encode from U+D800 to U+DFFF to a single unpaired surrogate,
    //this is intentional since we are not checking if utf32 string is valid
    //we are just converting them

    if (*ret_array_size == 0 || ret_array == NULL) {
        size_t ret_size = 0;

        for (size_t i = 0; i < array_size; i++) {
            uint32_t code = char_array[i];
            if (code >= 0x10000) {
                ret_size += 2;
            }
            else {
                ret_size += 1;
            }
        }

        *ret_array_size = ret_size;
        return;
    }

    size_t ret_arr_index = 0;

    for (size_t i = 0; i < array_size; i++) {
        uint32_t code = char_array[i];
        if (code >= 0x10000) {
            code -= 0x10000;
            ret_array[ret_arr_index++] = (code >> 10) + 0xD800;
            ret_array[ret_arr_index++] = (code & 0b00000000001111111111) + 0xDC00;
        }
        else {
            ret_array[ret_arr_index++] = code;
        }

        if (ret_arr_index >= *ret_array_size) {
            break;
        }
    }

    return;
}

void utf8string_to_16(const uint8_t* char_array, size_t array_size, uint16_t* ret_array, size_t* ret_array_size)
{
    //if ret_array_size is zero or ret_array is NULL, then calculate the array size
    if (*ret_array_size == 0 || ret_array == NULL) {
        size_t ret_size = 0;

        for (size_t i = 0; i < array_size;) {
            uint8_t byte = char_array[i];
            if ((byte & 0b10000000) == 0) {
                ret_size += 1;
                i += 1;
            }
            else if ((byte & 0b11100000) == 0b11000000) {
                ret_size += 1;
                i += 2;
            }
            else if ((byte & 0b11110000) == 0b11100000) {
                ret_size += 1;
                i += 3;
            }
            else {
                ret_size+=2;
                i += 4;
            }
        }

        *ret_array_size = ret_size;
        return;
    }

    size_t ret_arr_index = 0;

    for (size_t i = 0; i < array_size;) {
        uint8_t byte = char_array[i];
        uint32_t utf32_code = 0;

        if ((byte & 0b10000000) == 0) {
            utf32_code = byte;
            i += 1;
        }
        else if ((byte & 0b11100000) == 0b11000000) {
            utf32_code |= (char_array[i + 0] & 0b00011111) << 6;
            utf32_code |= (char_array[i + 1] & 0b00111111) << 0;
            i += 2;
        }
        else if ((byte & 0b11110000) == 0b11100000) {
            utf32_code |= (char_array[i + 0] & 0b00001111) << 12;
            utf32_code |= (char_array[i + 1] & 0b00111111) << 6;
            utf32_code |= (char_array[i + 2] & 0b00111111) << 0;
            i += 3;
        }
        else {
            utf32_code |= (char_array[i + 0] & 0b00000111) << 18;
            utf32_code |= (char_array[i + 1] & 0b00111111) << 12;
            utf32_code |= (char_array[i + 2] & 0b00111111) << 6;
            utf32_code |= (char_array[i + 3] & 0b00111111) << 0;
            i += 4;
        }

        //convert utf32 to 16
        if (utf32_code >= 0x10000) {
            utf32_code -= 0x10000;
            ret_array[ret_arr_index++] = (utf32_code >> 10) + 0xD800;
            ret_array[ret_arr_index++] = (utf32_code & 0b00000000001111111111) + 0xDC00;
        }
        else {
            ret_array[ret_arr_index++] = utf32_code;
        }

        if (ret_arr_index >= *ret_array_size) {
            break;
        }
    }
}

void utf16string_to_8(const uint16_t* char_array, size_t array_size, uint8_t* ret_array, size_t* ret_array_size)
{
    //if ret_array_size is zero or ret_array is NULL, then calculate the array size
    if (*ret_array_size == 0 || ret_array == NULL) {
        size_t ret_size = 0;

        for (size_t i = 0; i < array_size;) {
            uint16_t code = char_array[i];
            if ((code & 0b1111110000000000) == 0b1101100000000000) { //check if it's leading surrogate
                i += 2;
                ret_size+=4;
            }
            else {
                if (code >= 0x0800) {
                    ret_size += 3;
                }
                else if (code >= 0x080) {
                    ret_size += 2;
                }
                else {
                    ret_size += 1;
                }
                i += 1;
            }

        }

        *ret_array_size = ret_size;
        return;
    }

    size_t ret_arr_index = 0;

    for (size_t i = 0; i < array_size;) {

        uint16_t code = char_array[i];
        uint32_t utf32_code = 0;

        if ((code & 0b1111110000000000) == 0b1101100000000000) { //check if it's surrogate pair
            utf32_code |= (char_array[i] - 0xD800) << 10;
            utf32_code |= (char_array[i + 1] - 0xDC00);
            utf32_code += 0x10000;
            i += 2;
        }
        else {
            utf32_code = char_array[i];
            i += 1;
        }

        //convert utf32 to utf8
        if (utf32_code >= 0x10000) {
            ret_array[ret_arr_index++] = 0b11110000 | ((utf32_code >> 18) & 0b00000111);
            ret_array[ret_arr_index++] = 0b10000000 | ((utf32_code >> 12) & 0b00111111);
            ret_array[ret_arr_index++] = 0b10000000 | ((utf32_code >> 6) & 0b00111111);
            ret_array[ret_arr_index++] = 0b10000000 | ((utf32_code >> 0) & 0b00111111);
        }
        else if (utf32_code >= 0x800) {
            ret_array[ret_arr_index++] = 0b11100000 | ((utf32_code >> 12) & 0b00001111);
            ret_array[ret_arr_index++] = 0b10000000 | ((utf32_code >> 6) & 0b00111111);
            ret_array[ret_arr_index++] = 0b10000000 | ((utf32_code >> 0) & 0b00111111);
        }
        else if (utf32_code >= 0x80) {
            ret_array[ret_arr_index++] = 0b11000000 | ((utf32_code >> 6) & 0b00011111);
            ret_array[ret_arr_index++] = 0b10000000 | ((utf32_code >> 0) & 0b00111111);
        }
        else {
            ret_array[ret_arr_index++] = utf32_code;
        }

        if (ret_arr_index >= *ret_array_size) {
            break;
        }
    }
}


//https://stackoverflow.com/questions/32936646/getting-the-string-length-on-utf-8-in-c#:~:text=The%20number%20of%20code%20points,and%20stopping%20at%20'%5C0'%20.
size_t utf8_get_length(const char* str) {
    size_t count = 0;
    while (*str) {
        count += (*str++ & 0xC0) != 0x80;
    }
    return count;
}

#define CHK 1

bool utf8string_is_valid(UTF8String *str){
#if CHK
    if( str->count != utf8_get_length(str->data)){
        fprintf(stderr, "%s:%d:ERROR : str is not valid!!!", __FILE__, __LINE__);
        fprintf(stderr, " cached count : %zu, count : %zu\n", str->count, utf8string_count(str));
        return false;
    }
    if (str->data[str->data_size] != 0) {
        fprintf(stderr, "%s:%d:ERROR : str is not valid!!!", __FILE__, __LINE__);
        fprintf(stderr, " str is not null terminated");
        return false;
    }
#endif
    return true;
}

bool utf8string_sv_is_valid(UTF8StringView sv){
#if CHK
    if( sv.count != utf8string_sv_count(sv)){
        fprintf(stderr, "%s:%d:ERROR : sv is not valid!!!", __FILE__, __LINE__);
        fprintf(stderr, " cached count : %zu, count : %zu\n", sv.count, utf8string_sv_count(sv));
        return false;
    }
#endif
    return true;
}

///////////////////////
// UTF8String functions
///////////////////////

size_t utf8string_count_to_byte(UTF8String* str, size_t count)
{
    return utf8string_sv_count_to_byte(utf8string_sv_from_str(str), count);
}

size_t utf8string_byte_to_count(UTF8String* str, size_t byte)
{
    return utf8string_sv_byte_to_count(utf8string_sv_from_str(str), byte);
}

#define UTF_STR_DEFAULT_ALLOC 128

size_t calculate_size(size_t needed_size) {
    size_t current_size = UTF_STR_DEFAULT_ALLOC;
    //TODO : there probably was a better way to calculate this number
    //without using while loop.....
    while (current_size < needed_size) {
        current_size *= 2;
    }
    return current_size;
}

void utf8string_grow(UTF8String* str, size_t needed_size) {
    size_t calculated_needed = calculate_size(needed_size);
    if (str->raw_size < calculated_needed) {
        uint8_t* new_block = realloc(str->data, calculated_needed);
        if (new_block) {
            str->data = new_block;
            str->raw_size = calculated_needed;
        }
        else {
            fprintf(stderr, "%s:%d:ERROR : failed to grow a string!!!\n", __FILE__, __LINE__);
        }
    }
}

UTF8String* utf8string_from_cstr(const char* str) {
    UTF8String* to_return = malloc(sizeof(UTF8String));

    //copy str
    if (str) {
        size_t str_len = strlen(str);
        size_t null_included = str_len + 1;

        to_return->raw_size = calculate_size(null_included);

        to_return->data = malloc(to_return->raw_size);
        to_return->data_size = str_len;
        memcpy(to_return->data, str, null_included);
        to_return->count = utf8_get_length(str);
    }
    else {
        to_return->raw_size = UTF_STR_DEFAULT_ALLOC;
        to_return->data = malloc(to_return->raw_size);
        to_return->data[0] = 0;
        to_return->data_size = 0;
        to_return->count = 0;
    }

    utf8string_is_valid(to_return);

    return to_return;
}

UTF8String* utf8string_from_sv(UTF8StringView sv)
{
    UTF8String* to_return = malloc(sizeof(UTF8String));

    size_t str_len = sv.data_size;
    size_t null_included = str_len + 1;

    to_return->raw_size = calculate_size(null_included);

    to_return->data = malloc(to_return->raw_size);
    to_return->data_size = str_len;
    memcpy(to_return->data, sv.data, str_len);
    to_return->count = sv.count;

    //make it null terminated
    to_return->data[to_return->data_size] = 0;

    utf8string_is_valid(to_return);

    return to_return;
}

UTF8String* utf8string_sub_str(UTF8String* str, size_t from, size_t to)
{
    //TODO : maybe implement actual function to reduce overhead
    return utf8string_from_sv(utf8string_sv_sub_str(str, from, to));
}

UTF8String* utf8string_sub_sv(UTF8StringView sv, size_t from, size_t to)
{
    return utf8string_from_sv(utf8string_sv_sub_sv(sv, from, to));
}

UTF8String* utf8string_copy(UTF8String* str)
{
    //TODO : maybe implement actual function to reduce overhead
    return utf8string_from_sv(utf8string_sv_from_str(str));
}

void utf8string_destroy(UTF8String* str) {
    if (!str) { return; }
    if (str->data) { free(str->data); }
    free(str);
}

size_t utf8string_count(UTF8String* str)
{
    return utf8string_sv_count(utf8string_sv_from_str(str));
}

size_t utf8string_count_left_from(UTF8String* str, size_t from)
{
    return utf8string_sv_count_left_from(utf8string_sv_from_str(str), from);
}

size_t utf8string_count_right_from(UTF8String* str, size_t from)
{
    return utf8string_sv_count_right_from(utf8string_sv_from_str(str), from);
}

size_t utf8string_next(UTF8String* str, size_t pos)
{
    return utf8string_sv_next(utf8string_sv_from_str(str), pos);
}

size_t utf8string_prev(UTF8String* str, size_t pos)
{
    return utf8string_sv_prev(utf8string_sv_from_str(str), pos);
}

void utf8string_set_cstr(UTF8String* str, const char* to_set)
{
    size_t str_len = strlen(to_set);
    size_t null_included = str_len + 1;

    utf8string_grow(str, null_included);

    memcpy(str->data, to_set, null_included);

    str->data_size = str_len;
    str->count = utf8_get_length(to_set);

    utf8string_is_valid(str);
}
void utf8string_set_str(UTF8String* str, UTF8String* to_set)
{
    size_t str_len = to_set->data_size;
    size_t null_included = str_len+1;

    utf8string_grow(str, null_included);

    memcpy(str->data, to_set->data, null_included);

    str->data_size = str_len;
    str->count = to_set->count;

    utf8string_is_valid(str);
}
void utf8string_set_sv(UTF8String* str, UTF8StringView to_set)
{
    size_t str_len = to_set.data_size;
    size_t null_included = str_len+1;

    utf8string_grow(str, null_included);

    memcpy(str->data, to_set.data, null_included);

    str->data_size = str_len;
    str->count = to_set.count;

    utf8string_is_valid(str);
}

void utf8string_append_cstr(UTF8String* str, const char* to_append) {
    size_t str_len = strlen(to_append);
    size_t null_included = str_len + 1;

    utf8string_grow(str, null_included + str->data_size);

    memcpy(str->data + str->data_size, to_append, null_included);
    str->data_size += str_len;

    size_t str_count = utf8_get_length(to_append);
    str->count += str_count;

    utf8string_is_valid(str);
}

void utf8string_append_str(UTF8String* str, UTF8String* to_append)
{
    utf8string_append_sv(str, utf8string_sv_from_str(to_append));
}

void utf8string_append_sv(UTF8String* str, UTF8StringView to_append)
{
    size_t str_len = to_append.data_size;
    size_t null_included = str_len + 1;

    utf8string_grow(str, null_included + str->data_size);

    memcpy(str->data + str->data_size, to_append.data, str_len);
    str->data_size += str_len;

    str->data[str->data_size] = 0;

    str->count += to_append.count;

    utf8string_is_valid(str);
}

void utf8string_insert_cstr(UTF8String* str, size_t at, const char* to_insert) {
    size_t char_count = str->count;
    if (at > char_count) {
        fprintf(stderr, "%s:%d:ERROR : at(%zu) is bigger than string size(%zu)\n", __FILE__, __LINE__, at, char_count);
        return;
    }

    at = utf8string_count_to_byte(str, at);

    size_t str_len = strlen(to_insert);
    size_t null_included = str_len + 1;

    utf8string_grow(str, null_included + str->data_size);

    //last+1 > at instead of last >= at because it wraps aroud when at is 0
    for (size_t last = str->data_size - 1; last + 1 > at; last--) {
        str->data[last + str_len] = str->data[last];
    }

    memcpy(str->data + at, to_insert, str_len);
    str->data_size += str_len;
    str->data[str->data_size] = 0;

    size_t str_count = utf8_get_length(to_insert);
    str->count += str_count;

    utf8string_is_valid(str);
}

void utf8string_insert_str(UTF8String* str, size_t at, UTF8String* to_insert)
{
    utf8string_insert_sv(str, at, utf8string_sv_from_str(to_insert));
}


void utf8string_insert_sv(UTF8String* str, size_t at, UTF8StringView to_insert)
{
    size_t char_count = str->count;
    if (at > char_count) {
        fprintf(stderr, "%s:%d:ERROR : at(%zu) is bigger than string size(%zu)\n", __FILE__, __LINE__, at, char_count);
        return;
    }

    at = utf8string_count_to_byte(str, at);

    size_t str_len = to_insert.data_size;
    size_t null_included = str_len + 1;

    utf8string_grow(str, null_included + str->data_size);

    //last+1 > at instead of last >= at because it wraps aroud when at is 0
    /*for (size_t last = str->data_size - 1; last + 1 > at; last--) {
        str->data[last + str_len] = str->data[last];
    }*/
    memmove(str->data + at + to_insert.data_size, str->data + at, str->data_size - at);

    memcpy(str->data + at, to_insert.data, str_len);

    str->data_size += str_len;
    str->data[str->data_size] = 0;

    str->count += to_insert.count;

    utf8string_is_valid(str);
}

void utf8string_erase_range(UTF8String* str, size_t from, size_t to) {
    if (to <= from) {
        size_t tmp = to;
        to = from;
        from = tmp;
    }
    if (to - from >= str->count) {
        str->data_size = 0;
        str->count = 0;
        str->data[0] = 0;
        return;
    }

    size_t char_count = str->count;

    if(char_count <= to){to = char_count;}
    if(char_count <= from){from = char_count;}

    str->count -= to - from;

    from = utf8string_count_to_byte(str, from);
    to = utf8string_count_to_byte(str, to);

    size_t distance = to - from;

    /*for (size_t i = from; i < str->data_size; i++) {
        str->data[i] = str->data[i + distance];
    }*/
    memmove(str->data+from, str->data + distance + from, str->data_size - from);

    str->data_size -= distance;
    str->data[str->data_size] = 0;

    utf8string_is_valid(str);
}

void utf8string_erase_right(UTF8String* str, size_t how_many)
{
    size_t str_count = str->count;
    if (how_many >= str_count) {
        str->data_size = 0;
        str->data[str->data_size] = 0;
        str->count = 0;
        return;
    }

    str->count -= how_many;

    how_many = str_count - how_many;

    how_many = utf8string_count_to_byte(str, how_many);

    str->data_size = how_many;
    str->data[str->data_size] = 0;

    utf8string_is_valid(str);
}

void utf8string_erase_left(UTF8String* str, size_t how_many)
{
    if (how_many >= str->count) {
        str->data_size = 0;
        str->data[str->data_size] = 0;
        str->count = 0;
        return;
    }

    str->count -= how_many;

    how_many = utf8string_count_to_byte(str, how_many);

    /*for (size_t i = how_many; i < str->data_size; i++) {
        str->data[i - how_many] = str->data[i];
    }*/
    memmove(str->data, str->data + how_many, str->data_size - how_many);
    str->data_size -= how_many;
    str->data[str->data_size] = 0;

    utf8string_is_valid(str);
}

////////////////////////////
// String View Functions
////////////////////////////

size_t utf8string_sv_count_to_byte(UTF8StringView sv, size_t count)
{
    if (count == 0) {
        return 0;
    }
    count++;
    for(size_t i=0; i< sv.data_size; i++){
        if ((sv.data[i] & 0b11000000) != 0b10000000) {
            count--;
        }
        if(count == 0){
            return i;
        }
    }
    return sv.data_size;
}

size_t utf8string_sv_byte_to_count(UTF8StringView sv, size_t byte)
{
    size_t count = 0;
    for(size_t i=0; i< byte; i++){
        if((sv.data[i] & 0b11000000) != 0b10000000){
            count++;
        }
    }

    return count;
}

UTF8StringView utf8string_sv_from_cstr(const char* str)
{
    UTF8StringView sv = { .data = str, sv.data_size = strlen(str), .count = utf8_get_length(str)};
    return sv;
}

UTF8StringView utf8string_sv_from_str(UTF8String* str) {
    UTF8StringView sv = {.data = str->data, .data_size = str->data_size, .count = str->count};
    return sv;
}

UTF8StringView utf8string_sv_sub_str(UTF8String* str, size_t from, size_t to)
{
    size_t sv_count = str->count;
    if (sv_count <= to) {
        to = sv_count;
    }

    if (sv_count <= from) {
        from = sv_count;
    }

    if (to < from) {
        size_t tmp = to;
        to = from;
        from = tmp;
    }

    UTF8StringView sv;

    sv.count = to - from;

    if (from <= 0) {
        from = 0;
    }
    else {
        from = utf8string_count_to_byte(str, from);
    }
    if (to >= str->count) {
        to = str->data_size;
    }
    else {
        to = utf8string_count_to_byte(str, to);
    }

    sv.data_size = to - from;
    sv.data = str->data + from;

    utf8string_sv_is_valid(sv);

    return sv;
}

UTF8StringView utf8string_sv_sub_sv(UTF8StringView str, size_t from, size_t to)
{
    size_t sv_count = str.count;
    if (sv_count <= to) {
        to = sv_count;
    }

    if (sv_count <= from) {
        from = sv_count;
    }

    if (to < from) {
        size_t tmp = to;
        to = from;
        from = tmp;
    }

    UTF8StringView sv;
    sv.count = to - from;

    if (from <= 0) {
        from = 0;
    }
    else {
        from = utf8string_sv_count_to_byte(str, from);
    }
    if (to >= str.count) {
        to = str.data_size;
    }
    else {
        to = utf8string_sv_count_to_byte(str, to);
    }


    sv.data_size = to - from;
    sv.data = str.data + from;

    utf8string_sv_is_valid(sv);

    return sv;
}

UTF8StringView utf8string_sv_copy(UTF8StringView sv)
{
    UTF8StringView copy = { .data = sv.data, .data_size = sv.data_size, .count = sv.count};
    return copy;
}

size_t utf8string_sv_count(UTF8StringView sv)
{
    size_t count = 0;
    for (size_t i = 0; i < sv.data_size; i++) {
        if ((sv.data[i] & 0b11000000) != 0b10000000) {
            count++;
        }
    }

    return count;
}

size_t utf8string_sv_count_left_from(UTF8StringView sv, size_t from)
{
    size_t count = 0;
    char* ptr = sv.data + from;
    while (from--) {
        count += (*ptr-- & 0xC0) != 0x80;
    }
    return count;
}

size_t utf8string_sv_count_right_from(UTF8StringView sv, size_t from)
{
    size_t count = 0;
    char* ptr = sv.data + from;
    while (from++ < sv.data_size) {
        count += (*ptr++ & 0xC0) != 0x80;
    }
    return count;
}

size_t utf8string_sv_next(UTF8StringView sv, size_t pos)
{
    if (pos >= sv.data_size) {
        return sv.data_size;
    }

    if ((sv.data[pos] & 0b11000000) != 0b10000000) {
        pos++;
    }
    while (pos < sv.data_size && (sv.data[pos] & 0b11000000) == 0b10000000) {
        pos++;
    }
    return pos;
}

size_t utf8string_sv_prev(UTF8StringView sv, size_t pos)
{
    if (pos == 0) {
        return 0;
    }
    if (pos > sv.data_size) {
        pos = sv.data_size;
    }

    if ((sv.data[pos] & 0b11000000) != 0b10000000) {
        pos--;
    }
    while (pos > 0 && (sv.data[pos] & 0b11000000) == 0b10000000) {
        pos--;
    }
    return pos;
}

UTF8StringView utf8string_sv_trim_left(UTF8StringView sv, size_t how_many)
{
    size_t sv_count = sv.count;
    if (how_many >= sv_count) {
        sv.data_size = 0;
        sv.count = 0;
        return sv;
    }

    sv.count -= how_many;
    how_many = utf8string_sv_count_to_byte(sv, how_many);

    sv.data += how_many;
    sv.data_size -= how_many;

    utf8string_sv_is_valid(sv);

    return sv;
}

UTF8StringView utf8string_sv_trim_right(UTF8StringView sv, size_t how_many)
{
    size_t sv_count = sv.count;

    if (how_many >= sv_count) {
        sv.data_size = 0;
        sv.count = 0;
        return sv;
    }

    sv.count -= how_many;

    how_many = sv_count - how_many;
    how_many = utf8string_sv_count_to_byte(sv, how_many);

    sv.data_size = how_many;

    utf8string_sv_is_valid(sv);

    return sv;
}

bool utf8string_sv_cmp(UTF8StringView str1, UTF8StringView str2) {
    if (str1.data_size != str2.data_size) {
        return false;
    }

    for (size_t i = 0; i < str1.data_size; i++) {
        if (str1.data[i] != str2.data[i]) {
            return false;
        }
    }
    return true;
}

int utf8string_sv_find(UTF8StringView str, UTF8StringView to_find)
{
    if (str.data_size < to_find.data_size) {
        return -1;
    }

    int char_count = 0;
    size_t byte_offset = 0;

    while (byte_offset + to_find.data_size <= str.data_size) {
        UTF8StringView sub = {.data = str.data + byte_offset, .data_size = to_find.data_size};
        if (utf8string_sv_cmp(sub, to_find)) {
            return utf8string_sv_byte_to_count(str, byte_offset);
        }
        byte_offset++;
    }
    return -1;
}

int utf8string_sv_find_last(UTF8StringView str, UTF8StringView to_find) {
    if (str.data_size < to_find.data_size) {
        return -1;
    }

    int char_count = 0;
    int byte_offset = str.data_size - to_find.data_size;

    while (byte_offset >= 0) {
        UTF8StringView sub = { .data = str.data + byte_offset, .data_size = to_find.data_size };
        if (utf8string_sv_cmp(sub, to_find)) {
            return utf8string_sv_byte_to_count(str, byte_offset);
        }
        byte_offset--;
    }
    return -1;
}

int utf8string_sv_find_left_from(UTF8StringView str, UTF8StringView to_find, size_t from)
{
    UTF8StringView sub = utf8string_sv_sub_sv(str, 0, from);
    return utf8string_sv_find_last(sub, to_find);
}

int utf8string_sv_find_right_from(UTF8StringView str, UTF8StringView to_find, size_t from)
{
    UTF8StringView sub = utf8string_sv_sub_sv(str, from, str.count);
    int found_at = utf8string_sv_find(sub, to_find);
    if(found_at < 0){
        return -1;
    }
    return found_at + from;
}

bool utf8string_sv_starts_with(UTF8StringView sv, UTF8StringView with)
{
    return (utf8string_sv_find(sv, with) == 0);
}

bool utf8string_sv_ends_with(UTF8StringView sv, UTF8StringView with)
{
    return (utf8string_sv_find_last(sv, with) == (sv.count - with.count));
}

void utf8string_sv_fprint(UTF8StringView sv, FILE* file)
{
    for (int i = 0; i < sv.data_size; i++) {
        fputc(sv.data[i], file);
    }
}

void utf8string_sv_fprintln(UTF8StringView sv, FILE* file)
{
    for (int i = 0; i < sv.data_size; i++) {
        fputc(sv.data[i], file);
    }
    fputc('\n', file);
}

////////////////////////////
// Tests
////////////////////////////

static bool utf16_strcmp(uint16_t* str1, uint16_t* str2) {
    size_t index = 0;
    while (str1[index] != 0 && str2[index] != 0) {
        uint16_t char1 = str1[index];
        uint16_t char2 = str2[index];
        if (char1 != char2) {
            return false;
        }
        index++;
    }
    return true;
}

static bool utf32_strcmp(uint32_t* str1, uint32_t* str2) {
    size_t index = 0;
    while (str1[index] != 0 && str2[index] != 0) {
        uint32_t char1 = str1[index];
        uint32_t char2 = str2[index];
        if (char1 != char2) {
            return false;
        }
        index++;
    }
    return true;
}

bool utf8string_test()
{
    {
        UTF8String* str = utf8string_from_cstr(u8"abcde");
        assert(utf8string_sv_cmp(utf8string_sv_sub_str(str, 0, 1), utf8string_sv_from_cstr(u8"a")));
        utf8string_erase_left(str, 1);
        assert(utf8string_sv_cmp(utf8string_sv_sub_str(str, 0, 1), utf8string_sv_from_cstr(u8"b")));
        utf8string_erase_left(str, 1);
        assert(utf8string_sv_cmp(utf8string_sv_sub_str(str, 0, 1), utf8string_sv_from_cstr(u8"c")));
        utf8string_erase_left(str, 1);
        assert(utf8string_sv_cmp(utf8string_sv_sub_str(str, 0, 1), utf8string_sv_from_cstr(u8"d")));
        utf8string_erase_left(str, 1);
        assert(utf8string_sv_cmp(utf8string_sv_sub_str(str, 0, 1), utf8string_sv_from_cstr(u8"e")));
        utf8string_erase_left(str, 1);
        assert(str->count == 0);
        utf8string_destroy(str);
    }

    {
        UTF8String* str = utf8string_from_cstr(u8"abcde");
        assert(utf8string_sv_cmp(utf8string_sv_sub_str(str, str->count - 1, str->count), utf8string_sv_from_cstr(u8"e")));
        utf8string_erase_right(str, 1);
        assert(utf8string_sv_cmp(utf8string_sv_sub_str(str, str->count - 1, str->count), utf8string_sv_from_cstr(u8"d")));
        utf8string_erase_right(str, 1);
        assert(utf8string_sv_cmp(utf8string_sv_sub_str(str, str->count - 1, str->count), utf8string_sv_from_cstr(u8"c")));
        utf8string_erase_right(str, 1);
        assert(utf8string_sv_cmp(utf8string_sv_sub_str(str, str->count - 1, str->count), utf8string_sv_from_cstr(u8"b")));
        utf8string_erase_right(str, 1);
        assert(utf8string_sv_cmp(utf8string_sv_sub_str(str, str->count - 1, str->count), utf8string_sv_from_cstr(u8"a")));
        utf8string_erase_right(str, 1);
        assert(str->count == 0);
        utf8string_destroy(str);
    }

    {
        UTF8StringView str = utf8string_sv_from_cstr(u8"abcde");
        assert(utf8string_sv_cmp(utf8string_sv_sub_sv(str, 0, 1), utf8string_sv_from_cstr(u8"a")));
        str = utf8string_sv_trim_left(str, 1);
        assert(utf8string_sv_cmp(utf8string_sv_sub_sv(str, 0, 1), utf8string_sv_from_cstr(u8"b")));
        str = utf8string_sv_trim_left(str, 1);
        assert(utf8string_sv_cmp(utf8string_sv_sub_sv(str, 0, 1), utf8string_sv_from_cstr(u8"c")));
        str = utf8string_sv_trim_left(str, 1);
        assert(utf8string_sv_cmp(utf8string_sv_sub_sv(str, 0, 1), utf8string_sv_from_cstr(u8"d")));
        str = utf8string_sv_trim_left(str, 1);
        assert(utf8string_sv_cmp(utf8string_sv_sub_sv(str, 0, 1), utf8string_sv_from_cstr(u8"e")));
        str = utf8string_sv_trim_left(str, 1);
        assert(str.count == 0);
    }

    {
        UTF8StringView str = utf8string_sv_from_cstr(u8"abcde");
        assert(utf8string_sv_cmp(utf8string_sv_sub_sv(str, str.count-1, str.count), utf8string_sv_from_cstr(u8"e")));
        str = utf8string_sv_trim_right(str, 1);
        assert(utf8string_sv_cmp(utf8string_sv_sub_sv(str, str.count - 1, str.count), utf8string_sv_from_cstr(u8"d")));
        str = utf8string_sv_trim_right(str, 1);
        assert(utf8string_sv_cmp(utf8string_sv_sub_sv(str, str.count - 1, str.count), utf8string_sv_from_cstr(u8"c")));
        str = utf8string_sv_trim_right(str, 1);
        assert(utf8string_sv_cmp(utf8string_sv_sub_sv(str, str.count - 1, str.count), utf8string_sv_from_cstr(u8"b")));
        str = utf8string_sv_trim_right(str, 1);
        assert(utf8string_sv_cmp(utf8string_sv_sub_sv(str, str.count - 1, str.count), utf8string_sv_from_cstr(u8"a")));
        str = utf8string_sv_trim_right(str, 1);
        assert(str.count == 0);
    }

    {
        UTF8StringView sv = utf8string_sv_from_cstr(u8"Long Text");
        assert(utf8string_sv_find(sv, utf8string_sv_from_cstr(u8"does not exist")) < 0);
        assert(utf8string_sv_find_last(sv, utf8string_sv_from_cstr(u8"does not exist")) < 0);

        UTF8StringView seven = utf8string_sv_from_cstr(u8"!seven!");
        assert(utf8string_sv_count(seven) == 7);
        size_t middle = utf8string_sv_find(seven, utf8string_sv_from_cstr(u8"v"));
        assert(middle == 3);
    }


    {
        UTF8String* short_str = utf8string_from_cstr(u8"short");
        utf8string_erase_left(short_str, 2);
        utf8string_erase_right(short_str, 2);
        assert(utf8string_sv_cmp(utf8string_sv_from_str(short_str), utf8string_sv_from_cstr(u8"o")));
        assert(utf8string_count(short_str) == 1);
        assert(short_str->count == 1);
        utf8string_destroy(short_str);
    }

    {
        UTF8String* short_str = utf8string_from_cstr(u8"short");
        utf8string_erase_range(short_str, 0, 2);
        utf8string_erase_range(short_str, 1, 2);
        assert(utf8string_sv_cmp(utf8string_sv_from_str(short_str), utf8string_sv_from_cstr(u8"ot")));
        utf8string_erase_range(short_str, 0, 0);
        assert(utf8string_sv_cmp(utf8string_sv_from_str(short_str), utf8string_sv_from_cstr(u8"ot")));
        utf8string_erase_range(short_str, short_str->count, short_str->count);
        assert(utf8string_sv_cmp(utf8string_sv_from_str(short_str), utf8string_sv_from_cstr(u8"ot")));
        utf8string_destroy(short_str);
    }
    {
        ////////////////////////////////
        // utf8string_to_32 test
        ////////////////////////////////
        uint32_t utf32_vec[16];
        size_t utf32_vec_size = 0;

        const char str[] = u8"a߿일😀";
        utf8string_to_32(str, sizeof(str), utf32_vec, &utf32_vec_size);
        assert(utf32_vec_size == 5); //cause it's null terminated
        utf8string_to_32(str, sizeof(str), utf32_vec, &utf32_vec_size);
        assert(utf32_strcmp(utf32_vec, U"a߿일😀"));
    }
    {
        ////////////////////////////////
        // utf16string_to_32 test
        ////////////////////////////////
        uint32_t utf32_vec[16];
        size_t utf32_vec_size = 0;

        const uint16_t str[] = u"a😀";
        utf16string_to_32(str, (sizeof(str) / sizeof(uint16_t)), utf32_vec, &utf32_vec_size);
        assert(utf32_vec_size == 3); //cause it's null terminated
        utf16string_to_32(str, (sizeof(str) / sizeof(uint16_t)), utf32_vec, &utf32_vec_size);
        assert(utf32_strcmp(utf32_vec, U"a😀"));
    }
    {
        ////////////////////////////////
        // utf32string_to_8 test
        ////////////////////////////////
        uint8_t utf8_vec[16];
        size_t utf8_vec_size = 0;

        const uint32_t str[] = U"a߿일😀";
        utf32string_to_8(str, (sizeof(str) / sizeof(uint32_t)), utf8_vec, &utf8_vec_size);
        assert(utf8_vec_size == 11);
        utf32string_to_8(str, (sizeof(str) / sizeof(uint32_t)), utf8_vec, &utf8_vec_size);
        assert(strcmp(utf8_vec, u8"a߿일😀") == 0);
        utf8_vec_size = 0;
    }
    {
        ////////////////////////////////
        // utf32string_to_16 test
        ////////////////////////////////
        uint16_t utf16_vec[16];
        size_t utf16_vec_size = 0;

        const uint32_t str[] = U"a😀";
        utf32string_to_16(str, (sizeof(str) / sizeof(uint32_t)), utf16_vec, &utf16_vec_size);
        assert(utf16_vec_size == 4);
        utf32string_to_16(str, (sizeof(str) / sizeof(uint32_t)), utf16_vec, &utf16_vec_size);
        assert(utf16_strcmp(utf16_vec, u"a😀"));
        utf16_vec_size = 0;
    }
    {
        ////////////////////////////////
        // utf8string_to_32 test
        ////////////////////////////////
        uint32_t utf32_vec[16];
        size_t utf32_vec_size = 0;

        const char str[] = u8"a߿일😀";
        utf8string_to_32(str, sizeof(str), utf32_vec, &utf32_vec_size);
        assert(utf32_vec_size == 5); //cause it's null terminated
        utf8string_to_32(str, sizeof(str), utf32_vec, &utf32_vec_size);
        assert(utf32_strcmp(utf32_vec, U"a߿일😀"));
    }
    {
        ////////////////////////////////
        // utf8string_to_16 test
        ////////////////////////////////
        uint16_t utf16_vec[16];
        size_t utf16_vec_size = 0;

        const char str[] = u8"a߿일😀";
        utf8string_to_16(str, sizeof(str), utf16_vec, &utf16_vec_size);
        assert(utf16_vec_size == 6);
        utf8string_to_16(str, sizeof(str), utf16_vec, &utf16_vec_size);
        assert(utf16_strcmp(utf16_vec, u"a߿일😀"));
    }
    {
        ////////////////////////////////
        // utf16string_to_8 test
        ////////////////////////////////
        uint8_t utf8_vec[16];
        size_t utf8_vec_size = 0;

        const uint16_t str[] = u"a߿일😀";
        utf16string_to_8(str, (sizeof(str) / sizeof(uint16_t)), utf8_vec, &utf8_vec_size);
        assert(utf8_vec_size == 11);
        utf16string_to_8(str, (sizeof(str) / sizeof(uint16_t)), utf8_vec, &utf8_vec_size);
        assert(strcmp(utf8_vec, u8"a߿일😀") == 0);
    }
    {
        UTF8String* str = utf8string_from_cstr(u8"random string");
        utf8string_set_cstr(str, u8"고양이");
        assert(utf8string_sv_cmp(utf8string_sv_from_str(str), utf8string_sv_from_cstr(u8"고양이")));
        utf8string_destroy(str);
    }


    return true;
}


