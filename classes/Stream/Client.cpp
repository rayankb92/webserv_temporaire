/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rferradi <rferradi@student.42.fr>          +#+  +:+       +#+        */
/*                  map_locate                              +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/31 01:36:42 by rferradi          #+#    #+#             */
/*   Updated: 2023/08/05 23:47:03 by rferradi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Get.hpp"
#include "Post.hpp"
#include "Delete.hpp"

char Client::_buff[BUF_SIZE] = {0};
char Client::_error[9] = "404.html";

Client::Client(int fd, Server *ptr): Stream(fd), _server(ptr){
	std::cout << "SERV " << _server << std::endl;
	_cgi = new Cgi(fd, this);
	_method = NULL;
}

Client::Client(int fd, Server *ptr, int epfd): Stream(fd), _server(ptr), _epfd(epfd){
	std::cout << "SERV " << _server << _epfd << std::endl;
	_method = NULL;
	_cgi = new Cgi(fd, this);
}

void	Client::setMethods(Methods *met){ _method = met; }

Client::~Client()
{
	if (_method)
		delete _method;
	delete _cgi;
	close(_socketFd);
}

bool	Client::didIreadAll()
{
	std::size_t pos = _request.find("Content-Length: ") + 16;
	std::size_t end = _request.find("\r\n", pos);
	if (end == std::string::npos || pos == std::string::npos)
		return 0;
	std::cout << "pos = " << pos << " end = " << end << std::endl;
	std::stringstream ss(_request.substr(pos, end - pos));
	std::size_t res;
	ss >> res;
	std::cout << "---> subSTR " << _request.substr(pos, end - pos)<<std::endl;
	std::cout << "did i read = " << res << std::endl;
	if (_request.size() < res + end)
		return (0);
	return 1;
}

int		Client::handleRequest(int epfd, epoll_event& ev)
{
	if (ev.events == EPOLLIN)
	{
		ssize_t rec = 1;
		while (rec)
		{
			rec = recv(_socketFd, Client::_buff, BUF_SIZE, 0);
			if (rec == 0 || (rec < BUF_SIZE && rec > 0)){ 
				_request.append(Client::_buff, rec);
				break; 
		}
			if (rec < 0)
			{
				epoll_ctl(epfd, EPOLL_CTL_DEL, _socketFd, &ev);
				delete _method;
				delete this;
			}
			_request.append(Client::_buff, rec);
		}
		if (_request.find("\r\n\r\n") != std::string::npos && _request.find("GET") == 0)
		{
			std::cout << "Requete GET------------------------------------------\n";
			std::cout << " la requete " << _request << std::endl;
			std::cout << "-------------------------------------------------------\n\n";

			// if (_request.find("GET /favicon.ico") != std::string::npos)
			// 	return 0;
			std::cout << "METHOD VALUE " << _method << std::endl;
			if (_method == NULL)
				_method = new Get(_request, (_server->getLocations()), _cgi);
			_response = _method->HttPage(epfd);
			std::cout << "respons size " << _response.size() << std::endl;
			std::cout << "La reponse --> " << _response << std::endl;
			if (_response != "" || _method->isCgi() == true)
				ev.events = EPOLLOUT;
			_reqType = GET;
			if (epoll_ctl(epfd, EPOLL_CTL_MOD, _socketFd, &ev) < 0)
				return 0;
		}
		else if (_request.find("POST") != std::string::npos)
		{
			std::cout << "Requete POST------------------------------------------\n";
			std::cout << "zebi ya heja\n";
			std::cout << " la requete " << _request << std::endl;
			std::cout << "-------------------------------------------------------\n\n";
			if (_method == NULL)
				_method = new Post(_request, (_server->getLocations()), _cgi);
			_reqType = POST;
			if (_method->handle())
			{
				_response = _method->HttPage(epfd);
				if (_response != "" || _method->isCgi() == true)
					ev.events = EPOLLOUT;
				if (epoll_ctl(epfd, EPOLL_CTL_MOD, _socketFd, &ev) < 0)
					return 0;
			}	
		}
		else if (_request.find("DELETE") == 0)
		{
			if (_method == NULL)
				_method = new Delete(_request, (_server->getLocations()), _cgi);
			_method->handle();
			_response = _method->HttPage(epfd);
			ev.events = EPOLLOUT;
			if (epoll_ctl(epfd, EPOLL_CTL_MOD, _socketFd, &ev) < 0)
				return 0;
		}
		// else
		// {
		// 	_response = Page::errorPage(400, "Bad Request");
		// 	ev.events = EPOLLOUT;
		// 	if (epoll_ctl(epfd, EPOLL_CTL_MOD, _socketFd, &ev) < 0)
		// 		return 0;
		// }
	}
	else
	{
		if (_method->isCgi())
		{
			ev.events = EPOLLIN;
			if (epoll_ctl(epfd, EPOLL_CTL_MOD, _socketFd, &ev) < 0)
				return 0;
			return 0;
		}

		delete _method;
		_method = NULL;
		if (send(_socketFd, _response.c_str(), _response.length(), 0) < 0)
		{
			epoll_ctl(epfd, EPOLL_CTL_DEL, _socketFd, &ev);
			return 0;
		}
		std::cout << "j'ai recu " << _request << std::endl;
		std::cout << "\n\nj'ai rep\n\n" << _response << std::endl;
		ev.events = EPOLLIN;
		if (epoll_ctl(epfd, EPOLL_CTL_MOD, _socketFd, &ev) < 0)
			return 0;
		_request.clear();
		// delete this;
	}
	return (1);
}