/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Post.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rferradi <rferradi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/27 21:13:52 by rferradi          #+#    #+#             */
/*   Updated: 2023/09/19 13:46:04 by rferradi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Post.hpp"
#include "Cgi.hpp"
#define NPOS std::string::npos


void printrayan(std::string str, size_t beg)
{
	std::cout << "beg = " << beg << std::endl;
	for (size_t i = beg; str[i] && i < beg + 60; i++)
	{
		// std::cout << str[i] << "|" << (int)str[i] << "|->["<< i << "]" << std::endl;
	}
	std::cout << std::endl;
	// std::cout << "end = " <<  << std::endl;
}

Post::Post(std::string &req, map_locate const loc, Cgi *_cgi): Methods(req, loc, _cgi)
{
	// std::cout << "CONSTRUCTOR POST" << std::endl;
	// std::cout << "\n\nPOST -> " << req << "\n FIN POST\n\n";
	size_t pos = req.find("Content-Type: ");
	if (pos == std::string::npos)
		return ;
	size_t pointvirgule = req.find(";", pos);
	size_t eol = req.find("\r\n", pos);
	size_t endType = eol < pointvirgule ? eol : pointvirgule;
		// throw std::runtime_error("POST: missing content type");
	pos += 14;
	_postPath = _locate[_location]->getFilePathPost();
	std::cerr << "postpath = " << _postPath << std::endl;
	if (_postPath == "")
		_postPath = _siteRoot;
	_contentType = req.substr(pos, (endType - pos));
	printrayan(req, pointvirgule);
	size_t boundaryDash = req.find("boundary=", pointvirgule);
	// std::cout << "boundaryDash = " << boundaryDash << "eol -pointvir " << eol - pointvirgule << std::endl;
	if (boundaryDash != std::string::npos && (boundaryDash += 9))
		_boundary = req.substr(boundaryDash, (eol - boundaryDash));
	else
		// std::cout << "pas de boundary" << std::endl;
	// std::cout << "\nLe content type = " << _contentType << "\n la boundary = " << _boundary << "|" << std::endl;
	std::string mimtype = mimType(_contentType);
	// std::cout << "\nLe MIMTYPE type = " << mimtype << std::endl;
	size_t poslen = _request.find("Content-Length");
	_head = _request.find("\r\n\r\n", poslen);
	std::string contlen = _request.substr(poslen + 16, _head - (poslen + 16));
	std::stringstream ss(contlen);
	ss >> _contentLen;
	if (_boundary != "")
		_head = req.find(_boundary, eol) + _boundary.size() + 6;
	_body = _head + 4;
	_inbody = false;
	_boundary = "\r\n--" + _boundary;
	// std::cerr << "---> POST = \n" << req << std::endl;
	// printrayan(req, _head);
	// std::cerr << "req size = " << req.size() << std::endl;
}

Post::~Post()
{
	std::cout << "Poste destructor called\n" << std::endl;
	std::cout << "closing file " << _filename << std::endl;
	_fileStream.close();
}

bool Post::handle()
{
	if (_code != OK)
		return 1;
	size_t max = _locate[_location]->getMaxBodySize();
	if (_contentLen > max && max != 0)
	{
		_code = TOLARGE;
		return 1;
	}
	return parseBody();
}

size_t 	Post::findNextBound()
{
	if (_boundary == "")
		return (_request.size());
	_nextBoundary = _request.find(_boundary, _head);
	if (_nextBoundary != NPOS)
		_nextBoundary += _boundary.size() + 2;
	else
		_nextBoundary = _request.size();
	return (_nextBoundary);
}

size_t 	Post::findNextBoundBegin()
{
	_nextBoundary = _request.find(_boundary, _head);
	if (_nextBoundary == NPOS)
		_nextBoundary = _request.size();
	return (_nextBoundary);
}

int Post::multipartData()
{
	static int a = 0;
	a++;
	if (_head == _request.find(_boundary + "--") + _boundary.size() + 2)
		return (1);
	printrayan(_request, _head);
	size_t eol = _request.find("\r\n", _head);
	size_t filename = _request.find("; filename=\"", _head);
	std::cerr << "\n\n\n\nPOST multi filename = " << filename << std::endl;
	if (eol == NPOS)
		return (0);
	// std::cout << "eol - filename = " << eol - filename << std::endl;
	if (filename != NPOS && filename < eol)
	{
		filename += 12;
		// std::cout << "eol - filename = " << eol - filename << std::endl;
		_filename = _postPath + _request.substr(filename, (eol - filename) - 1);
		// std::cout << "Le filename = " << _filename << std::endl;
		if (!_fileStream.is_open())
			_fileStream.open(_filename.c_str(), std::ios::binary);
	}
	else if (_filename == "")
	{
		// std::cout << "pas de filename" << std::endl;
		_head = findNextBound();
		return -1;
	}
	if (!_inbody)
	{
		size_t bodypos = _request.find("\r\n\r\n", _head);
		if (bodypos != NPOS)
		{
			_head = bodypos + 4;
			_inbody = true;
		}
		else
			return (0);
	}
	if (_inbody)
		writeFileMulti();
	
	return false;
}

std::string Post::makeFilename(std::string file)
{
	std::string filename;
	int 		number = 2;

	filename = file + "(1)";
	std::cout << "make file filename = " << filename << " _File = " << file << std::endl;
	while (LibHelp::fileExists(_postPath + filename))
	{
		filename = file + "(" + std::to_string(number) + ")";
		number++;
	}
	return (_postPath + filename);
}

std::string Post::generateRandomFileName()
{
	std::string filename = "file";
	if (mimType(_contentType) != "")
		filename += mimType(_contentType);
	if (LibHelp::fileExists(_postPath + filename))
		return makeFilename(filename);
	return (_postPath + filename);
}

int Post::binaryData()
{
	if (_isFile && !_open && LibHelp::fileExists(_file))
		_filename = makeFilename(_file);
	else if (!_isFile && !_open)
		_filename = generateRandomFileName();
	else
		_filename =  _postPath + _path.substr(1);
	if (!_open)
	{
		_fileStream.open(_filename.c_str(), std::ios::binary);
		_open = _fileStream.is_open();
		if (!_open)
			throw std::runtime_error("POST: can't open file");
	}
	if (writeFile())
		return (1);
	return 0;
}

bool	Post::writeFile()
{
	_nextBoundary = findNextBound();
	std::string toWrite = _request.substr(_head, _nextBoundary);
	_fileStream.write(toWrite.c_str(), toWrite.size());
	if ( (_fileStream.rdstate() & std::ifstream::failbit ) != 0 )
		std::cerr << "Error opening 'test.txt'\n";
	if (_request.size() - _body == _contentLen)
	{
		_fileStream.close();
		_filename.clear();
		_head = _nextBoundary;
		_inbody = false;
		return (true);
	}
	_head = _nextBoundary;
	return (false);
}

bool Post::parseBody()
{
	// int i = 0;
	// int boundOpen = 0;
	// if (_contentLen == 0)
	// 	return (true);
	if (_contentType == "multipart/form-data")
	{
		if (_boundary != "" && _request.find(_boundary + "--\r\n") == NPOS)
			return (false);
		std::cerr << "multipart " << _request << std::endl;
		while (1)
			if (multipartData() > 0)
				return (true);
	}
	else if(_isCgi)
	{
		// _isCgi = false;
		// if (_boundary != "" && _request.find(_boundary + "--\r\n") == NPOS)
		// 	return (false);
		// else if (_request.size() - _body != _contentLen)
		// 	return (false);		
		// std::cout << "cgi" << std::endl;
		return true;
	}
	else
	{
		std::cout << "ELSE --> " << _request << std::endl;
		if (_request.size() - _body != _contentLen)
			return (false);
		_open = false;
		_head = _body;
		_boundary = "";
		while (1)
			if (binaryData() > 0)
				return (true);
	}
	return (false);
}

bool Post::writeFileMulti()
{
	if (!_fileStream.is_open())
		return (false);
	findNextBoundBegin();
	std::string toWrite = _request.substr(_head, _nextBoundary - _head);
	_fileStream.write(toWrite.c_str(), toWrite.size());
	if ( (_fileStream.rdstate() & std::ifstream::failbit ) != 0 )
		std::cerr << "Error opening 'test.txt'\n";
	if (_nextBoundary != _request.size())
	{
		_fileStream.close();
		_filename.clear();
		_head = findNextBound();
		_inbody = false;
	}
	return (false);
}

std::string Post::getBody()
{
	return (_request.substr(_body, _contentLen));
}

std::string	Post::HttPage(int epfd)
{
	std::cout << "HTTPAGE --> POST" << std::endl;
	std::cerr << "CODE = " << _code << std::endl;
	std::string html;
	if (_isCgi && _code == OK)
	{
		struct epoll_event ev;

		std::cerr << "ISCGI = " << _isCgi << std::endl;
		_cgi->setupCgi(this, this->getBody());
		_cgi->createPage();
		ev.events = EPOLLIN;
		ev.data.ptr = _cgi;
		epoll_ctl(epfd, EPOLL_CTL_ADD, _cgi->getOutPipe(), &ev);
		_request.clear();
	}
	if (_code == OK)
		return ("HTTP/1.1 201 Created\r\nContent-Length: 0\r\n\r\n");
	else if (_code == REDIRECT)
	{
		html = "HTTP/1.1 302 Moved Permanently\r\n" \
							"Location: " + _redirect + \
							"\r\n\r\n";
		return html;
	}
	else if (_code == TOLARGE)
		return Page::errorPage(413, "Content Too Large");
	else if (_code == METHODNOTALLOW)
		return Page::errorPage(405, "Method Not Allowed");
	else if (_code == UNAUTHORIZED)
		return Page::errorPage(401, "Unauthorized");
	else if (_code == FORBIDDEN)
		return Page::errorPage(403, "Forbidden");
	return Page::errorPage(BADREQUEST, "Bad Request");
}