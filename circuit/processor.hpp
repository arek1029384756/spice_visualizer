#ifndef PROCESSOR
#define PROCESSOR

#include <string>
#include <file_reader.hpp>
#include <parser_ngspice.hpp>
#include <gui_schematic_qt.hpp>
#include <gui_progress_qt.hpp>
#include <base_thread.hpp>

namespace circuit {

    class Processor : public task::BaseThread<const std::string, gui::GuiProgressInterfaceExtSync* const> {

        void run(const std::string filename, gui::GuiProgressInterfaceExtSync* const ifc) override {
            try {
                circuit::CircuitGraph circuit;

                ifc->show();

                parsers::ParserNGSPICE parser(circuit);
                file_reader::FileReader<parsers::ParserInterface, gui::GuiProgressInterfaceExtSync> freader(&parser, ifc);
                freader.readFile(filename);

                circuit.setTerminals( { "vdd", "vss", "gen", "out" } );

                circuit.createRecomendations();
                circuit.printiRecommendations();





                ifc->updateLabel("Max = 55 ...");
                ifc->updateMax(55);
                for(std::int32_t i = 0; i <= 55; ++i) {
                    ifc->updateProgress(i);
                    msSleep(20);
                }

                ifc->hide();
            } catch(const std::exception& e) {
                std::cerr << "\033[0;31mException raised:" << std::endl;
                std::cerr << e.what() << "\033[0m" << std::endl;
            } catch(...) {
                std::cerr << "\033[0;31mUnknown exception! Something really bad happened\033[0m"
                    << std::endl;
            }
 
        }

    };

}

#endif
