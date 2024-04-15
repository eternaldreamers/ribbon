#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TOKEN_LENGTH 100

typedef enum {
    TOKEN_KEYWORD,
    TOKEN_EQUAL,
    TOKEN_LEFT_BRACKET,
    TOKEN_RIGHT_BRACKET,
    TOKEN_IDENTIFIER,
    TOKEN_END
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LENGTH];
} Token;

// Función para leer el próximo token del archivo
Token getNextToken(FILE *file) { 
    Token token;
    char c = fgetc(file);

    // Saltar espacios en blanco
    while (c == ' ' || c == '\t' || c == '\n') {
        c = fgetc(file);
    }

    // Determinar el tipo de token
    if (c == EOF) {
        token.type = TOKEN_END;
    } else if (c == '=') {
        token.type = TOKEN_EQUAL;
        token.value[0] = '=';
        token.value[1] = '\0';
    } else if (c == '[') {
        token.type = TOKEN_LEFT_BRACKET;
        token.value[0] = '[';
        token.value[1] = '\0';
    } else if (c == ']') {
        token.type = TOKEN_RIGHT_BRACKET;
        token.value[0] = ']';
        token.value[1] = '\0';
    } else if (c == '{' || c == '}') {
        // Puedes continuar agregando soporte para otros símbolos aquí
    } else {
        // Token de palabra clave o identificador
        int i = 0;
        while (c != EOF && c != ' ' && c != '\t' && c != '\n' && c != '=' && c != '[' && c != ']') {
            token.value[i++] = c;
            c = fgetc(file);
        }
        token.value[i] = '\0';
        if (strcmp(token.value, "credentials") == 0 || strcmp(token.value, "origin") == 0 || strcmp(token.value, "links") == 0) {
            token.type = TOKEN_KEYWORD;
        } else {
            token.type = TOKEN_IDENTIFIER;
        }
    }

    return token;
}

// Función principal
int main() {
    FILE *file = fopen("./example/ribbonrc", "r");
    if (file == NULL) {
        printf("Error al abrir el archivo.\n");
        return 1;
    }

    Token token;
    do {
        token = getNextToken(file);
        switch (token.type) {
            case TOKEN_KEYWORD:
                printf("Palabra clave: %s\n", token.value);
                break;
            case TOKEN_EQUAL:
                printf("Igual\n");
                break;
            case TOKEN_LEFT_BRACKET:
                printf("Corchete izquierdo\n");
                break;
            case TOKEN_RIGHT_BRACKET:
                printf("Corchete derecho\n");
                break;
            case TOKEN_IDENTIFIER:
                printf("Identificador: %s\n", token.value);
                break;
            case TOKEN_END:
                printf("Fin de archivo\n");
                break;
            default:
                printf("Token desconocido\n");
                break;
        }
    } while (token.type != TOKEN_END);

    fclose(file);
    return 0;
}
