#include <stdio.h>

#define STRINGS_IMPLEMENTATION
#include "strings.h"

///This is just a macro i made to make printing info easier
#define STRING_PRETTY_PRINTF(string) printf("  String: %.*s\n    len: %ld\n    Capcity: %ld\n", (int)string.length, string.data, string.length, string.capacity);

int main(void){

	String string = string_new("String");
	STRING_PRETTY_PRINTF(string);

	string_append(&string, " appending to string");
	STRING_PRETTY_PRINTF(string);

	string_free(&string);

	String *string_ptr = string_new_ptr("Ptr to a string");

	STRING_PRETTY_PRINTF((*string_ptr));

	string_append(string_ptr, " appending to string pointer");
	STRING_PRETTY_PRINTF((*string_ptr));

	string_free(string_ptr);
	free(string_ptr);

	printf("RUN SUCCESSFULL\n");
	return 0;
}

