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
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    // Crear el socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("Fallo al crear el socket");
        exit(EXIT_FAILURE);
    }

    // Configurar opciones del socket
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("Fallo al configurar las opciones del socket");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Vincular el socket a una dirección IP y puerto
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("Fallo al vincular el socket");
        exit(EXIT_FAILURE);
    }

    // Escuchar conexiones entrantes
    if (listen(server_fd, 3) < 0)
    {
        perror("Fallo al escuchar las conexiones entrantes");
        exit(EXIT_FAILURE);
    }

    // Aceptar la conexión entrante
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
    {
        perror("Fallo al aceptar la conexión entrante");
        exit(EXIT_FAILURE);
    }

    // Proceso de chat
    while (1)
    {
        // Leer el mensaje del cliente
        memset(buffer, 0, sizeof(buffer));
        if (read(new_socket, buffer, sizeof(buffer)) <= 0)
        {
            perror("Fallo al leer del cliente");
            exit(EXIT_FAILURE);
        }

        printf("Cliente: %s", buffer);

        // Enviar respuesta al cliente
        printf("Servidor: ");
        fgets(buffer, BUFFER_SIZE, stdin);
        if (write(new_socket, buffer, strlen(buffer)) < 0)
        {
            perror("Fallo al enviar la respuesta al cliente");
            exit(EXIT_FAILURE);
        }
    }

    // Cerrar sockets
    close(new_socket);
    close(server_fd);

    return 0;
}
