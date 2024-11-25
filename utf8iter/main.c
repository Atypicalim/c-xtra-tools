#include "utf8iter.h"

#include <stdio.h>
#include <string.h>

int main() {

	const char* String = "Hello World, Γεια σου κόσμο, こんにちは世界, привет мир.";
	const char* Character;

	UTF8Iter ITER;

	utf8iter_init(&ITER, String);

	printf("\nString = %s\n\n", ITER.ptr);

	while (utf8iter_next(&ITER)) {

		Character = utf8iter_getchar(&ITER);

		printf("Character =  %s\t Codepoint =  %u\t\t BYTES:  ", Character, ITER.codepoint);

		for (int i = 0; i < 8; i++) {
			if (Character[i] == 0) break;
			printf("%u  ", (unsigned char)Character[i]);
		}

		printf("\n");
	}

	printf("\n");
	printf("Character Count = %u\n", ITER.count);
	printf("Length in BYTES = %u\n", ITER.length);

	return 0;
}
