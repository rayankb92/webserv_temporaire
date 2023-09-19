/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpServer.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rferradi <rferradi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/29 01:16:53 by rferradi          #+#    #+#             */
/*   Updated: 2023/09/19 07:28:59 by rferradi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_SERVER_HPP__
# define HTTP_SERVER_HPP__

# include <iostream>
# include <string>
# include <vector>
# include <map>
# include <fstream>
#include "Server.hpp"

# define MAX_EPOLL_EVENT 1024

class HttpServer
{
	private:
		std::map <int, std::string>	_request;
		std::vector<Stream *>		_servers;
		int 						_epollFd;
		std::vector <epoll_event> 	_events;

	public:
		HttpServer();
		~HttpServer();
		
		std :: vector<Stream *>	getStreams() const;

		void		printServers() const;
		void		addServer(Server *serv);
		void		initEpollInstance();
		void		startServer();
		void		closeAllFd () const;
		bool		findSocketServ ( int socketFd ) const;
		static int	_SigInt;
};


std :: ostream	&operator<<(std :: ostream &os, const p_locate &locate);

#endif