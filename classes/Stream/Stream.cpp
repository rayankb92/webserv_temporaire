/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Stream.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rferradi <rferradi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/31 00:55:25 by rferradi          #+#    #+#             */
/*   Updated: 2023/08/01 01:33:29 by rferradi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Stream.hpp"

Stream::Stream():_socketFd(-1){}
Stream::Stream(int fd):_socketFd(fd){}
int		Stream::getSocketFd(){ return _socketFd; }
void	Stream::setSocketFd(int fd){ _socketFd = fd; }