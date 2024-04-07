#include "Mail.h"

Mail::Mail(const string& sender, vector<string> rcpts_to, vector<string> rcpts_cc, vector<string> rcpts_bcc, const string& subject, const string& content, const string& filename, vector<File>attachments)   
: sender(sender), rcpts_to(rcpts_to), rcpts_cc(rcpts_cc), rcpts_bcc(rcpts_bcc), subject(subject), content(content), file_name(filename), attachments(attachments) {}

string Mail::getUid() {
    return this->uid;
}

string Mail::getSender() {
    regex e("\\b[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Za-z]{2,}\\b"); 
    smatch match;
    if (regex_search(this->sender, match, e)) {
        return match.str();
    } else {
        return "";
    }
}

string Mail::getRecipient() {
    return recipient;
}

vector<string> Mail::getRecipientsTo() {
    return rcpts_to;
}

vector<string> Mail::getRecipientsCc() {
    return rcpts_cc;
}

vector<string> Mail::getRecipientsBcc() {
    return rcpts_bcc;
}

string Mail::getBoundary() {
    return boundary;
}

string Mail::getMessageID() {
    return messageID;
}

string Mail::getMimeVersion() {
    return mime_version;
}

string Mail::getUserAgent() {
    return user_agent;
}

string Mail::getContentLanguage() {
    return content_language;
}

string Mail::getDate() {
    return date;
}

string Mail::getSubject() {
    return subject;
}

string Mail::getContent() {
    return content;
}

string Mail::getStatus() {
    return status;
}

string Mail::getFolder() {
    return folder;
}

string Mail::getFileName() {
    return file_name;
}

vector<File> Mail::getAttachments() {
    return attachments;
}

void Mail::setUid(const string& uid) {
    this->uid = uid;
}

void Mail::setSender(const string& sender) {
    this->sender = sender;
}

void Mail::setRecipient(const string& recipient) {
    this->recipient = recipient;
}

void Mail::setRecipientsTo(vector<string> rcpts_to) {
    this->rcpts_to = rcpts_to;
}

void Mail::setRecipientsCc(vector<string> rcpts_cc) {
    this->rcpts_cc = rcpts_cc;
}

void Mail::setRecipientsBcc(vector<string> rcpts_bcc) {
    this->rcpts_bcc = rcpts_bcc;
}

void Mail::setBoundary(const string& boundary) {
    this->boundary = boundary;
}

void Mail::setMessageID(const string& messageID) {
    this->messageID = messageID;
}

void Mail::setMimeVersion(const string& mime_version) {
    this->mime_version = mime_version;
}

void Mail::setUserAgent(const string& user_agent) {
    this->user_agent = user_agent;
}

void Mail::setContentLanguage(const string& content_language) {
    this->content_language = content_language;
}

void Mail::setDate(const string& date) {
    this->date = date;
}

void Mail::setSubject(const string& subject) {
    this->subject = subject;
}

void Mail::setContent(const string& content) {
    this->content = content;
}

void Mail::setStatus(const string& status) {
    this->status = status;
}

void Mail::setFolder(const string& folder) {
    this->folder = folder;
}

void Mail::setFileName(const string& file_name) {
    this->file_name = file_name;
}

void Mail::setAttachments(vector<File> attachments) {
    this->attachments = attachments;
}

void Mail::parseEmail(const string& buffer) {
    // cout << "-------------------Parsing email-------------------\n";
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
        this->boundary = boundary;

        size_t content_start_pos = buffer.find(boundary);
        // cout << "Content start pos: " << content_start_pos << '\n';
        header = buffer.substr(0, content_start_pos);
        // cout << "Header: " << header << '\n';

        size_t attachment_start_pos = buffer.find(boundary, content_start_pos + boundary.size() + 1);
        // cout << "Attachment start pos: " << attachment_start_pos << '\n';
        size_t content_data_start_pos = buffer.find("\r\n\r\n", content_start_pos + boundary.size() + 1);
        content = buffer.substr(content_data_start_pos, attachment_start_pos - content_data_start_pos);
        // cout << "Content: " << content << '\n';

        size_t attachment_final_pos = buffer.find(boundary + "--", attachment_start_pos + boundary.size() + 1);
        // cout << "Attachment final pos: " << attachment_final_pos << '\n';
        string attachment_content = buffer.substr(attachment_start_pos, attachment_final_pos + boundary.size() + 2 - attachment_start_pos);
        // ofstream out("attachments-content" + boundary + ".txt");
        // out << attachment_content;
        // out.close();

        // int a = 0;

        size_t end = 0;
        size_t start = 0;
        while (start != attachment_final_pos)
        {
            // cout <<"Start: " << start << '\n';
            end = attachment_content.find(boundary, start + boundary.size() + 1);
            if (end == string::npos)
            {
                break;
            }
            // cout << "End: " << end << '\n';
            string attachment = attachment_content.substr(start, end - start);
            // ofstream out("attachment" + to_string(a) + ".txt");
            // out << attachment;
            // out.close();

            // cout << "Attachment: " << attachment << '\n';
            attachments.push_back(attachment);
            start = end;
            // a++;
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
                this->messageID = message_id;
            }
            else if (line.find("Date:") != string::npos)
            {
                string date = line.substr(line.find(":") + 2);
                this->date = date;
            }
            else if (line.find("MIME-Version:") != string::npos)
            {
                string mime_version = line.substr(line.find(":") + 2);
                this->mime_version = mime_version;
            }
            else if (line.find("User-Agent:") != string::npos)
            {
                string user_agent = line.substr(line.find(":") + 2);
                this->user_agent = user_agent;
            }
            else if (line.find("Content-Language:") != string::npos)
            {
                string content_language = line.substr(line.find(":") + 2);
                this->content_language = content_language;
            }
            else if (line.find("To:") != string::npos)
            {
                string recipient = line.substr(line.find(":") + 2);
                this->recipient = recipient;
            }
            else if (line.find("Cc:") != string::npos)
            {
                string cc = line.substr(line.find(":") + 2);
                this->rcpts_cc.push_back(cc);
            }
            else if (line.find("From:") != string::npos)
            {
                string sender = line.substr(line.find(":") + 2);
                this->sender = sender;
            }

            else if (line.find("Subject:") != string::npos)
            {
                string subject = line.substr(line.find(":") + 2);
                this->subject = subject;
            }
        }
                
    }

    if (!content.empty())
    {
        this->content = content;

    }

    if (!attachments.empty())
    {
        vector<File> files;
        int a = 0;
        for (string attachment : attachments)
        {
            // ofstream out("attachment" + to_string(a) + ".txt");
            // out << attachment;
            // out.close();

            // cout << "----------------Attachment----------------\n";
            // cout << "Attachment size: " << attachments.size() << '\n';

            size_t start_pos = attachment.find("Content-Type:") + 14;
            // cout << "Start pos: " << start_pos << '\n';
            size_t end_pos = attachment.find(";", start_pos);
            // cout << "End pos: " << end_pos << '\n';
            string content_type = attachment.substr(start_pos, end_pos - start_pos);
            // cout << "Content type: " << content_type << '\n';

            start_pos = attachment.find("name=\"") + 6;
            // cout << "Start pos: " << start_pos << '\n';
            end_pos = attachment.find("\"", start_pos);
            // cout << "End pos: " << end_pos << '\n';
            string file_name = attachment.substr(start_pos, end_pos - start_pos);
            // cout << "File name: " << file_name << '\n';

            start_pos = attachment.find("\r\n\r\n", end_pos) + 4;
            // cout << "Start pos: " << start_pos << '\n';
            string fie_content = attachment.substr(start_pos);
            // cout << "File content: " << fie_content << '\n';

            File file(file_name);
            file.setContentType(content_type);
            file.setBase64Content(fie_content);
            files.push_back(file);

            // cout << "I am here 3\n";
            a++;
        }
        this->attachments = files;
    }
}