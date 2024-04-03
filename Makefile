CC = g++
CFLAGS = -Wall -Wextra

SRCS = main.cpp SmtpClient.cpp Pop3Client.cpp MailClient.cpp File.cpp Mail.cpp
OBJS = $(SRCS:.cpp=.o)

TARGET = mailClient

all: $(TARGET)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

.PHONY: clean

clean:
	rm -f $(OBJS) $(TARGET)