#include <errno.h>
#include <stdio.h>
#include <limits.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

int is_strict_integer(const char *str){
    if (str == NULL || *str == '\0')
        return -1;

    for (int i = 0; str[i] != '\0'; ++i) {
        if (!isdigit((unsigned char) str[i]))
            return -1;
    }
    return 1;
}


int parse_strict_positive_integer(const char *str){
    if (is_strict_integer(str) == -1)
        return -1;

    int result = atoi(str);
    return result;
}

int main(){
    char* distance;
    scanf("%s", distance);
    printf("\n");
    printf("%d", parse_strict_positive_integer(distance));
}