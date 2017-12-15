#ifndef PARSER_INTERFACE
#define PARSER_INTERFACE

namespace parsers {

    class ParserInterface {
        public:
        //virtual const mw::DataVector& getData() const = 0;
        virtual void parseLine(const std::string& line) = 0;
    };
}

#endif
