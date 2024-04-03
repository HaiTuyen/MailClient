#ifndef __MAILCLIENT_H__
#define __MAILCLIENT_H__

#pragma once
#include <iostream>
#include "SmtpClient.h"
#include "Pop3Client.h"
#include "File.h"
#include "Mail.h"
#include <sstream>
#include <vector>
#include <dirent.h>

using namespace std;

#define DEFAULT_SENDER "mailClient@fitus.hcmus.vn"
#define SMTP_SERVER "192.168.1.180"
#define SMTP_PORT 2225
#define POP3_SERVER "192.168.1.180"
#define POP3_PORT 3335
#define STORAGE_DIR "storage/"


class MailClient
{
private:
    vector<string> local_uids;

public:
    MailClient();
    ~MailClient();
    int sendMail(string to, string cc, string bcc, string subject, string content, vector<string> attachment_paths);
    vector<Mail> getMails(string folder);
    vector<string> splitString(string str, char delimiter);
    void pullNewEmails();
    vector<Mail> retriveLocalEmails(string folder);
};

#endif
