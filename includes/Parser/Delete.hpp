/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Delete.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rferradi <rferradi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 19:09:01 by rferradi          #+#    #+#             */
/*   Updated: 2023/09/19 07:25:00 by rferradi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DELETE_HPP
# define DELETE_HPP

#include "Methods.hpp"

class Delete : public Methods
{
	private:
	std::string _fileDel;

	public:
	Delete(std::string &req, map_locate const loc, Cgi *_cgi);
	~Delete();
	std::string		HttPage(int epfd);
	bool			handle();
};
#endif