#pragma once
#include <fstream>
#include <string>

namespace BinaryUtils {

// zapisva string v dvoichen fail
inline void writeString(std::ofstream& out, const std::string& str) {
    size_t len = str.size();
    out.write(reinterpret_cast<const char*>(&len), sizeof(len));
    if (len > 0) {
        ///c_str() превръща стринг в масив от чарове като поставя терминираша 0 накрая
        out.write(str.c_str(), len); //pishem vuv faila;https://cplusplus.com/forum/beginner/166519/
    }
}

// chete string ot dvoichen fail
inline std::string readString(std::ifstream& in) {
    size_t len = 0;
    in.read(reinterpret_cast<char*>(&len), sizeof(len));

    char* buffer = new char[len + 1];
    in.read(buffer,len);

    buffer[len] = '\0';
    std::string str = buffer;
    delete[] buffer;

    return str;
}

//pomoshten template za zapisvane na tipovene int,unsigned,bool,double..
template <typename T>
    void writePrimitive(std::ofstream& out, const T& val) {
    out.write(reinterpret_cast<const char*>(&val), sizeof(T));
}

//pomoshten template za chetene na tipovene int,unsigned,bool,double..
template <typename T>
    void readPrimitive(std::ifstream& in, T& val) {
    in.read(reinterpret_cast<char*>(&val), sizeof(T));
}

}
