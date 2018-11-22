#ifndef PROCESSOR
#define PROCESSOR

#include <string>
#include <file_reader.hpp>
#include <parser_ngspice.hpp>
#include <gui_schematic_qt.hpp>
#include <gui_progress_qt.hpp>
#include <base_thread.hpp>

namespace circuit {

    class Processor : public task::BaseThread<const std::string,
                                              gui::GuiSchematicInterfaceExtSync* const,
                                              gui::GuiProgressInterfaceExtSync* const> {

        typedef circuit::CircuitGraph<gui::GuiSchematicInterfaceExtSync,
                                      gui::GuiProgressInterfaceExtSync> TCircuitGraph;

        void run(const std::string filename,
                 gui::GuiSchematicInterfaceExtSync* const schIfc,
                 gui::GuiProgressInterfaceExtSync* const progressIfc) override {

            try {
                TCircuitGraph circuit(schIfc, progressIfc);

                progressIfc->show();

                parsers::ParserNGSPICE<TCircuitGraph> parser(circuit);
                file_reader::FileReader<parsers::ParserInterface, gui::GuiProgressInterfaceExtSync> freader(&parser, progressIfc);
                freader.readFile(filename);

                circuit.setTerminals( { "vdd", "vss", "gen", "out" } );

                circuit.createRecomendations();
                circuit.print();
                circuit.printRecommendations();


                //temporary progress bar test
                progressIfc->updateLabel("Max = 55 ...");
                progressIfc->updateMax(55);
                for(std::int32_t i = 0; i <= 55; ++i) {
                    progressIfc->updateProgress(i);
                    msSleep(20);
                }
                //

                progressIfc->hide();
            } catch(const std::exception& e) {
                std::cerr << "\033[0;31mException raised:" << std::endl;
                std::cerr << e.what() << "\033[0m" << std::endl;
            } catch(...) {
                std::cerr << "\033[0;31mUnknown exception! Something really bad happened\033[0m"
                    << std::endl;
            }
 
        }

        public:
        virtual ~Processor() = default;

    };

}

#endif
