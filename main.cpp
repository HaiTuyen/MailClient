#include "MailClient.h"
#include "File.h"
#include <limits> // for numeric_limits

int main()
{
    MailClient mail_client;
    int main_choice = 0, edit_choice = 0, receive_choice = 0, mailbox_choice = 0, download_choice = 0;
    int attachmentChoice = 0;
    string to = "", cc = "", bcc = "", subject = "", content = "";
    string mailbox_folder = "inbox";
    vector<Mail> mails;

    std::string new_sender_email;
    std::string new_smtp_server;
    std::string new_smtp_port;
    std::string new_pop3_server;
    std::string new_pop3_port;

    vector<string> attachment_paths;
    
    while (main_choice != 4)
    {
        cout << "-----------------MENU------------------\n";
        cout << "| 1. Send email                        |\n";
        cout << "| 2. Received email                    |\n";
        cout << "| 3. Edit account                      |\n";
        cout << "| 4. Exit                              |\n";
        cout << "---------------------------------------\n";
        cout << "Enter your choice:";
        while(!(cin >> main_choice) || main_choice < 1 || main_choice > 4) {
            cin.clear(); // clear the error flags
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // ignore the rest of the line
            cout << "Invalid input. Please enter a number between 1 and 4: ";
        }
        switch (main_choice)
        {
        case 1:
            cout << "-------------------------- SENDING EMAIL CONSOLE --------------------------\n";
            cout << "(Fill in the information below, press ENTER to skip)\n";
            cout << "| To (separate by ',' if multiple): ";
            cin.ignore();
            getline(cin, to);
            cout << "| CC (separate by ',' if multiple): ";
            getline(cin, cc);
            cout << "| BCC (separate by ',' if multiple): ";
            getline(cin, bcc);
            cout << "| Subject: ";
            getline(cin, subject);
            cout << "| Content: ";
            getline(cin, content);
            cout << "| Do you want to attach a file? (1. Yes, 2. No): ";
            cin >> attachmentChoice;
            if (attachmentChoice == 1)
            {
                string attachment_path;
                cin.ignore();
                do
                {
                    attachment_path = "";
                    cout << "|-- Attachment path (ENTER to exit): ";
                    getline(cin, attachment_path);
                    if (!attachment_path.empty())
                    {
                        attachment_paths.push_back(attachment_path);
                    }
                } while (!attachment_path.empty());
            }
            else if (attachmentChoice == 2)
            {
            }

            if (to.empty() || subject.empty() || content.empty())
            {
                cout << "[ERROR]To, Subject and Content are required\n";
                break;
            }
            else
            {
                if (mail_client.sendMail(to, cc, bcc, subject, content, attachment_paths) < 0)
                {
                    cout << "[ERROR] Failed to send email\n";
                }
                else
                {
                    cout << "[INFO] Email sent successfully\n";
                }
                attachment_paths.clear();
            }
            break;
        case 2:
            receive_choice = 0;
            while (receive_choice != 6)
            {
                mails.clear();

                // Pull new emails from server (if any)
                mail_client.pullNewEmails();

                mailbox_choice = 0;
                cout << "------------------------- RECEIVING EMAIL CONSOLE -------------------------\n";
                cout << "| Select a folder to see emails:                                           |\n";
                cout << "| 1. Inbox                                                                 |\n";
                cout << "| 2. Project                                                               |\n";
                cout << "| 3. Important                                                             |\n";
                cout << "| 4. Work                                                                  |\n";
                cout << "| 5. Spam                                                                  |\n";
                cout << "| 6. Exit                                                                  |\n";
                cout << "---------------------------------------------------------------------------\n";
                cout << "Enter your choice:";
                cin >> receive_choice;
                switch (receive_choice)
                {
                case 1:
                    mailbox_folder = "inbox";
                    break;
                case 2:
                    mailbox_folder = "project";
                    break;
                case 3:
                    mailbox_folder = "important";
                    break;
                case 4:
                    mailbox_folder = "work";
                    break;
                case 5:
                    mailbox_folder = "spam";
                    break;

                default:
                    break;
                }

                while (mailbox_choice == 0 && receive_choice != 6)
                {
                    mails = mail_client.getMailsInFolder(mailbox_folder);
                    cout << "mails size: " << mails.size() << endl;
                    cout << "------------------------------- MAIL BOX -------------------------------\n";
                    for (size_t i = 0; i < mails.size(); i++)
                    {
                        cout << "| " << i + 1 << ". ";
                        if (mail_client.getStatus(mails[i].getMessageID()) == 0)
                        {
                            cout << "[NEW] \r\n";
                        }
                        else
                        {
                            cout << "\r\n";
                        }
                        {
                            cout << "| From: " << mails[i].getSender() << endl;
                            cout << "| Subject: " << mails[i].getSubject() << endl;
                            cout << "| Date: " << mails[i].getDate() << endl;
                        }
                    }
                    cout << "-----------------------------------------------------------------------\n";
                    do
                    {
                        cout << "Select an email to view (press 0 to show mails list, press -1 to exit): ";
                        cin >> mailbox_choice;
                        switch (mailbox_choice)
                        {
                        case 0:
                            break;
                        case -1:
                            break;
                        default:
                            mail_client.updateStatus(mails[mailbox_choice - 1].getMessageID(), 1);
                            cout << "-------------------------------- EMAIL --------------------------------\n";
                            cout << "From: " << mails[mailbox_choice - 1].getSender() << endl;
                            cout << "To: " << mails[mailbox_choice - 1].getRecipient() << endl;
                            cout << "Subject: " << mails[mailbox_choice - 1].getSubject() << endl;
                            cout << "Date: " << mails[mailbox_choice - 1].getDate() << endl;
                            cout << "Content: " << mails[mailbox_choice - 1].getContent() << endl;
                            if (mails[mailbox_choice - 1].getAttachments().size() > 0)
                            {
                                cout << "Attachments: " << endl;
                                for (size_t i = 0; i < mails[mailbox_choice - 1].getAttachments().size(); i++)
                                {
                                    cout << "---> Attachment " << i + 1 << ": " << mails[mailbox_choice - 1].getAttachments()[i].getName() << endl;
                                }
                                cout << "Do you want to download all attachments? (1. Yes, 2. No): ";
                                try
                                {
                                    cin >> download_choice;
                                    if (download_choice == 1)
                                    {
                                        cout << "Enter the folder path to download (ENTER to exit): ";
                                        cin.ignore();
                                        string download_path;
                                        getline(cin, download_path);
                                        if (!download_path.empty())
                                        {
                                            if (download_path.back() != '/')
                                            {
                                                download_path += '/';
                                            }
                                            for (size_t i = 0; i < mails[mailbox_choice - 1].getAttachments().size(); i++)
                                            {
                                                mails[mailbox_choice - 1].getAttachments()[i].writeFile(mails[mailbox_choice - 1].getAttachments()[i].getBase64Content(), download_path + mails[mailbox_choice - 1].getAttachments()[i].getName());
                                            }
                                        }
                                        else
                                        {
                                            cout << "[Eror]: Path is required\n";
                                        }
                                    }
                                }
                                catch (const std::exception &e)
                                {
                                    cout << "[Eror]: " << e.what() << "\n";
                                }
                            }
                            cout << "-----------------------------------------------------------------------\n";
                            break;
                        }
                    } while (mailbox_choice != -1 && mailbox_choice != 0);
                }
            }

            break;
        case 3:
            while (edit_choice != 6)
            {
                cout << "--------------------- EDIT ACCOUNT ---------------------\n";
                cout << "| 1. Change sender email                               |\n";
                cout << "| 2. Change SMTP server                                |\n";
                cout << "| 3. Change SMTP port                                  |\n";
                cout << "| 4. Change POP3 server                                |\n";
                cout << "| 5. Change POP3 port                                  |\n";
                cout << "| 6. Exit                                              |\n";
                cout << "--------------------------------------------------------\n";
                cout << "Enter your choice:";
                cin >> edit_choice;
                cin.ignore();
                switch (edit_choice)
                {
                case 1:
                    cout << "Enter new sender email: ";
                    getline(cin, new_sender_email);
                    mail_client.updateSenderEmail(new_sender_email);
                    break;
                case 2:
                    cout << "Enter new SMTP server: ";
                    getline(cin, new_smtp_server);
                    mail_client.updateSmtpServer(new_smtp_server);
                    break;
                case 3:
                    cout << "Enter new SMTP port: ";
                    getline(cin, new_smtp_port);
                    mail_client.updateSmtpPort(std::stoi(new_smtp_port));
                    break;
                case 4:
                    cout << "Enter new POP3 server: ";
                    getline(cin, new_pop3_server);
                    mail_client.updatePop3Server(new_pop3_server);
                    break;
                case 5:
                    cout << "Enter new POP3 port: ";
                    getline(cin, new_pop3_port);
                    mail_client.updatePop3Port(std::stoi(new_pop3_port));
                    break;
                }
            }
            break;
        case 4:
            cout << "Exit\n";
            break;
        default:
            cout << "Invalid choice\n";
            break;
        }
    }

    // string email_content = "Hello, I send this in C++ code. TO(myself), CC(user02) and BCC(user03)\n";
    // // for (int i = 1; i <= 800000; i++) {
    // //     email_content += to_string(i) + ".Hello, this is a test email from SMTP client\n";
    // // };

    // MailClient client;
    // vector<string> rcpts_to = {"haituyen@fitus.hcmus.vn"};
    // vector<string> rcpts_cc = {};
    // vector<string> rcpts_bcc = {};
    // vector<string> attachment_paths = {"/home/kali/Desktop/Huong Dan.pdf", "/home/kali/Desktop/glass-ball.jpg"};
    // client.sendMail("hoho@kakaka.com", rcpts_to, rcpts_cc, rcpts_bcc, "From Kali, sending with a PDF attachment", email_content, attachment_paths);

    // File file("/home/kali/Desktop/HaiTuyen.pdf");

    // string encoded = file.readAndEncode();
    // cout << encoded << endl;
    // string namefile = file.getName();
    // cout << namefile << endl;

    // File file2("/home/kali/Desktop/HaiTuyen2.pdf");
    // file2.writeFile(encoded);
}