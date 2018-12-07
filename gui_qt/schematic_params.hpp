#ifndef SCHEMATIC_PARAMS
#define SCHEMATIC_PARAMS

namespace gui_qt {

    constexpr qreal g_gridRaster = 10;

    constexpr qreal g_logSchWidth = 100;
    constexpr qreal g_logSchHeight = g_logSchWidth / 2;

    template<typename T>
    constexpr T L2P(T log) {
        return log * g_gridRaster;
    }

    template<typename T>
    constexpr T P2L(T phys) {
        return phys / g_gridRaster;
    }

    constexpr qreal g_scaleExp = 0.2;
}

#endif
