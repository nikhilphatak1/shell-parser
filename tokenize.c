/*
Nikhil Phatak
tokenize.c
*/
// initial author: Nat Tuck.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "tokenize.h"

int
type(char c) {
    return !((c == '<')
          || (c == '>')
          || (c == '|')
          || (c == '&')
          || (c == ';'));
}

int
is_op(char c) {
    return !type(c);
}

int
is_char(char c) {
    return type(c);
}

char** get_tokens(char* word) {
    char** strings = malloc(20*sizeof(char*));
    for (int i = 0; i < 20; ++i) {
        strings[i] = malloc(20*sizeof(char));
    }

    // j indexes word
    // k indexes strings
    // l indexes tokens
    int j = 0;
    int k = 0;
    int l = 0;

    while (word[j]) {
        if (word[j + 1]) {
            char b = word[j];
            char c = word[j + 1];
            strings[k][l] = b;
            ++l;

            if ((is_char(b) && is_op(c))
            || (is_op(b) && is_char(c))) {
                strings[k][l] = '\0';
                ++k;
                l = 0;
            }
        } else {
            strings[k][l] = word[j];
            ++l;
        }
        ++j;
    }
    strings[k][l] = '\0';
    ++k;
    strcpy(strings[k]," ");
    return strings;

}

char**
tokenize(char* line) {
    char space_chars[] = " \n\r\t";
    char* tokens[80];
    char** end_tokens = malloc(20*sizeof(char*));
    for (int j = 0; j < 20; ++j) {
        end_tokens[j] = malloc(20*sizeof(char));
    }
    tokens[0] = strtok(line,space_chars);

    int ii = 0;
    while (tokens[ii] != NULL) {

        ++ii;
        tokens[ii] = strtok(NULL,space_chars);

    }

    // for indexing end_tokens
    int aa = 0;

    // print tokens
    for (int kk = 0; kk < ii; kk++) {
        char** sub_tok = get_tokens(tokens[kk]);
        int idx = 0;

        while (strcmp(sub_tok[idx]," ")) {
            strcpy(end_tokens[aa],sub_tok[idx]);
            ++aa;
            ++idx;
        }

        for (int i = 0; i < 20; ++i) {
            free(sub_tok[i]);
        }
        free(sub_tok);
    }

    strcpy(end_tokens[aa]," ");
    return end_tokens;

}
