#ifndef PROGRESS
#define PROGRESS

#include <QDialog>
#include <QFormLayout>
#include <QProgressBar>
#include <QLabel>
#include <memory>
#include <gui_progress_interface.hpp>

namespace gui_qt {

    class Progress : public QDialog {
        Q_OBJECT

        gui::GuiProgressInterfaceInt* m_ifc;

        std::shared_ptr<QFormLayout> m_layout;

        std::shared_ptr<QProgressBar> m_progressBar;

        std::shared_ptr<QLabel> m_label;

        public slots:
        void slotNewData();
        void slotShow();
        void slotHide();

        signals:
        void signalNewData();
        void signalShow();
        void signalHide();

        public:
        Progress(gui::GuiProgressInterfaceInt* ifc, QDialog* parent = 0);
        virtual ~Progress() = default;
    };
}

#endif
