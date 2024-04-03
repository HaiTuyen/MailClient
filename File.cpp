#include "File.h"

File::File(const string &path) : path(path)
{
    // Get the file name
    size_t found = path.find_last_of("/\\");
    this->name = path.substr(found + 1);
    this->ext = (this->name.substr(this->name.find_last_of(".")));
    std::transform(this->ext.begin(), this->ext.end(), this->ext.begin(), ::tolower);
}

string File::readAndEncode()
{
    ifstream file(path, ios::binary);
    ostringstream ss;
    ss << file.rdbuf();
    string contents = ss.str();

    // Encode contents in base64
    using namespace boost::archive::iterators;
    typedef base64_from_binary<transform_width<string::const_iterator, 6, 8>> base64_enc;

    stringstream os;
    // copy(base64_enc(contents.begin()), base64_enc(contents.end()), ostream_iterator<char>(os));
    auto begin = base64_enc(contents.begin());
    auto end = base64_enc(contents.end());
    int count = 0;
    for (auto it = begin; it != end; ++it)
    {
        os << *it;
        if (++count == 100)
        {
            os << '\r' << '\n';
            count = 0;
        }
    }
    return os.str();
}

string File::getName()
{
    return this->name;
}

string File::getPath()
{
    return this->path;
}

string File::getExt()
{
    return this->ext;
}

string File::getContentType()
{
    return this->content_type;
}

string File::getBase64Content()
{
    return this->base64_content;
}

void File::write(const string &data)
{
    ofstream file(path, ios::binary);
    file << data;
}

string File::decode_base64(const string &data)
{
    try
    {
        using namespace boost::archive::iterators;
        typedef transform_width<binary_from_base64<string::const_iterator>, 8, 6> binary_t;

        unsigned int size = data.size();
        // Remove the padding characters
        if (size && data[size - 1] == '=')
        {
            --size;
            if (size && data[size - 1] == '=')
                --size;
        }
        if (size == 0)
            return string();

        stringstream os;
        copy(binary_t(data.begin()), binary_t(data.begin() + size), ostream_iterator<char>(os));
        return os.str();
    } 
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return "";
    }
}

void File::writeFile(const string &encodedContent, const string &path)
{
    try
    {
        string temp = encodedContent;
        ofstream testfile("test.txt", ios::binary);
        testfile << temp;
        testfile.close();
        
        size_t pos = 0;
        string target = "\r\n";
        while ((pos = temp.find(target, pos)) != std::string::npos)
        {
            temp.erase(pos, target.length());
        }

        string decodedContent = decode_base64(temp);
        ofstream file(path, ios::binary);
        file << decodedContent;
    }
    catch (const std::exception &e)
    {
        // std::cerr << e.what() << '\n';
    }
}

void File::setFilename(const string &filename)
{
    this->name = filename;
}

void File::setContentType(const string &content_type)
{
    this->content_type = content_type;
}

void File::setBase64Content(const string &base64_content)
{
    this->base64_content = base64_content;
}