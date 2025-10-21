#ifndef PARKINGLOT_HPP
#define PARKINGLOT_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <iomanip>
#include "Permit.hpp"

static const double PRICE_HOURLY = 4.0;
static const double PRICE_DAILY  = 12.0;
static const std::string SAVE_FILE = "parking_data.txt";

struct ParkingSpace {
    std::string id;
    bool occupied{};
    std::string currentPermitCode;
};

class ParkingLot {
public:
    ParkingLot(int floors, int perFloor) {
        for (int f = 1; f <= floors; ++f) {
            for (int s = 0; s < perFloor; ++s) {
                char letter = 'A' + (s % 26);
                spaces.push_back({std::to_string(f) + letter, false, ""});
            }
        }
        load();
    }

    ~ParkingLot() { save(); }

    std::vector<ParkingSpace> listAvailable();
    std::string assignAny(Permit permit);
    bool releasePermit(const std::string& permitCode);
    std::vector<Permit> activePermits();
    void printStatus();

private:
    std::vector<ParkingSpace> spaces;
    std::map<std::string, Permit> permits;

    void updateExpirations();
    void save();
    void load();
};

// ---------- Implementation ----------

inline std::vector<ParkingSpace> ParkingLot::listAvailable() {
    updateExpirations();
    std::vector<ParkingSpace> available;
    for (auto& s : spaces)
        if (!s.occupied) available.push_back(s);
    return available;
}

inline std::string ParkingLot::assignAny(Permit permit) {
    updateExpirations();
    for (auto& sp : spaces) {
        if (!sp.occupied) {
            sp.occupied = true;
            sp.currentPermitCode = permit.code;
            permit.spaceId = sp.id;
            permits[permit.code] = permit;
            return sp.id;
        }
    }
    return "";
}

inline bool ParkingLot::releasePermit(const std::string& code) {
    std::map<std::string, Permit>::iterator it = permits.find(code);
    if (it == permits.end()) return false;
    it->second.active = false;

    for (size_t i = 0; i < spaces.size(); ++i) {
        if (spaces[i].currentPermitCode == code) {
            spaces[i].occupied = false;
            spaces[i].currentPermitCode.clear();
            break;
        }
    }
    return true;
}

inline std::vector<Permit> ParkingLot::activePermits() {
    updateExpirations();
    std::vector<Permit> result;
    for (std::map<std::string, Permit>::iterator it = permits.begin(); it != permits.end(); ++it)
        if (it->second.active) result.push_back(it->second);
    return result;
}

inline void ParkingLot::printStatus() {
    updateExpirations();
    std::cout << "\nParking Status:\n";
    for (size_t i = 0; i < spaces.size(); ++i) {
        const ParkingSpace& sp = spaces[i];
        std::cout << " " << std::setw(4) << sp.id << " : "
                  << (sp.occupied ? "OCCUPIED" : "FREE");
        if (sp.occupied)
            std::cout << " (Permit: " << sp.currentPermitCode << ")";
        std::cout << "\n";
    }
}

inline void ParkingLot::updateExpirations() {
    TimePoint now = Clock::now();
    for (std::map<std::string, Permit>::iterator it = permits.begin(); it != permits.end(); ++it) {
        Permit& p = it->second;
        if (p.active && p.expires <= now) {
            p.active = false;
            for (size_t i = 0; i < spaces.size(); ++i) {
                if (spaces[i].currentPermitCode == it->first) {
                    spaces[i].occupied = false;
                    spaces[i].currentPermitCode.clear();
                    break;
                }
            }
        }
    }
}

inline void ParkingLot::save() {
    std::ofstream out(SAVE_FILE);
    if (!out) return;
    for (std::map<std::string, Permit>::iterator it = permits.begin(); it != permits.end(); ++it)
        out << it->second.serialize() << "\n";
}

inline void ParkingLot::load() {
    std::ifstream in(SAVE_FILE);
    if (!in) return;
    std::string line;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        Permit p = Permit::deserialize(line);
        permits[p.code] = p;
        if (p.active) {
            for (size_t i = 0; i < spaces.size(); ++i) {
                if (spaces[i].id == p.spaceId) {
                    spaces[i].occupied = true;
                    spaces[i].currentPermitCode = p.code;
                    break;
                }
            }
        }
    }
}

#endif
