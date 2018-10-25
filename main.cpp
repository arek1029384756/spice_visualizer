#include <QApplication>
#include <iostream>
#include <file_reader.hpp>
#include <parser_ngspice.hpp>
#include <gui_schematic_qt.hpp>
#include <gui_progress_qt.hpp>

//temporary for test
#include <thread>
#include <chrono>
//

namespace {

    //Temporary test function
    void tmpProgressTest(gui::GuiProgressInterfaceExtSync* const ifc) {
        ifc->show();

        ifc->updateLabel("Max = 100 ...");
        ifc->updateMax(100);
        for(int i = 0; i <= 100; ++i) {
            ifc->updateProgress(i);
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
        }

        ifc->updateLabel("Max = 55 ...");
        ifc->updateMax(55);
        for(int i = 0; i <= 55; ++i) {
            ifc->updateProgress(i);
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
        }

        ifc->hide();
    }

    class App {
        int m_argc;
        char** m_argv;
        QApplication* m_qtApp;
        std::unique_ptr<gui::GuiSchematicQt> m_schematic;
        std::unique_ptr<gui::GuiProgressQt> m_progress;
        std::thread m_th;

        public:
        App(int argc, char** argv, QApplication* const qtApp)
            : m_argc(argc), m_argv(argv), m_qtApp(qtApp) {}

        virtual ~App() {
            if(m_th.joinable()) {
                m_th.join();
            }
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

                m_progress.reset(new gui::GuiProgressQt());
                m_th = std::thread(tmpProgressTest, m_progress.get());

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
