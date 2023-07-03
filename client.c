#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main()
{
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};

    // Crear el socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Fallo al crear el socket");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convertir la dirección IP de la forma legible al formato binario
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        perror("Dirección inválida o no soportada");
        exit(EXIT_FAILURE);
    }

    // Conectar al servidor
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("Fallo al conectar al servidor");
        exit(EXIT_FAILURE);
    }

    // Proceso de chat
    while (1)
    {
        printf("Cliente: ");
        fgets(buffer, BUFFER_SIZE, stdin);

        // Enviar mensaje al servidor
        if (send(sock, buffer, strlen(buffer), 0) < 0)
        {
            perror("Fallo al enviar el mensaje al servidor");
            exit(EXIT_FAILURE);
        }

        // Leer respuesta del servidor
        memset(buffer, 0, sizeof(buffer));
        if (read(sock, buffer, sizeof(buffer)) <= 0)
        {
            perror("Fallo al leer la respuesta del servidor");
            exit(EXIT_FAILURE);
        }

        printf("Servidor: %s", buffer);
    }

    // Cerrar socket
    close(sock);

    return 0;
}
