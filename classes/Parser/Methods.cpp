/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Methods.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rferradi <rferradi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/06 20:17:40 by rferradi          #+#    #+#             */
/*   Updated: 2023/09/11 18:48:43 by rferradi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Methods.hpp"

const std::map<std::string, std::string> Methods::_mimType = LibHelp::setMimType();

Methods::Methods(std::string &req, map_locate const loc, Cgi *cgi): _request(req), _locate(loc)
{
	std::stringstream ss(_request);
	std::string line, word;
	std::getline(ss, line);
	std::cerr << "\n\nRequete line ---> " << line << std::endl;
	vstring vec = LibHelp::toVString(line, ' ');
	_path = vec[1];
	std::cerr << "Path = " << _path << std::endl;
	size_t queryStr = _path.find('?');
	if (queryStr != std::string::npos)
	{
		_queryString = _path.substr(queryStr + 1);
		_path = _path.substr(0, queryStr);
	}
	_cgi = cgi;
	_reqType = vec[0];
	_hasLocation = false;
	_isFile = true;
	size_t slash = _path.find('/', 1);
	_file = "";
	if (slash != std::string::npos && _path[slash + 1] != '/' && _path.size() - 1 > slash )
	{
		_file = _path.substr(slash + 1);
		_location = _path.substr(0, slash);
		std::cout << "RENTRER file = " << _file << \
		" _location = " << _location << std::endl;
	}
	else
		_location = _path;
	if (_locate.find(_location) == _locate.end())
	{
		std::cout << "pas de loc " << _location << std::endl;
		_file = _path.substr(1);
	}
	size_t point = _file.find('.');
	if (point != std::string::npos)
		_textType = _file.substr(point + 1);
	if (_locate.find(_location) == _locate.end())
		_location = "/";
	else
		_hasLocation = true;
	_errorPage =  _locate[_location]->getErrorPage();
	_methods = getMethods();
	_redirect = _locate[_location]->getRedirect();
	std::cout << "FILE VAUT |" << _file << "|" << std::endl;
	if (_file == "")
	{
		_isFile = false;
		_file = _locate[_location]->getIndex();
		std::cout << "defini file a " << _file  << std::endl;
	}
	size_t ext = _file.find('.');
	if (ext != std::string::npos)
		_fileType = _file.substr(ext + 1);
	if (_fileType == "php" || _fileType == "py")
		_isCgi = true;
	else
		_isCgi = false;
	_siteRoot = _locate[_location]->getRoot();
	_textType = _fileType != "" ? _fileType : "plain";
	_file = _siteRoot + _file;
	_code = checkRequest();
}

Methods::~Methods(){}

int	Methods::checkRequest()
{
	if (_file.find("..") != std::string::npos)
		return UNAUTHORIZED;
	std::cout << "FILE = " << _file << std::endl;
	if (_reqType != "POST" && LibHelp::isDirectory(_file))
		return DIRECTORY;
	if (_methods.size() != 0 && std::find(_methods.begin(), _methods.end(), _reqType) == _methods.end())
		return METHODNOTALLOW;
	if (_redirect != "")
		return REDIRECT;
	if ((_isCgi || _reqType == "GET") && access(_file.c_str(), F_OK) != 0)
	{
		std::cerr << "checkRequest NOTFOUND" << std::endl;
		std::cerr << "_isCgi = " << _isCgi << " _reqType = " << _reqType << std::endl;
		return NOTFOUND;
	}
	if ((_isCgi || _reqType == "GET") && access(_file.c_str(), R_OK) != 0)
		return FORBIDDEN;
	return OK;
}

std::string	Methods::CreatePage(int code, std::string page, std::string type)
{
	std::string response = "HTTP/1.1 " + LibHelp::toString(code) + \
							" " + type + "\r\nContent-Length: " + \
							LibHelp::toString(page.size()) + \
							"\r\nContent-Type: " + "text/" + _textType + \
							"; charset=utf-8\r\n\r\n" + page;
	return response;
}

std::string Methods::mimType(std::string content)
{
	if (Methods::_mimType.find(content) != Methods::_mimType.end())
		return (Methods::_mimType.find(content)->second);
	return ("");
}

cvstring Methods::getMethods()
{
	std::vector<std::string> res = _locate[_location]->getAllowedMethods();
	if (_location != "/" && res.size() == 0)
		res = _locate["/"]->getAllowedMethods();
	return res;		
}

std::string	Methods::getFileType ( void )
{
	return (_fileType);
}

std::string	Methods::getFile ( void )
{
	return (_file);
}

std::string	Methods::getReqType ( void )
{
	return (_reqType);
}

std::string Methods::getPath ( void )
{
	return (_path);
}

std::string Methods::getLocation ( void )
{
	return (_location);
}

map_locate Methods::getLocate ( void )
{
	return (_locate);
}

std::string Methods::getQueryString ( void )
{
	return (_queryString);
}

bool	Methods::isCgi ( void )
{
	return (_isCgi);
}

void 	Methods::setIsCgi(const bool& isCgi)
{
	_isCgi = isCgi;
}