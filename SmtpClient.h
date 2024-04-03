#ifndef SMTPCLIENT_H
#define SMTPCLIENT_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <cstring>
#include <vector>
#include <random>
#include "File.h"
#include "Mail.h"
#include <map>
using namespace std;

class SmtpClient
{
private:
    int sockfd;
    sockaddr_in serv_addr;
    char buffer[1024] = {0};
    string smtp_server;
    int smtp_port;
    map<std::string, std::string> mimeTypes;

public:
    SmtpClient(const string& smtp_server,const int& smtp_port);
    ~SmtpClient();
    int connectToServer();
    int sendMessage(const string& message);
    int readResponse();
    string joinRecipients(vector<string>recipients);
    string generateBoundary();
    void setUpMimeTypes();
    int sendMail(Mail mail);
    void closeConnection();
};

#endif 