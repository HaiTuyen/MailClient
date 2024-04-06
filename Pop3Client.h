#ifndef POP3CLIENT_H
#define POP3CLIENT_H

#pragma once
#include <iostream>    
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <vector>
#include <sstream>
#include <algorithm>
#include <unistd.h>
#include <fcntl.h>
#include "Mail.h"
using namespace std;

#define BUFFER_SIZE 1024

class Pop3Client
{
private:
    int sockfd;
    sockaddr_in serv_addr;
    string username;
    string pop3_server;
    int pop3_port;
    vector<string>& local_uids;
    string receive;
    string STORAGE_DIR;
public:
    Pop3Client(const string &pop3_server, const int &pop3_port, const string &usernam, vector<string>& local_uids, string STORAGE_DIR);
    ~Pop3Client();
    int connectToServer();
    int sendMessage(const string& message);
    int readResponse();
    int readRETRResponse();
    int getEmails();
    vector<string> parseEmailList(const string& buffer);
    vector<pair<size_t, string>> filerNewUids_index(vector<string> server_uids);
    // Mail parseReceivedEmail();
    // void cutResponse();
};

#endif