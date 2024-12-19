#include <stdio.h>
#include <winsock2.h>
#include <windows.h>
#include <stdlib.h>
#include "server.h"

void inicializar_winsock(WSADATA*);

void cargar_html(char **html) {
    FILE *file = fopen("index.html", "r");
    if (file == NULL) {
        printf("Error: No se pudo abrir el archivo index.html\n");
        printf("Asegurate de que el archivo index.html existe en el directorio actual\n");
        exit(1);
    }
    int size = 0;
    while (fgetc(file) != EOF) {
        size += 1;
    }
    fseek(file, 0, SEEK_SET);
    *html = (char*)malloc(size + 1);
    fread(*html, 1, size, file);
    (*html)[size] = '\0';
    fclose(file);
}

int main() {

    WSADATA wsaData;
    int puerto = 8000;
    char *html;
    Server server;
    
    cargar_html(&html);
    inicializar_winsock(&wsaData);

    inicializar_servidor(&server, puerto);
    ejecutar_servidor(&server, html);
    limpiar_servidor(&server);

    WSACleanup();
    return 0;
}