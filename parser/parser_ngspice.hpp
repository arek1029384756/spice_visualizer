#ifndef PARSER_NGSPICE
#define PARSER_NGSPICE

#include <iostream>
#include <fstream>
#include <algorithm>
#include <regex>
#include <parser_interface.hpp>
#include <circuit.hpp>

namespace parsers {

    template<typename TCircuit>
    class ParserNGSPICE : public ParserInterface {
        enum class IgnoreStatus {
            None,
            Line,
            SectionBegin,
            SectionEnd
        };

        static const std::list<std::pair<std::string, std::string>> m_ignoreSections;
        static const std::list<char> m_ignoreLines;

        bool m_ignore;
        TCircuit& m_circuit;

        IgnoreStatus checkIgnore(std::string token) const {
            std::transform(token.begin(), token.end(), token.begin(), ::tolower);

            auto itS = std::find_if(m_ignoreSections.begin(), m_ignoreSections.end(), [&](const auto& pair) {
                    return (pair.first == token || pair.second == token) ? true : false;
                    });

            if(itS != m_ignoreSections.end()) {
                if(itS->first == token) {
                    return IgnoreStatus::SectionBegin;
                } else if(itS->second == token) {
                    return IgnoreStatus::SectionEnd;
                }
            }

            auto itL = std::find_if(m_ignoreLines.begin(), m_ignoreLines.end(), [&](const auto& c){
                    return (c == token.at(0)) ? true : false;
                    });

            if(itL != m_ignoreLines.end()) {
                return IgnoreStatus::Line;
            }

            return IgnoreStatus::None;
        }

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
                auto ignoreStat = checkIgnore(tkn);

                if(ignoreStat == IgnoreStatus::SectionBegin) {
                    m_ignore = true;
                } else if(ignoreStat == IgnoreStatus::SectionEnd) {
                    m_ignore = false;
                } else if(!m_ignore && ignoreStat == IgnoreStatus::None) {
                    std::list<std::string> connections;
                    for(std::size_t i = 1; i < tokens.size() - 1; ++i) {
                        connections.emplace_back(tokens.at(i));
                    }

                    m_circuit.addComponent(std::string(1, tkn.at(0)), tkn, tokens.at(tokens.size() - 1), connections);
                }
            } catch(const std::out_of_range& e) {
            }
        }

        public:
        ParserNGSPICE(TCircuit& circuit)
            : m_ignore(false), m_circuit(circuit) {
        }

        virtual ~ParserNGSPICE() = default;

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

    template<typename TCircuit>
    const std::list<std::pair<std::string, std::string>> ParserNGSPICE<TCircuit>::m_ignoreSections = { {".control", ".endc"},
                                                                                                       {".subckt", ".ends"} };

    template<typename TCircuit>
    const std::list<char> ParserNGSPICE<TCircuit>::m_ignoreLines = { '*', '.', 'v', 'i' };
}

#endif
