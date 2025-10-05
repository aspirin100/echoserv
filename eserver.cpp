#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <memory>

int main(){
	std::unique_ptr<sockaddr_in> saddr{new sockaddr_in};
	auto socket_closer = [](int *sock_ptr){
		if(sock_ptr)
			close(*sock_ptr);
		delete(sock_ptr);
	};
	
	std::unique_ptr<int, decltype(socket_closer)> listener_sock(new int, socket_closer);

	if(*listener_sock = socket(AF_INET, SOCK_STREAM, 0); *listener_sock < 0){
		perror("socket");
		return -1;
	}

	saddr->sin_family = AF_INET;
	
	short port;
	std::cout << "insert port:\n";
	std::cin >> port;

	saddr->sin_port = htons(port);
	saddr->sin_addr.s_addr = INADDR_ANY; 

	if(bind(*listener_sock, reinterpret_cast<sockaddr*>(saddr.get()), sizeof(*saddr)) < 0){
		perror("bind");
		return -1;
	}

	listen(*listener_sock, 1);

	while(true){
		std::unique_ptr<int, decltype(socket_closer)> client_fd(new int, socket_closer);
		if(*client_fd = accept(*listener_sock, nullptr, nullptr); *client_fd < 0){
			perror("accept");
			return -1;
		}
		
		char buff[1024];
		while(true){
			int readed = recv(*client_fd, buff, 1024, 0);

			if (readed <= 0) break;
			send(*client_fd, buff, readed, 0);
		}
	}

	return 0;
}
