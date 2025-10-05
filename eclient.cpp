#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <memory>
#include <string>

int main(){	
	std::unique_ptr<sockaddr_in> saddr{new sockaddr_in};
	
	auto sock_closer = [](int *sock_ptr){
		if(sock_ptr)
			close(*sock_ptr);
		
		delete sock_ptr;
	};

	std::unique_ptr<int, decltype(sock_closer)> sock(new int, sock_closer);
	
	if(*sock = socket(AF_INET, SOCK_STREAM, 0); *sock < 0){
		perror("socket");
		return -1;
	}

	short port;

	std::cout << "enter server port:\n";
	std::cin >> port;

	saddr->sin_family = AF_INET;
	saddr->sin_port = htons(port);
	saddr->sin_addr.s_addr = htonl(INADDR_LOOPBACK);

	if(connect(*sock, reinterpret_cast<sockaddr*>(saddr.get()), sizeof(*saddr)) < 0){
		perror("connect");
		return -1;
	}

	std::string msg, received_msg;
	while(true){
		std::cin >> msg;
		received_msg.resize(msg.size());

		if(send(*sock, msg.data(), msg.size(), 0) < 0){
			perror("send");
			continue;
		}

		recv(*sock, received_msg.data(), msg.size(), 0);
		std::cout << "server: " << received_msg << "\n";
	}

	return 0;
}
