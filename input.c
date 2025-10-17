#include <stdio.h>
#include <stdlib.h>
#include "error_codes.h"

Err input_int(unsigned int *x) {
    int result;
    char chr = '\0';
    do {
        result = scanf("%u", x);
        if (result == 1 && *x >= 0) {
            if (scanf("%c", &chr) == 1 && chr == '\n') { 
                return ERR_OK;
            } else {
                printf("Ошибка: попробуйте еще раз.\n");
                scanf("%*[^\n]%*c");
            }
        } else if (result == EOF) {
            printf("Ввод завершён (EOF).\n");
            return ERR_EOF;
        } else {
            printf("Ошибка: некорректный ввод. Попробуйте снова ввести число (положительное больше 0).\n");
            scanf("%*[^\n]%*c");
        }
    } while (1);
}
