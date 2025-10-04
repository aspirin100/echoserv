#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <memory>

int main(){
	std::unique_ptr<sockaddr_in> saddr{new sockaddr_in};
	int listener_sock;

	if(listener_sock = socket(AF_INET, SOCK_STREAM, 0); listener_sock < 0){
		perror("socket");
		return -1;
	}

	saddr->sin_family = AF_INET;
	
	short port;
	std::cout << "insert port:\n";
	std::cin >> port;

	saddr->sin_port = htons(port);
	saddr->sin_addr.s_addr = INADDR_ANY; 

	if(bind(listener_sock, (sockaddr*)&(*saddr), sizeof(*saddr)) < 0){
		perror("bind");
		return -1;
	}

	listen(listener_sock, 1);

	while(true){
		int sock = accept(listener_sock, nullptr, nullptr);
		char buff[1024];
		while(true){
			int readed = recv(sock, buff, 1024, 0);

			if (readed <= 0) break;
			send(sock, buff, readed, 0);
		}
		close(sock);
	}

	return 0;
}
