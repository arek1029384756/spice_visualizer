#ifndef GUI_PROGRESS_QT
#define GUI_PROGRESS_QT

#include <iostream>
#include <memory>
#include <mutex>
#include <progress.h>
#include <gui_progress_interface.hpp>

namespace gui {

    class GuiProgressQt : public GuiProgressInterfaceExtSync,
                          public GuiProgressInterfaceExt,
                          public GuiProgressInterfaceInt {
        std::unique_ptr<gui_qt::Progress> m_progress;

        std::mutex m_mtx;
        std::int32_t m_sharedValue;
        std::int32_t m_sharedMax;
        std::string m_sharedLabel;

        //use returned reference only when 'dst' is a local (not shared) object
        template<typename T>
        T& safeCopy(T& dst, const T& src) {
            std::unique_lock<std::mutex> lck(m_mtx);
            dst = src;
            return dst;
        }

        void notifyProgress() const {
            emit m_progress->signalNewData();
        }

        public:
        GuiProgressQt()
            : m_progress(new gui_qt::Progress(this)) {
        }

        virtual ~GuiProgressQt() {
        }


        //External synchronized interface
        virtual void updateProgress(std::int32_t value) override {
            safeCopy(m_sharedValue, value);
            notifyProgress();
        }

        virtual void updateMax(std::int32_t max) override {
            safeCopy(m_sharedMax, max);
            notifyProgress();
        }

        virtual void updateLabel(const std::string& label) override {
            safeCopy(m_sharedLabel, label);
            notifyProgress();
        }

        virtual void show() const override {
            emit m_progress->signalShow();
        }

        virtual void hide() const override {
            emit m_progress->signalHide();
        }


        //Internal interface
        virtual std::int32_t getValue() override {
            std::int32_t val;
            return safeCopy(val, m_sharedValue);
        }

        virtual std::int32_t getMax() override {
            std::int32_t max;
            return safeCopy(max, m_sharedMax);
        }

        virtual std::string getLabel() override {
            std::string lab;
            return safeCopy(lab, m_sharedLabel);
        }

    };
}

#endif
