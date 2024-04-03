#include "MailClient.h"
#include "File.h"

int main()
{
    MailClient mail_client;
    int main_choice = 0, edit_choice = 0, receive_choice = 0;
    int attachmentChoice = 0;
    string to = "", cc = "", bcc = "", subject = "", content = "";

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
        cin >> main_choice;
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
            while (receive_choice != 6)
            {
                int inbox_choice = 0;
                vector<Mail> mails;
                cout << "------------------------- RECEIVING EMAIL CONSOLE -------------------------\n";
                cout << "| Select a folder to see emails:                                           |\n";
                cout << "| 1. Inbox                                                                 |\n";
                cout << "| 2. Projects                                                              |\n";
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
                    mail_client.retriveLocalEmails("storage/inbox/");

                    while (inbox_choice == 0)
                    {
                        mails = mail_client.getMails("storage/inbox/");
                        // cout << "mails size: " << mails.size() << endl;
                        cout << "-------------------------------- INBOX --------------------------------\n";
                        for (size_t i = 0; i < mails.size(); i++)
                        {
                            string temp = "ID: " + mails[i].getMessageID() + "Hehe" + "\r\n";
                            cout << "| " << i + 1 << ". " << endl;
                            cout << "| From: " << mails[i].getSender() << endl;
                            cout << "| Subject: " << mails[i].getSubject() << endl;
                            cout << "| Date: " << mails[i].getDate() << endl;
                        }
                        cout << "-----------------------------------------------------------------------\n";
                        do
                        {
                            cout << "Select an email to view (press 0 to show mails list, press -1 to exit): ";
                            cin >> inbox_choice;
                            switch (inbox_choice)
                            {
                            case 0:
                                break;
                            case -1:
                                break;
                            default:
                                cout << "-------------------------------- EMAIL --------------------------------\n";
                                cout << "From: " << mails[inbox_choice - 1].getSender() << endl;
                                cout << "To: " << mails[inbox_choice - 1].getRecipient() << endl;
                                cout << "Subject: " << mails[inbox_choice - 1].getSubject() << endl;
                                cout << "Date: " << mails[inbox_choice - 1].getDate() << endl;
                                cout << "Content: " << mails[inbox_choice - 1].getContent() << endl;
                                if (mails[inbox_choice - 1].getAttachments().size() > 0)
                                {
                                    cout << "Attachments: " << endl;
                                    for (size_t i = 0; i < mails[inbox_choice - 1].getAttachments().size(); i++)
                                    {
                                        cout << "---> Attachment " << i + 1 << ": " << mails[inbox_choice - 1].getAttachments()[i].getName() << endl;
                                    }
                                    cout << "Do you want to download all attachments? (1. Yes, 2. No): ";
                                    try
                                    {
                                        int download_choice;
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
                                                for (size_t i = 0; i < mails[inbox_choice - 1].getAttachments().size(); i++)
                                                {
                                                    mails[inbox_choice - 1].getAttachments()[i].writeFile(mails[inbox_choice - 1].getAttachments()[i].getBase64Content(), download_path + mails[inbox_choice - 1].getAttachments()[i].getName());
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
                                        // cout << "Error: " << e.what() << "\n";
                                    }
                                }
                                cout << "-----------------------------------------------------------------------\n";
                                break;
                            }
                        } while (inbox_choice != -1 && inbox_choice != 0);
                    }
                    break;
                case 2:
                    break;
                case 3:
                    break;
                case 4:
                    break;
                case 5:
                    break;

                default:
                    cout << "Invalid choice\n";
                    break;
                }
            }

            // mail_client.getMail();
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
                    break;
                case 2:
                    cout << "Enter new SMTP server: ";
                    break;
                case 3:
                    cout << "Enter new SMTP port: ";
                    break;
                case 4:
                    cout << "Enter new POP3 server: ";
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