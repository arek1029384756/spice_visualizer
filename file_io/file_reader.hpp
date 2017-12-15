#ifndef FILE_READER
#define FILE_READER

#include <fstream>
#include <string>
#include <vector>

namespace file_reader {

    template<typename TParser>
    class FileReader {

        TParser* m_parser;

        FileReader() = delete;

        void removeWindowsDelim(std::string& line) {
            try {
                auto last = line.length() - 1;
                if(line.at(last) == 0x0D) {
                    line.erase(last, 1); 
                }
            } catch(const std::out_of_range& e) {
            }
        }

        public:
        FileReader(TParser* const parser)
            : m_parser(parser) {
        }

        void readFile(const std::string& filename) {
            std::ifstream ifs(filename);
            if(ifs.is_open()) {
                std::string line;
                while(std::getline(ifs, line)) {
                    removeWindowsDelim(line);
                    m_parser->parseLine(line);
                }
            } else {
                throw std::runtime_error(std::string("File open error '") + filename + std::string("'"));
            }
        }
    };
}

#endif
