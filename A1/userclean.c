/*
* Group Work!
* Code completed by Dusan Barudzija.
* Connor Carroll assisting in demo and setup Github init for collaboration 
* in following assignments and labs. 
 * Student Name: Dusan Barudzija
 * Student ID: [YOUR_STUDENT_ID]
 * Submission Date: 2026-05-04
 * File Name: userclean.c
 * Description: Command-line utility that validates and cleans usernames
 *              read from standard input, one per line.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MIN_LEN 3
#define MAX_LEN 16
#define PAD_STR "user"

/* is_valid: checks if a username meets all validity criteria.
 * params: username - the string to check
 * return: 1 if valid, 0 otherwise
 */
int is_valid(const char *username) {
    size_t len = strlen(username);

    if (strcmp(username, "root") == 0 || strcmp(username, "admin") == 0) {
        return 0;
    }

    if (len < MIN_LEN || len > MAX_LEN) {
        return 0;
    }

    if (!islower((unsigned char)username[0])) {
        return 0;
    }

    for (size_t i = 0; i < len; i++) {
        char c = username[i];
        if (!islower((unsigned char)c) && !isdigit((unsigned char)c) && c != '_') {
            return 0;
        }
    }

    return 1;
}

/* transform: applies transformation rules to produce a cleaned username.
 * params: input - the original username string
 * return: a newly allocated transformed string (caller must free)
 */
char *transform(const char *input) {
    size_t input_len = strlen(input);

    /* Step 1 & 2 & 3: lowercase, whitespace->underscore, remove invalid chars */
    char *buf = malloc(input_len + 1);
    if (!buf) {
        return NULL;
    }

    size_t buf_len = 0;
    for (size_t i = 0; i < input_len; i++) {
        unsigned char c = (unsigned char)input[i];
        if (isspace(c)) {
            buf[buf_len++] = '_';
        } else if (isalpha(c)) {
            buf[buf_len++] = (char)tolower(c);
        } else if (isdigit(c) || c == '_') {
            buf[buf_len++] = (char)c;
        }
        /* else: discard */
    }
    buf[buf_len] = '\0';

    /* Step 4: pad with "user" if shorter than MIN_LEN */
    size_t pad_len = strlen(PAD_STR);
    size_t pad_idx = 0;
    while (buf_len < MIN_LEN) {
        char *tmp = realloc(buf, buf_len + 2);
        if (!tmp) {
            free(buf);
            return NULL;
        }
        buf = tmp;
        buf[buf_len++] = PAD_STR[pad_idx % pad_len];
        buf[buf_len] = '\0';
        pad_idx++;
    }

    /* Step 5: truncate to MAX_LEN */
    if (buf_len > MAX_LEN) {
        buf[MAX_LEN] = '\0';
    }

    return buf;
}

/* strip_newline: removes trailing newline/carriage return from a string in place.
 * params: s - the string to modify
 */
void strip_newline(char *s) {
    size_t len = strlen(s);
    while (len > 0 && (s[len - 1] == '\n' || s[len - 1] == '\r')) {
        s[--len] = '\0';
    }
}

/* read_line: reads a full line from stdin dynamically.
 * return: newly allocated string, or NULL on EOF/error
 */
char *read_line(void) {
    size_t capacity = 64;
    size_t len = 0;
    char *buf = malloc(capacity);
    if (!buf) {
        return NULL;
    }

    int c;
    while ((c = getchar()) != EOF && c != '\n') {
        if (len + 1 >= capacity) {
            capacity *= 2;
            char *tmp = realloc(buf, capacity);
            if (!tmp) {
                free(buf);
                return NULL;
            }
            buf = tmp;
        }
        buf[len++] = (char)c;
    }

    if (c == EOF && len == 0) {
        free(buf);
        return NULL;
    }

    buf[len] = '\0';
    return buf;
}

int main(void) {
    char *line;

    while ((line = read_line()) != NULL) {
        strip_newline(line);

        if (is_valid(line)) {
            printf("%s : %s\n", line, line);
        } else {
            char *cleaned = transform(line);
            if (!cleaned) {
                free(line);
                fprintf(stderr, "Memory allocation error\n");
                return EXIT_FAILURE;
            }

            if (is_valid(cleaned)) {
                printf("%s : %s\n", line, cleaned);
            } else {
                printf("%s : invalid and unfixable\n", line);
            }

            free(cleaned);
        }

        free(line);
    }

    return EXIT_SUCCESS;
}
