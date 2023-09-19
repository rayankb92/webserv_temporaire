/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rferradi <rferradi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/30 23:26:38 by rferradi          #+#    #+#             */
/*   Updated: 2023/09/19 11:24:42 by rferradi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Stream.hpp"
#include "Server.hpp"
#include "Cgi.hpp"

#include "Methods.hpp"

class Methods;
class Cgi;

enum e_type
{
	GET,
	POST,
	DELETE
};

class Client: public Stream
{
	private:
		Server *		_server;
		std::string 	_request;
		std::string 	_requestType;
		std::ofstream 	_outfile;
		std::string		_getFile;
		static char		_buff[BUF_SIZE];
		static char		_error[9];
		int				_reqType;
		int 			_epfd;
		Methods			*_method;
		std::string 	_response;
		Cgi 			*_cgi;

	public:
		Client(int fd, Server *ptr);
		Client(int fd, Server *ptr, int epfd);
		~Client();

		int			handleRequest(int epfd, epoll_event& ev);
		bool		didIreadAll();
		std::string	get();
		std::string	getLocation();
		std::string	answer();
		std::string	createHtmlAnswer(std::string html, int type, int len);
		void		setMethods(Methods *);
};
#endif