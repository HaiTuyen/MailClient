<p align="center">
  <a href="" rel="noopener">
  <h1 align="center">MAIL CLIENT</h1>
</p>


<div align="center">

[![Status](https://img.shields.io/badge/status-active-success.svg)]()
[![GitHub Issues](https://img.shields.io/github/issues/HaiTuyen/MailClient.svg)](https://github.com/HaiTuyen/MailClient/issues)
[![GitHub Pull Requests](https://img.shields.io/github/issues-pr/HaiTuyen/MailClient.svg)](https://github.com/HaiTuyen/MailClient/pulls)

</div>

---

<p align="center">A CLI application that enables user to send and receive emails using POP3 and SMTP protocols.
</p>


## 🧐 About <a name = "about"></a>
This project is a Mail Client application developed in C++ for a Networking course. The application uses the Post Office Protocol (POP3) for receiving emails and the Simple Mail Transfer Protocol (SMTP) for sending emails.

The main functionalities of this application include sending and receiving emails, with the capability to send emails with file attachments. Additionally, it can filter emails using a configuration file. This project serves as a practical application of network programming concepts and provides hands-on experience with widely-used email protocols.



## 🏁 Getting Started <a name = "getting_started"></a>

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes.

### Prerequisites
- To simplify, a mock Mail Server is needed. It can be downloaded from this link: https://github.com/eugenehr/test-mail-server/releases/download/1.0/test-mail-server-1.0.jar.

- In order to run the mail server, Java 8 (JDK 8u361) is required. The following command can be used to run the server with SMTP port 2225 and POP3 port 3335:
```
java -jar test-mail-server-1.0.jar -s 2225 -p 3335 -m ./
```
- Run on Linux operating system.


### Installing
Here are the steps to install the application:

1. Clone the repository to your machine:
```bash
git clone https://github.com/HaiTuyen/MailClient.git
```
2. Navigate to the directory containing the source code, create a `build` directory and navigate into it:
```bash
mkdir build
cd build
```
3. Run `cmake` from the `build` directory:
```bash
cmake ..
```
4. Build the executable file and run it:
```bash
make
./MailClient
```

## ✍️ Authors <a name = "authors"></a>
- [@HaiTuyen](https://github.com/HaiTuyen)
- [@NoxionK](https://github.com/NoxionK)
- [@HungPhan2003](https://github.com/HungPhan2003)
