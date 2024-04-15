#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TOKEN_LENGTH 100

// Enumeración para los tipos de tokens
typedef enum {
    TOKEN_KEYWORD,
    TOKEN_EQUAL,
    TOKEN_LEFT_BRACKET,
    TOKEN_RIGHT_BRACKET,
    TOKEN_IDENTIFIER,
    TOKEN_STRING,
    TOKEN_ARRAY,
    TOKEN_END
} TokenType;

// Estructura para representar un token
typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LENGTH];
} Token;

// Estructura para representar las credenciales
typedef struct {
    char user[MAX_TOKEN_LENGTH];
    char pass[MAX_TOKEN_LENGTH];
} Credentials;

// Estructura para representar la información de origen
typedef struct {
    char ref[MAX_TOKEN_LENGTH];
    char branch[MAX_TOKEN_LENGTH];
} Origin;

// Estructura para representar los enlaces
typedef struct {
    char links[MAX_TOKEN_LENGTH][MAX_TOKEN_LENGTH];
    int count;
} Links;

// Estructura que contiene toda la información
typedef struct {
    Credentials credentials;
    Origin origin;
    Links links;
} ABC;

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
    } else if (c == '{') {
        token.type = TOKEN_LEFT_BRACKET;
        token.value[0] = '{';
        token.value[1] = '\0';
    } else if (c == '}') {
        token.type = TOKEN_RIGHT_BRACKET;
        token.value[0] = '}';
        token.value[1] = '\0';
    } else if (c == '[') {
        token.type = TOKEN_ARRAY;
        token.value[0] = '[';
        token.value[1] = '\0';
    } else if (c == ']') {
        // Token para el final de la lista
        token.type = TOKEN_END;
        token.value[0] = ']';
        token.value[1] = '\0';
    } else if (c == '"') {
        // Token para un string
        token.type = TOKEN_STRING;
        int i = 0;
        token.value[i++] = '"';
        c = fgetc(file);
        while (c != '"') {
            token.value[i++] = c;
            c = fgetc(file);
        }
        token.value[i++] = '"';
        token.value[i] = '\0';
    } else {
        // Token de palabra clave, identificador o número
        int i = 0;
        while (c != EOF && c != ' ' && c != '\t' && c != '\n' && c != '=' && c != '{' && c != '}' && c != '[' && c != ']') {
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

// Función para leer el contenido y almacenarlo en una estructura ABC
ABC parseABC(FILE *file) {
    ABC abc;
    Token token;

    while (1) {
        token = getNextToken(file);
        if (token.type == TOKEN_END) {
            break;
        }

        if (token.type == TOKEN_KEYWORD) {
            if (strcmp(token.value, "credentials") == 0) {
                // Leer credenciales
                while (1) {
                    token = getNextToken(file);
                    if (token.type == TOKEN_RIGHT_BRACKET) {
                        break;
                    } else if (token.type == TOKEN_IDENTIFIER) {
                        if (strcmp(token.value, "user") == 0) {
                            token = getNextToken(file);
                            strcpy(abc.credentials.user, token.value);
                        } else if (strcmp(token.value, "pass") == 0) {
                            token = getNextToken(file);
                            strcpy(abc.credentials.pass, token.value);
                        }
                    }
                }
            } else if (strcmp(token.value, "origin") == 0) {
                // Leer origen
                while (1) {
                    token = getNextToken(file);
                    if (token.type == TOKEN_RIGHT_BRACKET) {
                        break;
                    } else if (token.type == TOKEN_IDENTIFIER) {
                        if (strcmp(token.value, "ref") == 0) {
                            token = getNextToken(file);
                            strcpy(abc.origin.ref, token.value);
                        } else if (strcmp(token.value, "branch") == 0) {
                            token = getNextToken(file);
                            strcpy(abc.origin.branch, token.value);
                        }
                    }
                }
            } else if (strcmp(token.value, "links") == 0) {
                // Leer enlaces
                abc.links.count = 0;
                while (1) {
                    token = getNextToken(file);
                    if (token.type == TOKEN_RIGHT_BRACKET) {
                        break;
                    } else if (token.type == TOKEN_STRING) {
                        strcpy(abc.links.links[abc.links.count], token.value);
                        abc.links.count++;
                    }
                }
            }
        }
    }

    return abc;
}

// Función principal para probar el código
int main() {
    FILE *file = fopen("./example/ribbonrc", "r");
    if (file == NULL) {
        printf("Error al abrir el archivo.\n");
        return 1;
    }

    ABC abc = parseABC(file);

    fclose(file);

    // Imprimir el contenido de la estructura ABC
    printf("Credenciales:\n");
    printf("Usuario: %s\n", abc.credentials.user);
    printf("Contraseña: %s\n", abc.credentials.pass);
    printf("\nOrigen:\n");
    printf("Referencia: %s\n", abc.origin.ref);
    printf("Rama: %s\n", abc.origin.branch);
    printf("\nEnlaces:\n");
    for (int i = 0; i < abc.links.count; i++) {
        printf("%s\n", abc.links.links[i]);
    }

    return 0;
}
