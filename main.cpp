#include <QApplication>
#include <iostream>
#include <file_reader.hpp>
#include <parser_ngspice.hpp>
#include <gui_schematic_qt.hpp>

namespace {
    class App {
        int m_argc;
        char** m_argv;
        QApplication* m_qtApp;
        std::unique_ptr<gui::GuiSchematicQt> m_schematic;

        public:
        App(int argc, char** argv, QApplication* const qtApp)
            : m_argc(argc), m_argv(argv), m_qtApp(qtApp) {}

        virtual ~App() {
        }

        static const std::string& getVersion() {
            static const std::string ver = "1.0";
            return ver;
        }

        int run() {
            try {
                std::setlocale(LC_NUMERIC, "C");

                const std::string filename = (m_argc > 1) ? m_argv[1] : "<empty>";

                parsers::ParserNGSPICE parser;
                file_reader::FileReader<parsers::ParserInterface> freader(&parser);
                freader.readFile(filename);

                auto& circuit = circuit::CircuitGraph::getInstance();
                circuit.setTerminals( { "vdd", "vss", "gen", "out" } );

                circuit.createRecomendations();
                circuit.printiRecommendations();

                m_schematic.reset(new gui::GuiSchematicQt());

                //gui::GuiSchematicInterfaceExt* pSchematicExt = m_schematic.get();
                //gui::GuiSchematicInterfaceExtSync* pSchematicExtSync = m_schematic.get();

                return m_qtApp->exec();
            } catch(const std::exception& e) {
                std::cerr << "\033[0;31mException raised:" << std::endl;
                std::cerr << e.what() << "\033[0m" << std::endl;
                return EXIT_FAILURE;
            } catch(...) {
                std::cerr << "\033[0;31mUnknown exception! Something really bad happened\033[0m"
                    << std::endl;
                return EXIT_FAILURE;
            }
            return 0;
        }
    };
}

int main(int argc, char** argv) {
    QApplication qtApp(argc, argv);
    App app(argc, argv, &qtApp);
    return app.run();
}
