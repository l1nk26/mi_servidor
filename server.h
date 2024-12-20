#ifndef SERVER
#define SERVER

#define BUFFER_SIZE 100
typedef struct Server {
    SOCKET socket;
    struct sockaddr_in addr;
    char buffer[BUFFER_SIZE];
    int puerto;
    int estado;
} Server;

int inicializar_servidor(Server* s, int puerto);
int ejecutar_servidor(Server* s, char* html);
void limpiar_servidor(Server* s);
void crear_respuesta(char *body);
void inicializar_winsock(WSADATA*);
char* cargar_html();

#endif