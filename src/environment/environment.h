#pragma once

#include <map>
#include <string>
#include <cmath>
#include <stdexcept>

class Environment {
    public:
        Environment() {
            saved["pi"] = 3.1415926535897932385;
            saved["e"] = 2.7182818284590452354;
            saved["phi"] = 1.6180339887498948482;
            saved["tau"] = 6.2831853071795864769;
            saved["x"] = 0;
        }

        void define(const std::string& s, float val) {
            if (s == "pi" || s == "e" || s == "phi" || s == "tau") {
                throw std::runtime_error(s + " is a constant. " + "Cannot redefine constant");
            }
            saved[s] = val;
        }

        float& get(const std::string& s) {
            if (saved.find(s) == saved.end())
                throw s + " not declared";
            return saved[s];
        }

    private:
        std::map<std::string, float> saved;
};
