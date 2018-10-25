#include <QtWidgets>
#include <iostream>
#include "progress.h"

namespace gui_qt {

    Progress::Progress(gui::GuiProgressInterfaceInt* ifc, QDialog* parent)
        : QDialog(parent), m_ifc(ifc) {

        setWindowTitle(tr("Processing progress"));

        m_layout.reset(new QFormLayout());
        m_progressBar.reset(new QProgressBar());
        m_label.reset(new QLabel());

        m_progressBar->setMinimumWidth(400);
        m_label->setText("");

        m_layout->addRow(m_label.get());
        m_layout->addRow(m_progressBar.get());

        m_layout->setSizeConstraint(QLayout::SetFixedSize);

        setLayout(m_layout.get());

        m_progressBar->setMinimum(0);
        m_progressBar->setMaximum(0);
        m_progressBar->setValue(0);

        connect(this, SIGNAL(signalNewData()), this, SLOT(slotNewData()));
        connect(this, SIGNAL(signalShow()), this, SLOT(slotShow()));
        connect(this, SIGNAL(signalHide()), this, SLOT(slotHide()));
    }

    void Progress::slotNewData() {
        auto label = m_ifc->getLabel();
        m_label->setText(label.c_str());

        auto val = m_ifc->getMax();
        m_progressBar->setMaximum(val);

        val = m_ifc->getValue();
        m_progressBar->setValue(val);
    }

    void Progress::slotShow() {
        show();
    }

    void Progress::slotHide() {
        hide();
    }

}
