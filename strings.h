#ifndef STRINGS_H
#define STRINGS_H

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define FG_RED  		"\033[31m"
#define FG_ORANGE		"\033[38;5;208m"

#define BOLD			"\033[1m"

#define CLEAR			"\033[0m"

#ifdef ENABLE_MSG
#define WARN(msg) fprintf(stderr, BOLD FG_ORANGE "[WARNING] " CLEAR msg "\n");
#define ERROR(msg) fprintf(stderr, BOLD FG_RED "[ERROR] " CLEAR msg "\n");
#else
#define WARN(msg)
#define ERROR(msg)

#endif

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

String *string_new(const char* c_str);
int string_append(String *str, const char* append_data);
int string_resize(String *str, size_t new_size);
void string_clear(String *str);
int string_free(String *str);

/**
 * Sized strings arent gaurenteed to be null terminated and my contain garbage if the capacity > length.
 * Use this small function to safely print a string and handle errors, Doesn't include a new line.
 *  NOTE, You can you this syntax if you need the string to be apart of a msg or format, printf will ensure only length chars are printed.
 *   printf("%.*s", (int) str->length, str->data);
 */
void string_print(String *str);

/**
 * Sized strings arent gaurenteed to be null terminated and my contain garbage if the capacity > length.
 * Use this small function to safely print a string and handle errors, Include a new line.
 *  NOTE, You can you this syntax if you need the string to be apart of a msg or format, printf will ensure only length chars are printed.
 *   printf("%.*s", (int) str->length, str->data);
 */
void string_print(String *str);

/**
 * Compares two strings, returns > 0 if equal, 0 if not equal,
 * Will return < 0 if Either string is NULL or if an error happens.
 */
int string_compare(const String *str1, const String *str2);

/**
 * Determines if a Char sequence is present in the String
 * NOT A REGGEX
 */
int string_match(String *str, const char *char_seq);

/**
 * White space is not considered alphanumeric by thi function.
 */
int string_check_alpha_numeric(const String *const str);

#ifdef STRINGS_IMPLEMENTATION

void string_println(String *str){
#ifndef DISABLE_CHECKS
	if(str == NULL){
		ERROR("From: string_print\n\tPassed in a Refrence to a NULL for String. STRING POINTER IS NOT VALID\nAppend Aborted");
		return;
	}

	if(str->data == NULL){
		WARN(
			"From: string_print\n\tStrings buffer is set to NULL. String is either: \n\tInvalid.\n\tAlready been Freed\n\tCreated Incorrectly."
			"\n\tUse a valid string or string_reuse currenting string.\nPrinting NULL"
		);
		printf("NULL\n");
		return;
	}
	if(str->length == 0){
		WARN(" Cannot print zero length String");
		return;
	}
#endif
	printf("%.*s\n", (int) str->length, str->data);
}

void string_print(String *str){
#ifndef DISABLE_CHECKS
	if(str == NULL){
		ERROR("From: string_print\n\tPassed in a Refrence to a NULL for String. STRING POINTER IS NOT VALID\nAppend Aborted");
		return;
	}

	if(str->data == NULL){
		WARN(
			"From: string_print\n\tStrings buffer is set to NULL. String is either: \n\tInvalid.\n\tAlready been Freed\n\tCreated Incorrectly."
			"\n\tUse a valid string or string_reuse currenting string.\nPrinting NULL"
		);
		printf("NULL");
		return;
	}
	if(str->length == 0){
		WARN(" Cannot print zero length String");
		return;
	}
#endif
	printf("%.*s", (int) str->length, str->data);
}

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

int string_append(String *str, const char* append_data){
#ifndef DISABLE_CHECKS
	if(str == NULL){
		ERROR("From: string_append\n\tPassed in a Refrence to a NULL for String. STRING POINTER IS NOT VALID\nAppend Aborted");
		return -1;
	}

	if(append_data == NULL){
		ERROR("From: string_append\n\t Data to append to string is a Refrence to a NULL. POINTER IS NOT VALID\nAppend Aborted");
		return -1;
	}
	if(str->data == NULL){
		WARN(
			"From: string_append\n\tStrings buffer is set to NULL. String is either: \n\tInvalid.\n\tAlready been Freed\n\tCreated Incorrectly."
			"\n\tUse a valid string or string_reuse currenting string.\n  Append Aborted"
		);
		return 0;
	}
#endif
	size_t append_size = strlen(append_data);
	if(str->capacity < str->length + append_size){
		string_ptr new_string = (string_ptr) malloc((str->length + append_size)*2);
		memcpy(new_string, str->data, str->length);
		memcpy(new_string+str->length, append_data, append_size);
		free(str->data);
		str->data = new_string;
		str->capacity = (str->length + append_size)*2;
		str->length += append_size;
		return 1;
	}
	memcpy(str->data + str->length, append_data, append_size);
	str->length += append_size;
	return 1;
}

int string_resize(String *str, size_t new_size){
#ifndef DISABLE_CHECKS
	if(str == NULL){
		ERROR("From: string_resize\n\tPassed in a Refrence to a NULL. STRING POINTER IS NOT VALID");
		return -1;
	}
#endif
	if(str->capacity < new_size){
		free(str->data);
		str->data = (string_ptr) malloc(new_size);
		return 1;
	}

	str->capacity = new_size;
	str->length = 0;
	return 1;
}

void string_clear(String *str){
#ifndef DISABLE_CHECKS
	if(str == NULL){
		ERROR("From: string_clear\n\tStrings buffer is set to NULL. String is either: \n\tInvalid.\n\tAlready been Freed\n\tCreated Incorrectly.");
		return;
	}
	if(str->data == NULL){
		WARN("From: string_clear\n\tThe String Data points to a NULL reference. You may have freed this string.");
	}
	if(str->capacity <= 0 ){
		ERROR("From string_clear\n\tString cannot be clear as it has no capacity ( <= 0). The String may have been freed or manualy altered.");
	}
#endif
	str->length = 0;
}

int string_free(String *str){
#ifndef DISABLE_CHECKS
	if(str == NULL){
		ERROR("From: string_free\n\tPassed in a Refrence to a NULL. STRING POINTER IS NOT VALID");
		return -1;
	}
#endif
	str->capacity= 0;
	str->length = 0;

#ifndef DISABLE_CHECKS
	if(str->data == NULL){
		WARN("From: string_free\n\tString data is NULL, Free has not been called, Could be caused by: \n\t    - Attempted double free. \n\t    - Changed the Pointer Manualy.");
		WARN("If the pointer has been manualy changed, You may have a memory leak.\nString free aborted");
		return 0;
	}
#endif
	free(str->data);
	str->data = NULL;
	return 1;
}

/**
 * Compares two strings, returns > 0 if equal, 0 if not equal,
 * Will return < 0 if Either string is NULL or if an error happens.
 */
int string_compare(const String *str1, const String *str2){
#ifndef DISABLE_CHECKS
	if(str1->data == NULL){
		WARN("The First String is NULL. You may have freed this String");
		return -1;
	}

	if(str2->data == NULL){
		WARN("The Second String is NULL. You may have freed this String");
		return -1;
	}
#endif
	if(str1->length != str2->length){return 0;}
	return 1;
}

/**
 * Determines if a Char sequence is present in the String
 * NOT A REGGEX
 */
int string_match(String *str, const char *char_seq){

#ifndef DISABLE_CHECKS
	if(str == NULL){
		ERROR("From: string_match\n\tPassed in a Refrence to a NULL. STRING POINTER IS NOT VALID\nMatch Aborted");
		return -1;
	}

	if(char_seq == NULL){
		ERROR("From: string_match\n\t Sequnce to mathc is a Refrence to a NULL. POINTER IS NOT VALID\nMatch Aborted");
		return -1;
	}
	if(str->data == NULL){
		WARN(
			"From: string_match\n\tStrings buffer is set to NULL. String is either: \n\tInvalid.\n\tAlready been Freed\n\tCreated Incorrectly."
			"Match Aborted"
		);
		return 0;
	}
#endif
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
#ifndef DISABLE_CHECKS
	if(str == NULL){
		ERROR("From: string_check_alpha_numeric\n\tPassed in a Refrence to a NULL for String. STRING POINTER IS NOT VALID\nMatch Aborted");
		return -1;
	}

	if(str->data == NULL){
		WARN(
			"From: string_check_alpha_numeric\n\tStrings buffer is set to NULL. String is either: \n\tInvalid.\n\tAlready been Freed\n\tCreated Incorrectly."
			"\n\tUse a valid string or string_reuse currenting string.\n  Match Aborted"
		);
		return 0;
	}
#endif
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
