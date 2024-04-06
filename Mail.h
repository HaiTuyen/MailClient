#ifndef MAIL_H
#define MAIL_H
#pragma once

#include <iostream>
#include <vector>
#include "File.h"
#include <regex>

using namespace std;


class Mail {
private:
    string uid;
    string sender;
    string recipient;
    vector<string> rcpts_to;
    vector<string> rcpts_cc;
    vector<string> rcpts_bcc;
    string boundary;
    string messageID;
    string mime_version;
    string user_agent;
    string content_language;
    string date;
    string subject;
    string content;
    string status;
    string folder;
    string file_name;
    vector<File>attachments;

public:
    Mail(const string& sender, vector<string> rcpts_to, vector<string> rcpts_cc, vector<string> rcpts_bcc, const string& subject, const string& content, const string& filename,vector<File>attachments);
    string getUid();
    string getSender();
    string getRecipient();
    vector<string> getRecipientsTo();
    vector<string> getRecipientsCc();
    vector<string> getRecipientsBcc();
    string getBoundary();
    string getMessageID();
    string getMimeVersion();
    string getUserAgent();
    string getContentLanguage();
    string getDate();
    string getSubject();
    string getContent();
    string getStatus();
    string getFolder();
    string getFileName();
    vector<File> getAttachments();


    void setUid(const string& uid);
    void setSender(const string& sender);
    void setRecipient(const string& recipient);
    void setRecipientsTo(vector<string> rcpts_to);
    void setRecipientsCc(vector<string> rcpts_cc);
    void setRecipientsBcc(vector<string> rcpts_bcc);
    void setBoundary(const string& boundary);
    void setMessageID(const string& messageID);
    void setMimeVersion(const string& mime_version);
    void setUserAgent(const string& user_agent);
    void setContentLanguage(const string& content_language);
    void setDate(const string& date);
    void setSubject(const string& subject);
    void setContent(const string& content);
    void setStatus(const string& status);
    void setFolder(const string& folder);
    void setFileName(const string& file_name);
    void setAttachments(vector<File> attachments);

    void parseEmail(const string& msg);
};


#endif // MAIL_H