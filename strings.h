#ifndef STRINGS_H
#define STRINGS_H

#include <stddef.h>
#include <stdlib.h>
#include <string.h>


typedef size_t u64 ;

typedef char* string_ptr;
typedef const char *const string_view_ptr;

typedef struct {
	string_ptr data;
	size_t capacity;
	size_t length;
}String;

typedef struct {
	string_view_ptr data;
	size_t length;
}string_view;

typedef struct {
	string_ptr data;
	size_t capacity;
	size_t length;
	u64 hash;
}fast_strings;

const static size_t STRING_SIZE = sizeof(String);
const static size_t STRING_VIEW_SIZE = sizeof(string_view);

/*String *new_string(const char* c_str){
	size_t string_size = strlen(c_str) + 1;
	char *new_string = (char*) malloc(string_size);
	memcpy(new_string, c_str, string_size);
	String *str = (String *) malloc(STRING_SIZE);
	return str;
}*/

#ifdef STRINGS_IMPLEMENTATION

String *string_new(const char* c_str){
	size_t string_size = strlen(c_str);
	char *new_string = (char*) malloc(string_size+1);
	memcpy(new_string, c_str, string_size);
	String *str = (String *) malloc(STRING_SIZE);
	str->data = new_string;
	str->length = string_size;
	str->capacity = string_size;
	return str;
}

void string_append(String *str, const char* append_data){
	size_t append_size = strlen(append_data);
	if(str->capacity < str->length + append_size){
		string_ptr new_string = (string_ptr) malloc((str->length + append_size)*2);
		memcpy(new_string, str->data, str->length);
		memcpy(new_string+str->length, append_data, append_size);
		free(str->data);
		str->data = new_string;
		str->capacity = (str->length + append_size)*2;
		str->length += append_size;
		return;
	}
	memcpy(str->data + str->length, append_data, append_size);
	str->length += append_size;
}

void string_resize(String *str, size_t new_size){
	if(str->capacity < new_size){
		free(str->data);
		str->data = (string_ptr) malloc(new_size);
		return;
	}

	str->capacity = new_size;
	str->length = 0;
}

void string_clear(String *str){
	str->length = 0;
}

void string_free(String *str){
	free(str->data);
	str->capacity= 0;
	str->length = 0;
}

int string_compare(const String *str1, const String *str2){
	if(str1->length != str2->length){return 0;}
}

/**
 * Determines if a Char sequence is present in the String
 * NOT A REGGEX
 */
int string_match(String *str, const char *char_seq){
	size_t seq_size = strlen(char_seq);
	if(seq_size > str->length) return 0;

	for(size_t i = 0; i < str->length; i++){
		if(str->data[i] != char_seq[0]){
			continue;
		}
		size_t k = 0;
		for( /*Nyan~*/; i < str->length && k < seq_size; i++, k++){
			if(str->data[i] != char_seq[k])
				break;
		}
		if(k == seq_size) return 1;
	}

	return 0;
}
/**
 * White space is not considered alphanumeric by thi function.
 */
int string_check_alpha_numeric(const String *const str){
	for(size_t i = 0; i < str->length; i++){
		const char c = str->data[i];
		if( !((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9')) ){
			return 0;
		}
	}
	return 1;
}

#endif

#endif
