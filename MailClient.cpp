#include "MailClient.h"

MailClient::MailClient(/* args */) 
{
    this->config = readJSONFile("../config.json");
    // cout << this->config.dump(4) << '\n';
    this->USERNAME = this->config["username"];
    this->PASSWORD = this->config["password"];
    this->SMTP_SERVER = this->config["smtp_server"];
    this->SMTP_PORT = this->config["smtp_port"];
    this->POP3_SERVER = this->config["pop3_server"];
    this->POP3_PORT = this->config["pop3_port"];
    this->DATABASE = this->config["database_name"];
    this->STORAGE_DIR = this->config["storage_dir"];
    this->AUTOLOAD_SEC = this->config["auto_load"];

    initDatabase();
    // Update this->local_uids
    retriveAllLocalEmails(STORAGE_DIR);
    startPullEmailThread();
}

MailClient::~MailClient()
{
    this->stopPullEmailThread();
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

    Mail mail(USERNAME, rcpts_to, rcpts_cc, rcpts_bcc, subject, content, "", attachmemts);
    if (client.sendMail(mail) < 0)
        return -1;
    return 0;
}

vector<Mail> MailClient::getMailsInFolder(string folder)
{
    try
    {
        this->pullNewEmails();
        vector<string> file_name_list = getFileNamesInFolder(folder);
        vector<Mail> mails;

        DIR *dir;
        struct dirent *ent;

        if ((dir = opendir(STORAGE_DIR.c_str())) == NULL)
        {
            if (mkdir(folder.c_str(), 0777) == -1)
            {
                perror("[Sqlite3] Error creating directory (getMailsInFolder)");
            }

            dir = opendir(folder.c_str());
            if (dir == NULL)
            {
                perror("[Sqlite3] Error opening newly created directory (getMailsInFolder)");
            }
        }

        while ((ent = readdir(dir)) != NULL)
        {
            string filename = ent->d_name;
            if (filename != "." && filename != "..")
            {
                if (find(file_name_list.begin(), file_name_list.end(), filename) != file_name_list.end())
                {
                    string extension = filename.substr(filename.find_last_of(".") + 1);
                    if (extension.compare("msg"))
                    {
                        ifstream file(STORAGE_DIR + filename);
                        stringstream buffer;
                        buffer << file.rdbuf();
                        string contents = buffer.str();
                        // Parse the contents into a Mail object and add it to the mails vector
                        Mail mail(USERNAME, vector<string>(), vector<string>(), vector<string>(), "", "", filename, vector<File>());
                        mail.parseEmail(contents);
                        mails.push_back(mail);
                    }
                }
            }
        }
        closedir(dir);

        return mails;
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
    Pop3Client client(POP3_SERVER, POP3_PORT, USERNAME, this->local_uids, STORAGE_DIR);
    client.getEmails();

    vector<Mail> local_mails = retriveAllLocalEmails(STORAGE_DIR);
    updateDatabaseFromLocalStorage(local_mails);
}

vector<Mail> MailClient::retriveAllLocalEmails(string folder)
{
    vector<Mail> mails;
    DIR *dir;
    struct dirent *ent;
    try
    {
        if ((dir = opendir(folder.c_str())) == NULL)
        {
            if (mkdir(folder.c_str(), 0777) == -1)
            {
                perror("[Sqlite3] Error creating directory (retriveAllLocalEmails)");
            }

            dir = opendir(folder.c_str());
            if (dir == NULL)
            {
                perror("[Sqlite3] Error opening newly created directory (retriveAllLocalEmails)");
            }
        }

        while ((ent = readdir(dir)) != NULL)
        {
            // cout << "----------------\n";
            string filename = ent->d_name;
            // cout << "Filename: " << filename << '\n';
            if (filename != "." && filename != "..")
            {
                try
                {
                    this->local_uids.push_back(filename);
                    string extension = filename.substr(filename.find_last_of(".") + 1);
                    // cout << "Extension: " << extension << '\n';
                    if (extension.compare("msg"))
                    {
                        ifstream file(folder + filename);
                        stringstream buffer;
                        buffer << file.rdbuf();
                        string contents = buffer.str();
                        // Parse the contents into a Mail object and add it to the mails vector
                        Mail mail(USERNAME, vector<string>(), vector<string>(), vector<string>(), "", "", filename, vector<File>());
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
                }
                catch (const std::exception &e)
                {
                    std::cerr << e.what() << '\n';
                }
            }
        }
        closedir(dir);
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

void MailClient::initDatabase()
{
    sqlite3 *db;
    int rc = sqlite3_open(DATABASE.c_str(), &db);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "[Sqlite3]Can't open database (initDatabase): %s\n", sqlite3_errmsg(db));
        return;
    }
    else
    {
    }

    string sql = "CREATE TABLE IF NOT EXISTS MAIL ("
                 "id VARCHAR(100) PRIMARY KEY,"
                 "sender VARCHAR(255) NOT NULL,"
                 "receiver VARCHAR(255) NOT NULL,"
                 "subject VARCHAR(255) NOT NULL,"
                 "content TEXT NOT NULL,"
                 "folder VARCHAR(255) DEFAULT 'inbox',"
                 "status BOOLEAN DEFAULT FALSE,"
                 "filename VARCHAR(255) NOT NULL);";

    char *err_msg = 0;
    rc = sqlite3_exec(db, sql.c_str(), 0, 0, &err_msg);

    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "[Sqlite3]SQL error (initDatabase): %s\n", err_msg);
        sqlite3_free(err_msg);
    }
    else
    {
    }

    sqlite3_close(db);
}

// void addMailsToDatabase(string DATABASE, vector<Mail> mails, string folder)
// {
//     if (mails.size() == 0)
//         return;

//     sqlite3 *db;
//     int rc = sqlite3_open(DATABASE.c_str(), &db);
//     if (rc != SQLITE_OK)
//     {
//         fprintf(stderr, "[Sqlite3]Can't open database: %s\n", sqlite3_errmsg(db));
//         return;
//     }
//     else
//     {
//         fprintf(stderr, "[Sqlite3]Opened database successfully\n");
//     }

//     for (Mail mail : mails)
//     {
//         string sql = "INSERT OR IGNORE INTO MAIL (id, sender, receiver, subject, content, folder )"
//                      "VALUES ('" +
//                      mail.getMessageID() + "', '" + mail.getSender() + "', '" + mail.getRecipient() + "', '" + mail.getSubject() + "', '" + mail.getContent() + "', '" + folder + "');";

//         char *err_msg = 0;
//         rc = sqlite3_exec(db, sql.c_str(), 0, 0, &err_msg);

//         if (rc != SQLITE_OK)
//         {
//             fprintf(stderr, "[Sqlite3]SQL error: %s\n", err_msg);
//             sqlite3_free(err_msg);
//         }
//         else
//         {
//             fprintf(stdout, "[Sqlite3]Records created successfully\n");
//         }
//     }

//     sqlite3_close(db);
// }

void MailClient::updateDatabaseFromLocalStorage(vector<Mail> localMails)
{
    sqlite3 *db;
    int rc = sqlite3_open(DATABASE.c_str(), &db);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "[Sqlite3]Can't open database (updateDatabaseFromLocalStorage): %s\n", sqlite3_errmsg(db));
        return;
    }
    else
    {
    }

    vector<string> local_msg_ID;
    for (Mail mail : localMails)
    {
        local_msg_ID.push_back(mail.getMessageID());
    }

    // Remove if the mail is not in the local storage
    // ostringstream oss;
    // copy(local_msg_ID.begin(), local_msg_ID.end() - 1, ostream_iterator<string>(oss, ","));
    // oss << local_msg_ID.back();

    // string sql = "DELETE FROM MAIL WHERE ID NOT IN (" + oss.str() + ");";
    // char *err_msg = 0;
    // rc = sqlite3_exec(db, sql.c_str(), 0, 0, &err_msg);

    // if (rc != SQLITE_OK)
    // {
    //     fprintf(stderr, "[Sqlite3]SQL error (updateDatabaseFromLocalStorage): %s\n", err_msg);
    //     sqlite3_free(err_msg);
    // }
    // else
    // {
    // }

    // Add if the mail is not in the database

    for (Mail mail : localMails)
    {
        string sql = "INSERT OR IGNORE INTO MAIL (id, sender, receiver, subject, content, folder, filename)"
                     "VALUES ('" +
                     mail.getMessageID() + "', '" + mail.getSender() + "', '" + mail.getRecipient() + "', '" + mail.getSubject() + "', '" + mail.getContent() + "', '" + this->filterFolder(mail) + "', '" + mail.getFileName() + "');";

        char *err_msg = 0;
        rc = sqlite3_exec(db, sql.c_str(), 0, 0, &err_msg);

        if (rc != SQLITE_OK)
        {
            fprintf(stderr, "[Sqlite3]SQL error (updateDatabaseFromLocalStorage): %s\n", err_msg);
            sqlite3_free(err_msg);
        }
        else
        {
        }
    }

    sqlite3_close(db);
}

void MailClient::updateStatus(string message_id, int status)
{
    sqlite3 *db;
    int rc = sqlite3_open(DATABASE.c_str(), &db);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "[Sqlite3]Can't open database (updateStatus): %s\n", sqlite3_errmsg(db));
        return;
    }
    else
    {
    }

    string sql = "UPDATE MAIL SET status = " + to_string(status) + " WHERE id = '" + message_id + "';";
    char *err_msg = 0;
    rc = sqlite3_exec(db, sql.c_str(), 0, 0, &err_msg);

    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "[Sqlite3]SQL error (updateStatus): %s\n", err_msg);
        sqlite3_free(err_msg);
    }
    else
    {
    }

    sqlite3_close(db);
}

int MailClient::getStatus(string message_id)
{
    sqlite3 *db;
    int rc = sqlite3_open(DATABASE.c_str(), &db);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "[Sqlite3]Can't open database (getStatus): %s\n", sqlite3_errmsg(db));
        return -1;
    }
    else
    {
    }

    string sql = "SELECT status FROM MAIL WHERE id = '" + message_id + "';";
    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "[Sqlite3]SQL error (getStatus): %s\n", sqlite3_errmsg(db));
        return -1;
    }

    int status = -1;
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        status = sqlite3_column_int(stmt, 0);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return status;
}

vector<string> MailClient::getFileNamesInFolder(string folder)
{
    vector<string> file_names;
    sqlite3 *db;
    int rc = sqlite3_open(DATABASE.c_str(), &db);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "[Sqlite3]Can't open database (getFileNamesInFolder): %s\n", sqlite3_errmsg(db));
        return file_names;
    }
    else
    {
    }

    string sql = "SELECT filename FROM MAIL WHERE folder = '" + folder + "';";
    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "[Sqlite3]SQL error (getFileNamesInFolder): %s\n", sqlite3_errmsg(db));
        return file_names;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        string filename = string(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0)));
        file_names.push_back(filename);
    }

    return file_names;
}


nlohmann::json MailClient::readJSONFile(string file_path)
{
    ifstream file(file_path);
    nlohmann::json j;
    file >> j;
    file.close();
    return j;
}

string MailClient::filterFolder(Mail mail)
{
    nlohmann::json filter_rules = this->config["filter_rules"];

    for (auto& rule: filter_rules)
    {
        string name = rule["name"];
        nlohmann::json conditions = rule["conditions"];
        for (auto& condition: conditions)
        {
            if (name == "onSender")
            {
                auto senders = condition["senders"].get<vector<string>>();
                if (find(senders.begin(), senders.end(), mail.getSender()) != senders.end())
                {
                    return condition["folder"];
                }
            }
            else if (name == "onSubject")
            {
                auto words = condition["words"].get<vector<string>>();
                for (auto& word : words)
                {
                    if (mail.getSubject().find(word) != string::npos)
                    {
                        return condition["folder"];
                    }
                }
            }
            else if (name == "onContent")
            {
                auto words = condition["words"].get<vector<string>>();
                for (auto& word : words)
                {
                    if (mail.getContent().find(word) != string::npos)
                    {
                        return condition["folder"];
                    }
                }
            }
            else if (name == "onContent&Subject")
            {
                auto words = condition["words"].get<vector<string>>();
                for (auto& word : words)
                {
                    if ((mail.getContent().find(word) != string::npos) || (mail.getSubject().find(word) != string::npos))
                    {
                        return condition["folder"];
                    }
                }
            }
        }
    }

    return "inbox";
}


void MailClient::startPullEmailThread()
{
    this->pullEmailThread = thread([this] {
        while (!this->stopFlag)
        {
            // cout << "I am gonna pull new emails\n";
            this->pullNewEmails();
            this_thread::sleep_for(chrono::seconds(this->AUTOLOAD_SEC));
        }
    });
}

void MailClient::stopPullEmailThread()
{
    this->stopFlag = true;
    this->pullEmailThread.join();
}