/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alamjada <alamjada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/14 18:43:55 by alamjada          #+#    #+#             */
/*   Updated: 2026/05/15 11:45:33 by alamjada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"
#include <arpa/inet.h>
#include <cerrno>
#include <cstddef>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <netinet/in.h>
#include <poll.h>
#include <sstream>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>

int main(int ac, char **av) {
  if (ac != 3) {
    std::cerr << "Error: params: ./irc <port> <password>" << std::endl;
    return 1;
  }
  std::vector<struct pollfd> _fds;
  // create socket server  AF_INET ET PF_INET same
  int serverFd = socket(PF_INET, SOCK_STREAM, 0);
  if (serverFd == -1) {
    std::cerr << "Error: socket: " << strerror(errno) << std::endl;
    return 1;
  }
  // handle address already in use in speed start, change bind() comportement
  int opt = 1;
  setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

  // mode non bloquand
  fcntl(serverFd, F_SETFL, O_NONBLOCK);

  // bind
  // defining server address
  struct sockaddr_in addr;
  memset(&addr, 0, sizeof(struct sockaddr_in));
  addr.sin_family = PF_INET;
  std::string portString = av[1];
  int port;
  std::istringstream(portString) >> port;
  addr.sin_port = htons(static_cast<uint16_t>(port));
  addr.sin_addr.s_addr = INADDR_ANY;

  // affect name to socket, bind socket to address
  if (bind(serverFd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
    std::cerr << "Error: bind: " << strerror(errno) << std::endl;
    return 1;
  }

  // listen for Incoming Connections
  //  SOMAXCONN  socket max connection 128 , len of queue
  if (listen(serverFd, SOMAXCONN) == -1) {
    std::cerr << "Error: listen: " << strerror(errno) << std::endl;
    return 1;
  }

  // poll
  struct pollfd poll_fd;
  poll_fd.fd = serverFd;
  poll_fd.events = POLLIN;
  poll_fd.revents = 0;
  _fds.push_back(poll_fd);

  while (true) {
    // watch all fds
    int ret = poll(_fds.data(), _fds.size(), -1);
    if (ret == -1) {
      std::cerr << "Error: listen: " << strerror(errno) << std::endl;
      return 1;
    }
    for (size_t i = 0; i < _fds.size(); i++) {
      // nothing to read in this fd
      if (!(_fds[i].revents & POLLIN))
        continue;

      // new client
      if (_fds[i].fd == serverFd) {
        int clientFd = accept(serverFd, NULL, NULL);
        if (clientFd == -1) {
          std::cerr << "Error: accept client: " << strerror(errno) << std::endl;
          return 1;
        }
        struct pollfd clientPfd;
        clientPfd.fd = clientFd;
        clientPfd.events = POLLIN;
        clientPfd.revents = 0;
        _fds.push_back(clientPfd);
        std::cout << "New client: connected fd: " << clientFd << std::endl;
      } else {
				// message from client
        char buffer[1024] = {0};
        int bytes = recv(_fds[i].fd, buffer, sizeof(buffer) - 1, 0);
        // deconnexion or error
        if (bytes <= 0) {
          std::cout << "Client fd: " << _fds[i].fd << " disconnect"
                    << std::endl;
          close(_fds[i].fd);
          _fds.erase(_fds.begin() + i);
          i--;
        } else {
          std::cout << "Message: " << buffer << std::endl;
        }
      }
    }
  }

  close(serverFd);

  return 0;
}
