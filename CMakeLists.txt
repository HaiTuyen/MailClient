cmake_minimum_required(VERSION 3.28)

project(MailClient)

include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(json)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(MailClient main.cpp SmtpClient.cpp Pop3Client.cpp MailClient.cpp File.cpp Mail.cpp)

add_library(sqlite3 sqlite3.c)

target_link_libraries(MailClient sqlite3 nlohmann_json::nlohmann_json)


