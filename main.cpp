#include <QApplication>
#include <iostream>
#include <file_reader.hpp>
#include <parser_ngspice.hpp>
#include <gui_schematic_qt.hpp>
#include <gui_progress_qt.hpp>
#include <base_thread.hpp>

namespace {

    class TmpProgressTest : public task::BaseThread<gui::GuiProgressInterfaceExtSync* const> {

        void run(gui::GuiProgressInterfaceExtSync* const ifc) override {
            ifc->show();

            ifc->updateLabel("Max = 100 ...");
            ifc->updateMax(100);
            for(std::int32_t i = 0; i <= 100; ++i) {
                ifc->updateProgress(i);
                msSleep(20);
            }

            ifc->updateLabel("Max = 55 ...");
            ifc->updateMax(55);
            for(std::int32_t i = 0; i <= 55; ++i) {
                ifc->updateProgress(i);
                msSleep(20);
            }

            ifc->hide();
        }

    };

    class App {
        int m_argc;
        char** m_argv;
        QApplication* m_qtApp;
        std::unique_ptr<gui::GuiSchematicQt> m_schematic;
        std::unique_ptr<gui::GuiProgressQt> m_progress;

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

                m_progress.reset(new gui::GuiProgressQt());

                //temporary progress test
                TmpProgressTest progressTest;
                task::BaseThread<gui::GuiProgressInterfaceExtSync* const> * const pTask = &progressTest;
                pTask->start(m_progress.get());
                //

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
