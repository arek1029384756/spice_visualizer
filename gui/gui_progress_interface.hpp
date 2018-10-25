#ifndef GUI_PROGRESS_INTERFACE
#define GUI_PROGRESS_INTERFACE

#include <string>

namespace gui {

    class GuiProgressInterfaceExtSync {
        public:
        virtual void updateProgress(std::int32_t value) = 0;
        virtual void updateMax(std::int32_t max) = 0;
        virtual void updateLabel(const std::string& label) = 0;
        virtual void show() const = 0;
        virtual void hide() const = 0;
    };

    class GuiProgressInterfaceExt {
        public:
    };

    class GuiProgressInterfaceInt {
        public:
        virtual std::int32_t getValue() = 0;
        virtual std::int32_t getMax() = 0;
        virtual std::string getLabel() = 0;
    };

}

#endif
