#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include <chrono>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <random>
#include <string>

using Clock = std::chrono::system_clock;
using TimePoint = std::chrono::time_point<Clock>;

// Convert time point to string
inline std::string timePointToString(const TimePoint& tp) 
{
    std::time_t t = Clock::to_time_t(tp);
    std::tm* tmPtr = std::localtime(&t);
    char buf[64];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", tmPtr);
    return std::string(buf);
}

// Convert string to time point
inline TimePoint stringToTimePoint(const std::string& str) 
{
    std::tm tm{};
    std::istringstream ss(str);
    ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
    std::time_t t = std::mktime(&tm);
    return Clock::from_time_t(t);
}

// Generate random alphanumeric code
inline std::string generateCode(int length = 6) 
{
    static std::random_device rd;
    static std::mt19937_64 gen(rd());
    static const char chars[] = "ABCDEFGHJKMNPQRSTUVWXYZ23456789";
    std::uniform_int_distribution<> dis(0, sizeof(chars) - 2);
    std::string code;
    for (int i = 0; i < length; ++i)
        code.push_back(chars[dis(gen)]);
    return code;
}

#endif
