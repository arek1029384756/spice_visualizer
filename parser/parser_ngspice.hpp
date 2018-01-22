#ifndef PARSER_NGSPICE
#define PARSER_NGSPICE

#include <iostream>
#include <fstream>
#include <algorithm>
#include <regex>
#include <parser_interface.hpp>
#include <data_format.hpp>

namespace parsers {

    class ParserNGSPICE : public ParserInterface {
        bool m_ignore;

        void findTokens(std::string str, std::vector<std::string>& tokens, const std::string& regex) const {
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

        void printTokens(const std::vector<std::string>& tokens) const {
            for(const auto& tkn : tokens) {
                std::cout << "Token:" << tkn << std::endl;
            }
        }

        void parseTokens(const std::vector<std::string>& tokens) {
            try {
                const auto& tkn = tokens.at(0);
                const auto tknFirst = tkn.at(0);
                if(tknFirst == '*') {
                    //comment
                } else if(tkn == ".control") {
                    m_ignore = true;
                } else if(tkn == ".endc") {
                    m_ignore = false;
                } else if(tknFirst == '.') {
                    //control
                } else if(tknFirst == 'V' || tknFirst == 'v') {
                    //voltage source
                } else if(!m_ignore) {
                    auto& circuit = graph::CircuitGraph::getInstance();

                    std::list<std::string> connections;
                    for(std::size_t i = 1; i < tokens.size() - 1; ++i) {
                        connections.emplace_back(tokens.at(i));
                    }

                    circuit.addComponent(std::string(1, tknFirst), tkn, tokens.at(tokens.size() - 1), connections);
                }
            } catch(const std::out_of_range& e) {
            }
        }

        public:
        ParserNGSPICE()
            : m_ignore(false) {
        }

        virtual void parseLine(const std::string& line) override {
            try {
                std::cout << "Line:" << line << std::endl;

                std::vector<std::string> tokens;
                findTokens(line, tokens, "\\s*\\S+");
                parseTokens(tokens);
            } catch(const std::out_of_range& e) {
                throw std::runtime_error(std::string("Line processing error! '") +
                        e.what() + std::string("'"));
            }
        }

    };
}

#endif
