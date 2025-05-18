#ifndef DVMEXCEPTION_H
#define DVMEXCEPTION_H

#include <stdexcept>
#include <string>

class DVMNotFoundException : public std::runtime_error {
public:
    explicit DVMNotFoundException(int dvmId) 
        : std::runtime_error("DVM not found with ID: " + std::to_string(dvmId)) {}
};

#endif // DVMEXCEPTION_H
