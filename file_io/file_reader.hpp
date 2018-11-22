#ifndef FILE_READER
#define FILE_READER

#include <fstream>
#include <string>
#include <vector>

namespace file_reader {

    template<typename TParser, typename TProgress>
    class FileReader {

        TParser* m_parser;
        TProgress* m_progressIfc;

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

        std::size_t getLinesCount(std::ifstream& ifs) const {
            auto pos = ifs.tellg();
            ifs.seekg(0);
            auto lines = std::count(std::istreambuf_iterator<char>(ifs),
                    std::istreambuf_iterator<char>(), '\n');
            ifs.seekg(pos);
            return lines;
        }

        public:
        FileReader(TParser* const parser, TProgress* const progressIfc)
            : m_parser(parser), m_progressIfc(progressIfc)  {
        }

        virtual ~FileReader() = default;

        void readFile(const std::string& filename) {
            std::ifstream ifs(filename);
            if(ifs.is_open()) {
                m_progressIfc->updateLabel("Reading netlist and creating circuit graph...");
                m_progressIfc->updateMax(getLinesCount(ifs));

                std::size_t lc = 0;
                std::string line;
                while(std::getline(ifs, line)) {
                    removeWindowsDelim(line);
                    m_parser->parseLine(line);
                    m_progressIfc->updateProgress(++lc);
                }
            } else {
                throw std::runtime_error(std::string("File open error '") + filename + std::string("'"));
            }
        }
    };
}

#endif
