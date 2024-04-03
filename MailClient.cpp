#include "MailClient.h"

MailClient::MailClient(/* args */)
{
}

MailClient::~MailClient()
{
}

int MailClient::sendMail(string to, string cc, string bcc, string subject, string content, vector<string> attachment_paths)
{
    vector<string> rcpts_to = splitString(to, ',');
    vector<string> rcpts_cc = splitString(cc, ',');
    vector<string> rcpts_bcc = splitString(bcc, ',');

    SmtpClient client(SMTP_SERVER, SMTP_PORT);
    vector<File> attachmemts;
    for (string attachment_path : attachment_paths)
    {
        File file(attachment_path);
        attachmemts.push_back(file);
    }

    Mail mail(DEFAULT_SENDER, rcpts_to, rcpts_cc, rcpts_bcc, subject, content, attachmemts);
    if (client.sendMail(mail) < 0)
        return -1;
    return 0;
}

vector<Mail> MailClient::getMails(string folder)
{
    try
    {
        this->pullNewEmails();
        vector<Mail> local_mails = retriveLocalEmails(folder);
        return local_mails;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return vector<Mail>();
    }
}

vector<string> MailClient::splitString(string str, char delimiter)
{
    vector<string> result;
    stringstream ss(str);
    string token;

    while (getline(ss, token, delimiter))
    {
        result.push_back(token);
    }

    return result;
}

void MailClient::pullNewEmails()
{
    Pop3Client client(POP3_SERVER, POP3_PORT, DEFAULT_SENDER, this->local_uids);
    client.getEmails();
}

vector<Mail> MailClient::retriveLocalEmails(string folder)
{
    vector<Mail> mails;
    DIR *dir;
    struct dirent *ent;
    try
    {
        if ((dir = opendir(folder.c_str())) != NULL)
        {
            while ((ent = readdir(dir)) != NULL)
            {
                //cout << "----------------\n";
                string filename = ent->d_name;
                //cout << "Filename: " << filename << '\n';
                if (filename != "." && filename != "..")
                {
                    try
                    {
                        this->local_uids.push_back(filename);
                        string extension = filename.substr(filename.find_last_of(".") + 1);
                        //cout << "Extension: " << extension << '\n';
                        if (extension.compare("msg"))
                        {

                            ifstream file(folder + filename);
                            stringstream buffer;
                            buffer << file.rdbuf();
                            string contents = buffer.str();
                            // Parse the contents into a Mail object and add it to the mails vector
                            Mail mail(DEFAULT_SENDER, vector<string>(), vector<string>(), vector<string>(), "", "", vector<File>());
                            mail.parseEmail(contents);
                            // cout << "Subject: " << mail.getSubject() << "\r\n";
                            // cout << "Recipient: " << mail.getRecipient() << "\r\n";
                            // cout << "Date: " << mail.getDate() << "\r\n";
                            // cout << "Content: " << mail.getContent() << "\r\n";
                            // cout << "Attachments: " << mail.getAttachments().size() << "\r\n";
                            // cout << "Sender: " << mail.getSender() << "\r\n";
                            // string temp;
                            // temp += "Message ID: " + mail.getMessageID() + "\r\n";
                            // temp += "Date: " + mail.getDate() + "\r\n";
                            // temp += "MIME-Version: " + mail.getMimeVersion() + "\r\n";
                            // temp += "User-Agent: " + mail.getUserAgent() + "\r\n";
                            // temp += "Content-Language: " + mail.getContentLanguage() + "\r\n";
                            // temp += "To: " + mail.getRecipient() + "\r\n";
                            // temp += "From: " + mail.getSender() + "\r\n";
                            // temp += "Subject: " + mail.getSubject() + "\r\n";
                            // temp += "Content: " + mail.getContent() + "\r\n";
                            // temp += "Attachments: " + to_string(mail.getAttachments().size()) + "\r\n";

                            // ofstream out("test_" + filename + ".txt");
                            // out << temp;
                            // out.close();

                            mails.push_back(mail);
                        }
                    } catch (const std::exception &e)
                    {
                        std::cerr << e.what() << '\n';
                    }
                }
            }
            closedir(dir);
        }
        else
        {
            // Could not open directory
            perror("");
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }

    // cout << "Mails size: " << mails.size() << '\n';
    // for (Mail mail : mails)
    // {
    //     cout << "Mail: " << mail.getSubject() << '\n';
    // }
    // cout << "Mails size: " << mails.size() << '\n';
    return mails;
}
