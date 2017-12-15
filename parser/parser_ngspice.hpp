#ifndef PARSER_NGSPICE
#define PARSER_NGSPICE

#include <iostream>
#include <fstream>
#include <algorithm>
#include <regex>
#include "parser_interface.hpp"

namespace parsers {

    class ParserNGSPICE : public ParserInterface {

        void findTokens(std::string str, std::vector<std::string>& tokens, const std::string& regex) {
            std::regex r(regex.c_str());
            std::smatch sm;
            while(regex_search(str, sm, r)) {
                if(sm.size() > 0) { 
                    std::string token = sm[0];
                    token.erase(std::remove(token.begin(), token.end(), ' '), token.end());
                    tokens.emplace_back(token);
                }
                str = sm.suffix();
            }
        }

        public:
        ParserNGSPICE() {
        }

        virtual void parseLine(const std::string& line) override {
            try {
                std::vector<std::string> tokens;
                std::cout << line << std::endl;
                findTokens(line, tokens, "\\s*\\S+");
                for(const auto& tkn : tokens) {
                    std::cout << "Token:" << tkn << std::endl;
                }
                std::cout << std::endl;
            } catch(const std::out_of_range& e) {
                throw std::runtime_error(std::string("Line processing error! '") +
                        e.what() + std::string("'"));
            }
        }

    };
}

#endif
