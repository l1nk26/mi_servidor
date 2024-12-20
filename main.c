#include <stdio.h>
#include <winsock2.h>
#include <windows.h>
#include <stdlib.h>
#include "server.h"


int main() {

    WSADATA wsaData;
    int puerto = 8000;
    char *html = cargar_html();
    Server server;
    
    inicializar_winsock(&wsaData);

    inicializar_servidor(&server, puerto);
    ejecutar_servidor(&server, html);
    free(html);
    limpiar_servidor(&server);

    WSACleanup();
    return 0;
}