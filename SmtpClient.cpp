#include "SmtpClient.h"

SmtpClient::SmtpClient(const string &smtp_server, const int &smtp_port)
{
    this->smtp_server = smtp_server;
    this->smtp_port = smtp_port;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        std::cerr << "Failed to create socket\n";
        exit(1);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(smtp_port); 
    if (inet_pton(AF_INET, smtp_server.c_str(), &serv_addr.sin_addr) <= 0)
    {
        std::cerr << "Failed to set SMTP server address\n";
        exit(1);
    }

    setUpMimeTypes();
}

SmtpClient::~SmtpClient()
{
}

int SmtpClient::connectToServer()
{
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        std::cerr << "Failed to connect to server\n";
        return -1;
    }
    return 0;
}

int SmtpClient::sendMessage(const string &message)
{
    if (send(sockfd, message.c_str(), message.size(), 0) < 0)
    {
        std::cerr << "Failed to send message\n";
        return -1;
    }
    return 0;
}

int SmtpClient::readResponse()
{
    memset(buffer, 0, sizeof(buffer));
    if (read(sockfd, buffer, 1024) < 0)
    {
        std::cerr << "Failed to read response\n";
        return -1;
    }
    std::cout << "[SMTP Server]: " << buffer << '\n';
    return 0;
}

string SmtpClient::joinRecipients(vector<string> recipients)
{
    string result = "";
    for (size_t i = 0; i < recipients.size(); i++)
    {
        result += recipients[i];
        if (i < recipients.size() - 1)
        {
            result += ", ";
        }
    }
    return result;
}

string SmtpClient::generateBoundary()
{
    string characters = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

    random_device random_device;
    mt19937 generator(random_device());
    uniform_int_distribution<> distribution(0, characters.size() - 1);

    string random_string;

    for (size_t i = 0; i < 16; ++i)
    {
        random_string += characters[distribution(generator)];
    }
    return random_string;
}

void SmtpClient::setUpMimeTypes()
{
    this->mimeTypes[".aac"] = "audio/aac";
    this->mimeTypes[".abw"] = "application/x-abiword";
    this->mimeTypes[".apng"] = "image/apng";
    this->mimeTypes[".arc"] = "application/x-freearc";
    this->mimeTypes[".avif"] = "image/avif";
    this->mimeTypes[".avi"] = "video/x-msvideo";
    this->mimeTypes[".azw"] = "application/vnd.amazon.ebook";
    this->mimeTypes[".bin"] = "application/octet-stream";
    this->mimeTypes[".bmp"] = "image/bmp";
    this->mimeTypes[".bz"] = "application/x-bzip";
    this->mimeTypes[".bz2"] = "application/x-bzip2";
    this->mimeTypes[".cda"] = "application/x-cdf";
    this->mimeTypes[".csh"] = "application/x-csh";
    this->mimeTypes[".css"] = "text/css";
    this->mimeTypes[".csv"] = "text/csv";
    this->mimeTypes[".doc"] = "application/msword";
    this->mimeTypes[".docx"] = "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
    this->mimeTypes[".eot"] = "application/vnd.ms-fontobject";
    this->mimeTypes[".epub"] = "application/epub+zip";
    this->mimeTypes[".gz"] = "application/gzip";
    this->mimeTypes[".gif"] = "image/gif";
    this->mimeTypes[".htm"] = "text/html";
    this->mimeTypes[".html"] = "text/html";
    this->mimeTypes[".ico"] = "image/vnd.microsoft.icon";
    this->mimeTypes[".ics"] = "text/calendar";
    this->mimeTypes[".jar"] = "application/java-archive";
    this->mimeTypes[".jpeg"] = "image/jpeg";
    this->mimeTypes[".jpg"] = "image/jpeg";
    this->mimeTypes[".js"] = "text/javascript";
    this->mimeTypes[".json"] = "application/json";
    this->mimeTypes[".jsonld"] = "application/ld+json";
    this->mimeTypes[".mid"] = "audio/midi";
    this->mimeTypes[".midi"] = "audio/midi";
    this->mimeTypes[".mjs"] = "text/javascript";
    this->mimeTypes[".mp3"] = "audio/mpeg";
    this->mimeTypes[".mp4"] = "video/mp4";
    this->mimeTypes[".mpeg"] = "video/mpeg";
    this->mimeTypes[".mpkg"] = "application/vnd.apple.installer+xml";
    this->mimeTypes[".odp"] = "application/vnd.oasis.opendocument.presentation";
    this->mimeTypes[".ods"] = "application/vnd.oasis.opendocument.spreadsheet";
    this->mimeTypes[".odt"] = "application/vnd.oasis.opendocument.text";
    this->mimeTypes[".oga"] = "audio/ogg";
    this->mimeTypes[".ogv"] = "video/ogg";
    this->mimeTypes[".ogx"] = "application/ogg";
    this->mimeTypes[".opus"] = "audio/opus";
    this->mimeTypes[".otf"] = "font/otf";
    this->mimeTypes[".png"] = "image/png";
    this->mimeTypes[".pdf"] = "application/pdf";
    this->mimeTypes[".php"] = "application/x-httpd-php";
    this->mimeTypes[".ppt"] = "application/vnd.ms-powerpoint";
    this->mimeTypes[".pptx"] = "application/vnd.openxmlformats-officedocument.presentationml.presentation";
    this->mimeTypes[".rar"] = "application/vnd.rar";
    this->mimeTypes[".rtf"] = "application/rtf";
    this->mimeTypes[".sh"] = "application/x-sh";
    this->mimeTypes[".svg"] = "image/svg+xml";
    this->mimeTypes[".tar"] = "application/x-tar";
    this->mimeTypes[".tif"] = "image/tiff";
    this->mimeTypes[".tiff"] = "image/tiff";
    this->mimeTypes[".ts"] = "video/mp2t";
    this->mimeTypes[".ttf"] = "font/ttf";
    this->mimeTypes[".txt"] = "text/plain";
    this->mimeTypes[".vsd"] = "application/vnd.visio";
    this->mimeTypes[".wav"] = "audio/wav";
    this->mimeTypes[".weba"] = "audio/webm";
    this->mimeTypes[".webm"] = "video/webm";
    this->mimeTypes[".webp"] = "image/webp";
    this->mimeTypes[".woff"] = "font/woff";
    this->mimeTypes[".woff2"] = "font/woff2";
    this->mimeTypes[".xhtml"] = "application/xhtml+xml";
    this->mimeTypes[".xls"] = "application/vnd.ms-excel";
    this->mimeTypes[".xlsx"] = "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
    this->mimeTypes[".xml"] = "application/xml";
    this->mimeTypes[".xul"] = "application/vnd.mozilla.xul+xml";
    this->mimeTypes[".zip"] = "application/zip";
    this->mimeTypes[".3gp"] = "video/3gpp";
    this->mimeTypes[".3g2"] = "video/3gpp2";
    this->mimeTypes[".7z"] = "application/x-7z-compressed";
}


int SmtpClient::sendMail(Mail mail)
{
    string sender = mail.getSender();
    vector<string> rcpts_to = mail.getRecipientsTo();
    vector<string> rcpts_cc = mail.getRecipientsCc();
    vector<string> rcpts_bcc = mail.getRecipientsBcc();
    string subject = mail.getSubject();
    string content = mail.getContent();
    vector<File> attachments = mail.getAttachments();

    connectToServer();
    string boundary = (attachments.size() > 0) ? generateBoundary() : "";

    // Send HELO command amd wait.
    string hello_command = "HELO " + this->smtp_server + "\r\n";
    if (sendMessage(hello_command) < 0) return -1;
    if (readResponse() < 0) return -1;

    // Send MAIL FROM command and wait.
    string mail_from_command = "MAIL FROM:<" + sender + ">\r\n";
    if (sendMessage(mail_from_command) < 0) return -1;
    if (readResponse() < 0) return -1;

    // Send RCPT TO command for each recipient (TO, CC, BCC) and wait.
    for (string rcpt_to : rcpts_to)
    {
        string rcpt_to_command = "RCPT TO:<" + rcpt_to + ">\r\n";
        if (sendMessage(rcpt_to_command) < 0) return -1;
        if (readResponse() < 0) return -1;
    }

    
    for (string rcpt_cc : rcpts_cc)
    {
        string rcpt_to_command = "RCPT TO:<" + rcpt_cc + ">\r\n";
        if (sendMessage(rcpt_to_command) < 0) return -1;
        if (readResponse() < 0) return -1;
    }

    for (string rcpt_bcc : rcpts_bcc)
    {
        string rcpt_to_command = "RCPT TO:<" + rcpt_bcc + ">\r\n";
        if (sendMessage(rcpt_to_command) < 0) return -1;
        if (readResponse() < 0) return -1;
    }

    // Send DATA command.
    string data_command = "DATA\r\n";
    if (sendMessage(data_command) < 0) return -1;
    if (readResponse() < 0) return -1;

    // Send email content.
    string email_content = "";
    email_content += (attachments.size() > 0) ? "Content-Type: multipart/mixed; boundary=\"-----------" + boundary + "\"\r\n" : "";
    email_content += "MIME-Version: 1.0\r\n";
    email_content += "To: " + joinRecipients(rcpts_to) + "\r\n";
    email_content += (rcpts_cc.size() > 0) ? "Cc: " + joinRecipients(rcpts_cc) + "\r\n" : "";
    email_content += "From: <" + sender + ">\r\n";
    email_content += "Subject: " + subject + "\r\n";

    email_content += (attachments.size() > 0) ? "\r\n-------------" + boundary + "\r\n" : "";
    email_content += "Content-Type: text/plain; charset=UTF-8; format=flowed\r\n";
    email_content += "Content-Transfer-Encoding: 7bit\r\n\r\n";
    email_content += content + "\r\n";

    if (attachments.size() > 0)
    {
        for (File attachment : attachments)
        {
            email_content += "\r\n-------------" + boundary + "\r\n";
            email_content += "Content-Type: " + mimeTypes[attachment.getExt()] + "; name=\"" + attachment.getName() + "\"\r\n";
            email_content += "Content-Disposition: attachment; filename=\"" + attachment.getName() + "\"\r\n";
            email_content += "Content-Transfer-Encoding: base64\r\n";
            email_content += "\r\n";
            email_content += attachment.readAndEncode() + "\r\n";
        }
    }

    email_content += (attachments.size() > 0) ? "\r\n-------------" + boundary + "--\r\n" : "";
    email_content += ".\r\n";
    // cout << "Email content: " << email_content << "\n";

    if (sendMessage(email_content) < 0) return -1;
    if (readResponse() < 0) return -1;

    // Send QUIT command.
    string quit_command = "QUIT\r\n";
    if (sendMessage(quit_command) < 0) return -1;
    if (readResponse() < 0) return -1;

    closeConnection();
    return 0;
}

void SmtpClient::closeConnection()
{
    close(sockfd);
}

