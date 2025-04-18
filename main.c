#include <stdio.h>

#define STRINGS_IMPLEMENTATION
#include "strings.h"

int main(void){

	String *test = string_new("Test");
	String *test2 = string_new("Tost");
	printf("Alphnum: %d\n", string_check_alpha_numeric(test) );
	string_append(test, " Append Test");

	printf("Match: %d\n", string_match(test, "est"));

	printf("Compare: %d\n", string_compare(test, test2));
	printf("String: %s\nSize: %ld\nCapcity: %ld\n", test->data, test->length, test->capacity);
	printf("RUN SUCCESSFULL\n");
	return 0;
}

