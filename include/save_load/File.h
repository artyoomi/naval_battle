#ifndef FILE_H
#define FILE_H

#include <string>
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class File {
public:
    File(std::string name);

    void open_read();
    void open_write();
    
    void read(json &j);
    void write(const json &j);

    void close_read();
    void close_write();

    void close();

    ~File();

private:
    std::string   _name;
    std::ifstream _in;
    std::ofstream _out;
};

#endif
