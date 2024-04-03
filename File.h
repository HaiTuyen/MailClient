#ifndef FILE_H
#define FILE_H

#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include <algorithm>
using namespace std;


class File {
private:
    string path;
    string name;
    string ext;
    string content_type;
    string base64_content;


public:
    File (const string& path);
    string readAndEncode();
    string getName();
    string getPath();
    string getExt();
    string getContentType();
    string getBase64Content();
    
    void write(const string& data);
    string decode_base64(const string &data);
    void writeFile(const string &encodedContent, const string &path);

    void setFilename(const string& filename);
    void setContentType(const string& content_type);
    void setBase64Content(const string& base64_content);
};


#endif // FILE_H