#include <stdio.h>
#include <winsock2.h>
#include <windows.h>
#include <stdlib.h>

#define BUFFER_SIZE 2000

void iniciar_servidor(int puerto, SOCKET *serverSocket, struct sockaddr_in *server, char *html, int size) {

    char buffer[BUFFER_SIZE];

    server->sin_family = AF_INET;
    server->sin_port = htons(puerto);
    server->sin_addr.s_addr = inet_addr("127.0.0.1");

    if (bind(*serverSocket, (struct sockaddr *)server, sizeof(*server)) == SOCKET_ERROR) {
        printf("error bind: %d\n", WSAGetLastError());
        closesocket(*serverSocket);
        WSACleanup();
        exit(1);
    }

    if (listen(*serverSocket, 1) == SOCKET_ERROR) {
        printf("error listen: %d\n", WSAGetLastError());
        closesocket(*serverSocket);
        WSACleanup();
        exit(1);
    }

    printf("Servidor escuchando en el puerto %d\n", puerto);
    int addlen = sizeof(*server);

    while (1) {
        SOCKET clientSocket = accept(*serverSocket, (struct sockaddr*)server, &addlen);

        int bytes_read = recv(clientSocket, buffer, BUFFER_SIZE - 1, 0);
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';
            printf("Mensaje recibido: %s\n", buffer);
        } else {
            printf("error leyendo mensaje: %d\n", WSAGetLastError());
        }

        printf("Cliente conectado\n");

        char response[BUFFER_SIZE];
        sprintf(response, "HTTP/1.1 200 OK\r\n"
                        "Content-Type: text/html; charset=UTF-8\r\n"
                        "Connection: close\r\n"
                        "Content-Length: %d\r\n"
                        "\r\n"
                        "%s", size, html);

        send(clientSocket, response, strlen(response), 0);
        closesocket(clientSocket);
        Sleep(1000);
    }

}

void inicializar_winsock(WSADATA *wsaData) {
    int error = WSAStartup(MAKEWORD(2, 2), wsaData);
    if (error != 0) {
        printf("error inicializando Winsock: %d\n", WSAGetLastError());
        exit(1);
    }
}

void inicializar_socket(SOCKET *serverSocket) {
    *serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (*serverSocket == INVALID_SOCKET) {
        printf("error creando socket: %d\n", WSAGetLastError());
        WSACleanup();
        exit(1);
    }
}

void cargar_html(int *size, char **html) {
    FILE *file = fopen("index.html", "r");
    if (file == NULL) {
        printf("Error: No se pudo abrir el archivo index.html\n");
        printf("Asegurate de que el archivo index.html existe en el directorio actual\n");
        exit(1);
    }


    *size = 0;
    while (fgetc(file) != EOF) {
        *size += 1;
        printf("size: %d\nftell: %ld\n", *size, ftell(file));
    }
    fseek(file, 0, SEEK_SET);
    *html = (char*)malloc(*size + 1);
    fread(*html, 1, *size, file);
    (*html)[*size] = '\0';
    fclose(file);
}

int main() {

    WSADATA wsaData;
    SOCKET serverSocket;
    struct sockaddr_in server;
    int puerto = 5555;
    int size;
    char *html;
    
    inicializar_winsock(&wsaData);

    inicializar_socket(&serverSocket);

    cargar_html(&size, &html);  

    iniciar_servidor(puerto, &serverSocket, &server, html, size);


    closesocket(serverSocket);
    WSACleanup();
    return 0;
}