#ifndef PERMIT_HPP
#define PERMIT_HPP

#include <string>
#include <sstream>
#include <iomanip>
#include "Utilities.hpp"

struct Permit 
{
    std::string code;
    std::string ownerName;
    std::string spaceId;
    TimePoint start;
    TimePoint expires;
    double amountPaid{};
    bool active{};

    std::string serialize() const 
    {
        std::ostringstream oss;
        oss << code << "|" << ownerName << "|" << spaceId << "|"
            << timePointToString(start) << "|" << timePointToString(expires) << "|"
            << amountPaid << "|" << active;
        return oss.str();
    }

    static Permit deserialize(const std::string& line) 
    {
        Permit p;
        std::istringstream iss(line);
        std::string startStr, expStr;
        std::getline(iss, p.code, '|');
        std::getline(iss, p.ownerName, '|');
        std::getline(iss, p.spaceId, '|');
        std::getline(iss, startStr, '|');
        std::getline(iss, expStr, '|');
        iss >> p.amountPaid;
        iss.ignore(1, '|');
        iss >> p.active;
        p.start = stringToTimePoint(startStr);
        p.expires = stringToTimePoint(expStr);
        return p;
    }
};

#endif
