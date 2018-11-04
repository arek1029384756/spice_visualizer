#include <QApplication>
#include <iostream>
#include <gui_schematic_qt.hpp>
#include <gui_progress_qt.hpp>
#include <processor.hpp>

namespace {

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

                m_schematic.reset(new gui::GuiSchematicQt());
                //gui::GuiSchematicInterfaceExt* pSchematicExt = m_schematic.get();
                //gui::GuiSchematicInterfaceExtSync* pSchematicExtSync = m_schematic.get();

                m_progress.reset(new gui::GuiProgressQt());

                circuit::Processor processor;
                task::BaseThread<const std::string, gui::GuiProgressInterfaceExtSync* const> * const procThread = &processor;
                procThread->start(filename, m_progress.get());

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
