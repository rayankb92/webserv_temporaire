/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rferradi <rferradi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/20 00:21:15 by jewancti          #+#    #+#             */
/*   Updated: 2023/09/19 01:33:46 by rferradi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MAIN_HPP__
# define __MAIN_HPP__

# include <iostream>
# include <cstring>
# include <string>
# include <fstream>
# include <sstream>
# include <istream>
# include <algorithm>
# include <vector>
# include <map>

# include <dirent.h>
# include <sys/types.h>
# include <sys/socket.h>
#include <sys/stat.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <unistd.h>
# include <fcntl.h>

typedef struct sockaddr_in  t_sockaddr;

typedef std :: string :: iterator						string_it;
typedef std :: string :: const_iterator					cstring_it;
typedef std :: vector<std :: string>					vstring;
typedef const std :: vector<std :: string>				cvstring;
typedef std :: vector<std :: string> :: iterator		vstring_it;
typedef std :: vector<std :: string> :: const_iterator	cvstring_it;

#endif