/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alamjada <alamjada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 11:32:01 by alamjada          #+#    #+#             */
/*   Updated: 2026/05/15 11:48:42 by alamjada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <map>
#include <string>
#include <vector>
#include <map>
#include <sys/poll.h>

class Client;
class Channel;

class Server {
	private:
		int _fd;
		int _port;
		std::string _password;
		std::string _name;
		std::vector<struct pollfd> _fds;
		std::map<int, Client *> _clients;
		std::map<std::string, Channel *> _channels;
		std::vector<int> _fdToRemove;



	public:
		Server(int port, const std::string &pass);
		~Server(void);

		// run socket server, bind, listen 
		void run(void);
		void acceptNewClient(void);
		void handleReadClient(int fd);
		void handleWriteClient(int fd);

};

