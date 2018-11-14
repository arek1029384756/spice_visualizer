#ifndef GUI_SCHEMATIC_QT
#define GUI_SCHEMATIC_QT

#include <iostream>
#include <memory>
#include <mutex>
#include <schematic.h>
#include <gui_schematic_interface.hpp>

namespace gui {

    class GuiSchematicQt : public GuiSchematicInterfaceExtSync,
                           public GuiSchematicInterfaceExt,
                           public GuiSchematicInterfaceInt {
        std::unique_ptr<gui_qt::Schematic> m_schematic;

        public:
        GuiSchematicQt()
            : m_schematic(new gui_qt::Schematic(this)) {
        }

        virtual ~GuiSchematicQt() = default;

        //External synchronized interface


        //External interface


        //Internal interface

    };
}

#endif
