#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <fstream>
#include <map>

using namespace std;

int main()
{
    int listen_socket, connect_socket;
    socklen_t client_len;
    struct sockaddr_in server_address, client_address;
    char time_buffer[26];
    time_t start_time, end_time;
    struct tm * time_info;
    ofstream logfile;
    map<string, int> connections;

    listen_socket = socket(AF_INET, SOCK_STREAM, 0);
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(22);

    bind(listen_socket, (struct sockaddr *) &server_address, sizeof(server_address));
    listen(listen_socket, 5);

    logfile.open("connect.csv", ios::app);

    while (1) {
        client_len = sizeof(client_address);
        connect_socket = accept(listen_socket, (struct sockaddr *) &client_address, &client_len);

        start_time = time(NULL);
        sleep(2);
        end_time = time(NULL);

        time_info = localtime(&start_time);
        strftime(time_buffer, 26, "%Y-%m-%d %H:%M:%S", time_info);
        cout << "Client connected from " << inet_ntoa(client_address.sin_addr) << " at " << time_buffer;
        cout << " for " << end_time - start_time << " seconds" << endl;

        string ip_address = inet_ntoa(client_address.sin_addr);
        connections[ip_address]++;
        logfile << ip_address << "," << time_buffer << "," << end_time - start_time << "," << connections[ip_address] << endl;

        close(connect_socket);
    }

    logfile.close();
    return 0;
}
