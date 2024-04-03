#include "Pop3Client.h"

Pop3Client::Pop3Client(const string &pop3_server, const int &pop3_port, const string &username, vector<string> &local_uids)
    : username(username), pop3_server(pop3_server), pop3_port(pop3_port), local_uids(local_uids)
{
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        std::cerr << "Failed to create socket\n";
        exit(1);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(pop3_port);
    if (inet_pton(AF_INET, pop3_server.c_str(), &serv_addr.sin_addr) <= 0)
    {
        std::cerr << "Failed to set POP3 server address\n";
        exit(1);
    }
}

Pop3Client::~Pop3Client()
{
}

int Pop3Client::connectToServer()
{
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        std::cerr << "Failed to connect to server\n";
        return -1;
    }
    return 0;
}

int Pop3Client::sendMessage(const string &message)
{
    if (send(sockfd, message.c_str(), message.size(), 0) < 0)
    {
        std::cerr << "Failed to send message\n";
        return -1;
    }
    return 0;
}

int Pop3Client::readResponse()
{
    // memset(buffer, 0, sizeof(buffer));
    // if (read(sockfd, buffer, BUFFER_SIZE) < 0)
    // {
    //     std::cerr << "Failed to read response\n";
    //     return -1;
    // }
    // std::cout << "[Pop3 Server]: " << buffer << '\n';
    // return 0;

    // memset(buffer, 0, sizeof(buffer));
    // ssize_t bytes_read;
    // std::string response;

    // while ((bytes_read = read(sockfd, buffer, BUFFER_SIZE - 1)) > 0)
    // {
    //     cout << "Bytes read: " << bytes_read << '\n';
    // }

    // if (bytes_read < 0)
    // {
    //     std::cerr << "Failed to read response\n";
    //     return -1;
    // }

    // std::cout << "[Pop3 Server]: " << response << '\n';
    // return 0;
    const unsigned int MAX_BUF_LENGTH = 4096;
    std::vector<char> buffer(MAX_BUF_LENGTH);
    this->receive.clear();
    // cout << "Receive: " << this->receive << "\r\n";
    int bytesReceived = 0;
    do
    {
        bytesReceived = recv(sockfd, &buffer[0], buffer.size(), 0);
        if (bytesReceived == -1)
        {
            // Error handling
            break;
        }
        else if (bytesReceived == 0)
        {
            // The other side has closed the connection
            break;
        }
        else
        {
            this->receive.append(buffer.cbegin(), buffer.cbegin() + bytesReceived);
        }
        // cout << "Bytes received: " << bytesReceived << '\n';
        // cout << "Buffer:" << buffer.data() << '\n';
    } while (bytesReceived == MAX_BUF_LENGTH);

    // const unsigned int MAX_BUF_LENGTH = 4096;
    // std::vector<char> buffer(MAX_BUF_LENGTH);

    // while (true) {
    //     int bytesReceived = recv(sockfd, &buffer[0], buffer.size(), 0);
    //     if (bytesReceived == -1) {
    //         // error handling
    //         break;
    //     } else if (bytesReceived == 0) {
    //         // the server has closed the connection
    //         break;
    //     } else {
    //         this->receive.append(buffer.cbegin(), buffer.cbegin() + bytesReceived);
    //     }
    // }

    // std::cout << "[Pop3 Server]: " << this->receive << '\n';
    return 0;
}

int Pop3Client::readRETRResponse()
{
    const unsigned int MAX_BUF_LENGTH = 4096;
    std::vector<char> buffer(MAX_BUF_LENGTH);
    this->receive.clear();
    int bytesReceived = 0;
    do
    {
        bytesReceived = recv(sockfd, &buffer[0], buffer.size(), 0);
        if (bytesReceived == -1)
        {
            // Error handling
            break;
        }
        else if (bytesReceived == 0)
        {
            // The other side has closed the connection
            break;
        }
        else
        {
            this->receive.append(buffer.cbegin(), buffer.cbegin() + bytesReceived);
            if (this->receive.find("\r\n.\r\n") != string::npos)
            {
                this->receive = this->receive.substr(0, this->receive.find("\r\n.\r\n"));
                break;
            }
        }
        // cout << "Bytes received: " << bytesReceived << '\n';
        // cout << "Buffer:" << buffer.data() << '\n';
    } while (bytesReceived > 0);

    return 0;
}

int Pop3Client::getEmails()
{
    connectToServer();
    readResponse();

    // Send CAPA command
    sendMessage("CAPA\r\n");
    readResponse();

    // Send USER command
    sendMessage("USER " + this->username + "\r\n");
    readResponse();

    // Send STAT command
    sendMessage("STAT\r\n");
    readResponse();

    // Send LIST command
    sendMessage("LIST\r\n");
    readResponse();

    // Send UIDL command
    sendMessage("UIDL\r\n");
    readResponse();
    vector<string> server_uids = parseEmailList(this->receive);
    vector<pair<size_t, string>> new_idx_uids = filerNewUids_index(server_uids);
    // cout << "New UIDs: \r\n";
    // for (string uid : new_idx_uids)
    // {
    //     cout << uid << "\r\n";
    // }
    // cout << "----------------\r\n";
    // cout << "Local UIDs: \r\n";
    // for (string uid : local_uids)
    // {
    //     cout << uid << "\r\n";
    // }
    // cout << "----------------\r\n";
    // cout << "Server UIDs: \r\n";
    // for (string uid : server_uids)
    // {
    //     cout << uid << "\r\n";
    // }
    // cout << "----------------\r\n";

    // Send RETR command
    for (const auto &[index, uid] : new_idx_uids)
    {
        string message;
        sendMessage("RETR " + to_string(index) + "\r\n");
        readRETRResponse();
        message += this->receive;

        ofstream file("storage/inbox/" + uid);
        file << message;
        file.close();
        
        local_uids.push_back(uid);
    }

    // Send QUIT command
    sendMessage("QUIT\r\n");
    readResponse();
    
    return 0;
}

vector<string> Pop3Client::parseEmailList(const string &buffer)
{
    vector<string> result;
    stringstream ss(buffer);
    string line;
    while (getline(ss, line, '\n'))
    {
        size_t index = line.find(" ");
        string uidl;
        if (index != string::npos)
        {
            uidl = line.substr(index + 1);
            result.push_back(uidl);
        }
    }
    return result;
}

vector<pair<size_t, string>> Pop3Client::filerNewUids_index(vector<string> server_uids)
{
    vector<pair<size_t, string>> new_uids;
    for (size_t i = 0; i < server_uids.size(); i++)
    {
        if (find(local_uids.begin(), local_uids.end(), server_uids[i]) == local_uids.end())
        {
            new_uids.push_back(make_pair(i + 1, server_uids[i]));
        }
    }
    return new_uids;
}

Mail Pop3Client::parseReceivedEmail()
{
    string buffer(this->receive);
    // cout << "----------------Email----------------\n";
    // cout << "Buffer: " << buffer << '\n';
    // cout << "---------------- END BUFFER ----------------\n";
    Mail mail("", vector<string>{}, vector<string>{}, vector<string>{}, "", "", vector<File>{});

    string header = "";
    string content = "";
    vector<string> attachments;

    if (buffer.find("multipart/mixed") != string::npos)
    {
        // cout << "Buffer: " << buffer << '\n';
        size_t start_pos = buffer.find("boundary=\"");
        // cout << "Start pos: " << start_pos << '\n';
        size_t end_pos = buffer.find("\"", start_pos + 10);
        // cout << "End pos: " << end_pos << '\n';
        string boundary = "--" + buffer.substr(start_pos + 10, end_pos - start_pos - 10);
        // cout << "Boundary: " << boundary << '\n';
        mail.setBoundary(boundary);

        size_t content_start_pos = buffer.find(boundary);
        // cout << "Content start pos: " << content_start_pos << '\n';
        header = buffer.substr(0, content_start_pos);
        // cout << "Header: " << header << '\n';

        size_t attachment_start_pos = buffer.find(boundary, content_start_pos + boundary.size() + 1);
        // cout << "Attachment start pos: " << attachment_start_pos << '\n';
        content = buffer.substr(content_start_pos + boundary.size() + 1, attachment_start_pos - content_start_pos - boundary.size() - 1);
        // cout << "Content: " << content << '\n';

        while (attachment_start_pos != string::npos)
        {
            size_t attachment_end_pos = buffer.find(boundary, attachment_start_pos + boundary.size() + 1) != string::npos ? buffer.find(boundary, attachment_start_pos + boundary.size() + 1) : 0;
            // cout << "Attachment start pos: " << attachment_start_pos << '\n';
            // cout << "Attachment end pos: " << attachment_end_pos << '\n';
            string attachment = buffer.substr(attachment_start_pos, attachment_end_pos - attachment_start_pos);
            // cout << "Attachment: " << attachment << '\n';
            attachments.push_back(attachment);
            if (attachment_end_pos == 0)
            {
                break;
            };
            attachment_start_pos = buffer.find(boundary, attachment_end_pos + boundary.size() + 1);
        }
    }
    else
    {
        size_t content_start_pos = buffer.find("\r\n\r\n");
        header = buffer.substr(0, content_start_pos);
        content = buffer.substr(content_start_pos);
    }

    if (!header.empty())
    {
        stringstream ss(header);
        string line;
        while (getline(ss, line, '\n'))
        {
            if (line.find("Message-ID:") != string::npos)
            {
                string message_id = line.substr(line.find(":") + 2);
                mail.setMessageID(message_id);
            }
            else if (line.find("Date:") != string::npos)
            {
                string date = line.substr(line.find(":") + 2);
                mail.setDate(date);
            }
            else if (line.find("MIME-Version:") != string::npos)
            {
                string mime_version = line.substr(line.find(":") + 2);
                mail.setMimeVersion(mime_version);
            }
            else if (line.find("User-Agent:") != string::npos)
            {
                string user_agent = line.substr(line.find(":") + 2);
                mail.setUserAgent(user_agent);
            }
            else if (line.find("Content-Language:") != string::npos)
            {
                string content_language = line.substr(line.find(":") + 2);
                mail.setUserAgent(content_language);
            }
            else if (line.find("To:") != string::npos)
            {
                string sender = line.substr(line.find(":") + 2);
                mail.setSender(sender);
            }
            else if (line.find("From:") != string::npos)
            {
                string recipient = line.substr(line.find(":") + 2);
                mail.setRecipient(recipient);
            }
        }
    }

    if (!content.empty())
    {
        mail.setContent(content);
    }

    if (!attachments.empty())
    {
        vector<File> files;
        for (string attachment : attachments)
        {
            // cout << "----------------Attachment----------------\n";
            // cout << "Attachment size: " << attachments.size() << '\n';
            size_t start_pos = attachment.find("Content-Type:") + 14;
            size_t end_pos = attachment.find(";", start_pos);
            string content_type = attachment.substr(start_pos, end_pos - start_pos);

            start_pos = attachment.find("name=") + 6;
            // cout << "Start pos: " << start_pos << '\n';
            end_pos = attachment.find("\"", start_pos);
            // cout << "End pos: " << end_pos << '\n';
            string file_name = attachment.substr(start_pos, end_pos - start_pos);
            // cout << "File name: " << file_name << '\n';

            // start_pos = attachment.find("\r\n\r\n", end_pos) + 4;
            string fie_content = attachment.substr(start_pos);
            // cout << "File content: " << fie_content << '\n';

            File file(file_name);
            file.setContentType(content_type);
            file.setBase64Content(fie_content);
            files.push_back(file);
        }
        mail.setAttachments(files);
    }

    return mail;
}

void Pop3Client::cutResponse()
{
    // size_t end_pos = this->receive.find("\r\n.\r\n");
    // this->receive = this->receive.substr(0, end_pos);
    return;
}