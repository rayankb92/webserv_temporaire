/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Stream.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rferradi <rferradi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/30 23:26:44 by rferradi          #+#    #+#             */
/*   Updated: 2023/09/19 07:27:03 by rferradi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STREAM_HPP
#define STREAM_HPP

# include "FileReader.hpp"
# include "LibHelp.hpp"
# include "Location.hpp"
#include <sys/epoll.h>

# define BUF_SIZE 42000

class Stream
{
	protected:
		int			_socketFd;

	public:
		Stream();
		Stream(int fd);
		virtual ~Stream(){};

		int				getSocketFd();
		void			setSocketFd(int fd);
		virtual int		handleRequest(int epfd, epoll_event& ev) = 0;
};
#endif