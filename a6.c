#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKEN_LENGTH 100
#define MAX_TOKENS 1000
#define BUFFER_SIZE 1024

typedef enum {
    TOKEN_KEYWORD,
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_PUNCTUATION,
    TOKEN_COMMENT,
    TOKEN_WHITESPACE,
    TOKEN_UNKNOWN
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LENGTH];
} Token;

const char *keywords[] = {
    "int", "float", "char", "void", "if", "else", "return", "while", "for", NULL
};

int is_keyword(const char *word) {
    for (int i = 0; keywords[i] != NULL; i++) {
        if (strcmp(word, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

void tokenize(const char *code, Token tokens[], int *token_count) {
    const char *ptr = code;
    char current_token[MAX_TOKEN_LENGTH];
    int token_length = 0;

    while (*ptr) {
        if (isspace(*ptr)) {
            ptr++;
        } else if (*ptr == '/') {
            if (*(ptr + 1) == '/') {
                while (*ptr && *ptr != '\n') {
                    ptr++;
                }
                strcpy(tokens[*token_count].value, "COMMENT");
                tokens[*token_count].type = TOKEN_COMMENT;
                (*token_count)++;
            } else if (*(ptr + 1) == '*') {
                ptr += 2; // Skip "/*"
                while (*ptr && !(*ptr == '*' && *(ptr + 1) == '/')) {
                    ptr++;
                }
                ptr += 2; // Skip "*/"
                strcpy(tokens[*token_count].value, "COMMENT");
                tokens[*token_count].type = TOKEN_COMMENT;
                (*token_count)++;
            } else {
                current_token[token_length++] = *ptr++;
                current_token[token_length] = '\0';
                strcpy(tokens[*token_count].value, current_token);
                tokens[*token_count].type = TOKEN_OPERATOR;
                (*token_count)++;
            }
        } else if (isalpha(*ptr) || *ptr == '_') {
            token_length = 0;
            while (isalnum(*ptr) || *ptr == '_') {
                current_token[token_length++] = *ptr++;
            }
            current_token[token_length] = '\0';
            if (is_keyword(current_token)) {
                strcpy(tokens[*token_count].value, current_token);
                tokens[*token_count].type = TOKEN_KEYWORD;
            } else {
                strcpy(tokens[*token_count].value, current_token);
                tokens[*token_count].type = TOKEN_IDENTIFIER;
            }
            (*token_count)++;
        } else if (isdigit(*ptr)) {
            token_length = 0;
            while (isdigit(*ptr)) {
                current_token[token_length++] = *ptr++;
            }
            current_token[token_length] = '\0';
            strcpy(tokens[*token_count].value, current_token);
            tokens[*token_count].type = TOKEN_NUMBER;
            (*token_count)++;
        } else {
            current_token[0] = *ptr++;
            current_token[1] = '\0';
            strcpy(tokens[*token_count].value, current_token);
            tokens[*token_count].type = TOKEN_PUNCTUATION;
            (*token_count)++;
        }
    }
}

void write_tokens_to_file(Token tokens[], int token_count) {
    FILE *file = fopen("output.txt", "w");
    if (!file) {
        perror("Unable to open output.txt");
        exit(1);
    }
    for (int i = 0; i < token_count; i++) {
        fprintf(file, "%d: %s\n", tokens[i].type, tokens[i].value);
    }
    fclose(file);
}

int main() {
    FILE *file = fopen("input.txt", "r");
    if (!file) {
        perror("Unable to open input.txt");
        return 1;
    }

    char buffer[BUFFER_SIZE];
    char code[BUFFER_SIZE * 10] = ""; // Adjust size as needed
    while (fgets(buffer, BUFFER_SIZE, file)) {
        strcat(code, buffer);
    }
    fclose(file);

    Token tokens[MAX_TOKENS];
    int token_count = 0;

    tokenize(code, tokens, &token_count);
    
    if (token_count > 0) {
        write_tokens_to_file(tokens, token_count);
    } else {
        printf("No tokens found.\n");
    }

    return 0;
}
