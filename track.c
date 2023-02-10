#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

using namespace std;

int main()
{
    int listen_socket, connect_socket;
    socklen_t client_len;
    struct sockaddr_in server_address, client_address;
    char time_buffer[26];
    time_t current_time;
    struct tm * time_info;

    listen_socket = socket(AF_INET, SOCK_STREAM, 0);
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(21);

    bind(listen_socket, (struct sockaddr *) &server_address, sizeof(server_address));
    listen(listen_socket, 5);

    while (1) {
        client_len = sizeof(client_address);
        connect_socket = accept(listen_socket, (struct sockaddr *) &client_address, &client_len);

        current_time = time(NULL);
        time_info = localtime(&current_time);
        strftime(time_buffer, 26, "%Y-%m-%d %H:%M:%S", time_info);
        cout << "Client connected from " << inet_ntoa(client_address.sin_addr) << " at " << time_buffer << endl;
        close(connect_socket);
    }

    return 0;
}
