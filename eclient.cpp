#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <memory>

int main(int argc, char** argv){
	if(argc != 2){
		std::cout << "wrong arg count\n";
		return -1;	
	}

	std::unique_ptr<sockaddr_in> saddr{new sockaddr_in};
	int sock;
	
	if(sock = socket(AF_INET, SOCK_STREAM, 0); sock < 0){
		perror("socket");
		return -1;
	}

	saddr->sin_family = AF_INET;
	saddr->sin_port = htons(8080);
	saddr->sin_addr.s_addr = htonl(INADDR_LOOPBACK);

	if(connect(sock, (sockaddr*)&(*saddr), sizeof(*saddr)) < 0){
		perror("connect");
		return -1;
	}

	char buf[sizeof(argv[1])];

	send(sock, argv[1], sizeof(argv[1]), 0);
	recv(sock, buf, sizeof(argv[1]), 0);

	std::cout << buf << "\n";

	close(sock);

	return 0;
}
