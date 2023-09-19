/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpServer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rferradi <rferradi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/29 01:30:32 by rferradi          #+#    #+#             */
/*   Updated: 2023/09/19 08:41:04 by rferradi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpServer.hpp"

int HttpServer::_SigInt = 0;

HttpServer::HttpServer()
	: _events(MAX_EPOLL_EVENT)
{
}

HttpServer::~HttpServer()
{
	int i = -1;
	while (_events[++i].data.ptr)
		delete static_cast<Stream *>(_events[i].data.ptr);
	// closeAllFd();
}

void	HttpServer :: printServers() const
{
	for (std :: vector<Stream *> :: const_iterator it = _servers.cbegin(); it != _servers.cend(); it++)
	{
		map_locate currentLocations = static_cast<Server *>(*it) -> getLocations();
		for (map_locate_it m_it = currentLocations.begin(); m_it != currentLocations.end(); m_it++)
			std :: cout << *m_it;
	}
}

void HttpServer::initEpollInstance()
{
	if ((_epollFd = epoll_create1(0)) < 0)
	{
		closeAllFd();
		throw std::runtime_error("Epoll creation failed");
	}
	for (size_t i = 0; i < _servers.size(); i++)
	{
		std::cout << "Socket fd = " << _servers[i]->getSocketFd() << std::endl;
		_events[i].events = EPOLLIN;
		_events[i].data.ptr = _servers[i];
		if (epoll_ctl(_epollFd, EPOLL_CTL_ADD, _servers[i]->getSocketFd(),
				&_events[i]) < 0)
			throw std::runtime_error("Epoll_ctl failed");
	}
}

void HttpServer::closeAllFd() const
{
	_epollFd ? close(_epollFd) : false;
	for (size_t i = 0; i < _servers.size(); i++)
		close(_servers[i]->getSocketFd());
}

void HttpServer::addServer(Server *serv)
{
	_servers.push_back(serv);
}

void HttpServer::startServer(void)
{
	initEpollInstance();
	int numEvents;
	while (!HttpServer::_SigInt)
	{
		if ((numEvents = epoll_wait(_epollFd, _events.data(), _servers.size(),
					1000)) < 0)
			std::cout << "Epoll_wait failed" << std::endl;
			// throw std::runtime_error("Epoll_wait failed");
		for (int i = 0; i < numEvents; i++)
		{
			Stream *stream = static_cast<Stream *>(_events[i].data.ptr);
			stream->handleRequest(_epollFd, _events[i]);
		}
		// std::cout << "Start serv SigInt =  " << HttpServer::_SigInt << std::endl;
	}
}

std :: vector<Stream *> HttpServer :: getStreams() const
{
	return _servers;
}