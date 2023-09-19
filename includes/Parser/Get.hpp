/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Get.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rferradi <rferradi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 18:55:02 by rferradi          #+#    #+#             */
/*   Updated: 2023/09/19 07:31:10 by rferradi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef GET_HPP
# define GET_HPP

#include <algorithm>
#include "Methods.hpp"
#include "Cgi.hpp"

class Get : public Methods
{
	protected:

	public:
		Get(std::string &req, map_locate const loc, Cgi *_cgi);
		~Get();

		bool		handle(){return true;};
		std::string	HttPage(int epfd);
};
#endif