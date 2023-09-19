/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rferradi <rferradi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/21 06:11:07 by jewancti          #+#    #+#             */
/*   Updated: 2023/09/18 03:45:10 by rferradi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Server.hpp"
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "Client.hpp"

map_locate	Server :: getLocations() const
{
	return _locate;
}

Server :: Server(map_locate &locate)
{
	_locate = locate;
	int option = 1;
	map_locate_it	it = _locate.begin();
	_port = it -> second -> getPort();

	t_sockaddr serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(_port);
	serverAddr.sin_addr.s_addr = inet_addr(it -> second -> getListen().c_str());
	_socketFd = socket(AF_INET,  SOCK_STREAM, IPPROTO_TCP);
	if (_socketFd == -1)
		throw std::runtime_error("Socket failed");
	if (setsockopt(_socketFd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &option, sizeof(option)) < 0)
		throw std::runtime_error("Setsockopt failed");
	if (bind(_socketFd, (sockaddr *)& serverAddr, sizeof(serverAddr)) == -1) {
		close(_socketFd);
		throw std::runtime_error("Bind failed");
	}
	if (listen(_socketFd, 1) == -1) {
		close(_socketFd);
		throw std::runtime_error("Listen failed");
	}
	if (! LibHelp :: setNonBlocking(_socketFd))
		throw std::runtime_error("Failed to set fd in non blocking mode");
}

Server :: ~Server()
{

}

int		Server::handleRequest(int epfd, epoll_event& ev)
{
	while (1)
	{
		int connexionId = accept(_socketFd, 0, 0);
		if (connexionId == -1)
			return -1;
		std::cout << "accept " << std::endl;
		if (! LibHelp :: setNonBlocking(connexionId))
			return -1 ;
		ev.events = EPOLLIN;
		ev.data.ptr = new Client(connexionId, this, epfd);
		if (epoll_ctl(epfd, EPOLL_CTL_ADD, connexionId, & ev) < 0)
			return -1 ;
	}
}

/**
 * Private methods
*/

Location	*Server :: _parse(const std :: string buffer)
{
	std :: string line(buffer.begin(), LibHelp :: search(buffer.begin(), buffer.end(), '\n'));
	if (line.empty())
		return NULL;
	if (! LibHelp :: find(line.begin(), line.end(), "HTTP/1.1")) {
		return NULL;
	}
	vstring	words = LibHelp :: toVString(line, ' ');
	if (words.size() < 3 || _locate.find(words.at(1)) == _locate.end()) // la route [1]
		return NULL;
	Location *loc = _locate[words.at(1)];
	// la methode [0]
	if (loc -> getAllowedMethods().empty())
		return _locate[words.at(1)];
	vstring allowedMethods = loc -> getAllowedMethods();
	for (vstring_it it = allowedMethods.begin(); it != allowedMethods.end(); it++) {
		if (*it == words.at(0))
			return _locate[words.at(1)];
	}
	return NULL;
}


