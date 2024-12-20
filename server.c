#include <WinSock2.h>
#include <string.h>
#include <stdio.h>
#include "server.h"

#define MAX_TAM_HTML 10000

void inicializar_winsock(WSADATA *wsaData) {
    int error = WSAStartup(MAKEWORD(2, 2), wsaData);
    if (error != 0) {
        printf("error inicializando Winsock: %d\n", WSAGetLastError());
        exit(1);
    }
}

void crear_respuesta(char *response, char *body) {
    int longitud = strlen(body);

    char headers[200];
    sprintf(headers, "HTTP/1.1 200 OK\r\n"
                    "Content-Type: text/html; charset=UTF-8\r\n"
                    "Connection: close\r\n"
                    "Content-Length: %d\r\n"
                    "\r\n", longitud);

    sprintf(response, "%s%s", headers, body);
}

int inicializar_servidor(Server* s, int puerto) {
    s-> socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    s-> puerto = puerto;
    s-> addr.sin_family = AF_INET;
    s-> addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    s-> addr.sin_port = htons(puerto);
    s-> estado = 1;

    int error = bind(s-> socket, (struct sockaddr*)&(s-> addr), sizeof(s-> addr)); // vinculando el socket con el puerto

    if (error == SOCKET_ERROR) {
        printf("error vinculando puerto\n");
        return 0;
    }
    return 1;
}
int ejecutar_servidor(Server* s, char* html) {
    char response[MAX_TAM_HTML];
    
    crear_respuesta(response, html);

    int error = listen(s-> socket, 1);
    if (error == SOCKET_ERROR) {
        printf("error en escucha\n");
        return 0;
    }
    int server_size = sizeof(*s);
    printf("Servidor escuchando en puerto %d\n", s-> puerto);

    while (s-> estado) {
        SOCKET clientSocket = accept(s-> socket, (struct sockaddr*)&(s-> addr), &server_size);
        send(clientSocket, response, strlen(response), 0);
        printf("Cliente conectado\n\n");

        int bytes_read = recv(clientSocket, s-> buffer, BUFFER_SIZE - 1, 0);

        if (bytes_read > 0) {
            s-> buffer[bytes_read] = '\0';
            printf("Mensaje recibido: %s\n", s-> buffer);
        } else {
            printf("error leyendo mensaje: %d\n", WSAGetLastError());
        }
        closesocket(clientSocket);
    }
    return 1;
}

void limpiar_servidor(Server* s) {
    closesocket(s-> socket);
}
char* cargar_html() {
    FILE *file = fopen("index.html", "r");
    char *html;
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
    html = (char*)malloc(size + 1);
    fread(html, 1, size, file);
    html[size] = '\0';
    fclose(file);
    return html;
}
