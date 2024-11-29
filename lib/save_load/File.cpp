#include "../../include/save_load/File.h"

File::File(std::string name)
    : _name{name}
{}


void File::open_read()
{
    _in.open(_name, std::ios::in);
    if (!_in.is_open())
        throw std::runtime_error("Could not open file for reading!");
}

void File::open_write()
{
    _out.open(_name, std::ios::trunc | std::ios::out);
    if (!_out.is_open())
        throw std::runtime_error("Could not open file for writing!");
}

void File::read(json &j)
{
    if (_in.is_open())
        _in >> j;
    else
        throw std::runtime_error("File not opened for reading!");
}

void File::write(const json &j)
{
    if (_out.is_open())
        _out << j.dump(4);
    else
        throw std::runtime_error("File not opened for writing!");
}

void File::close_read()
{
    if (_in.is_open())
        _in.close();
}

void File::close_write()
{
    if (_out.is_open())
        _out.close();
}

File::~File()
{
    close_read();
    close_write();
}
