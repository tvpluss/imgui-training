#pragma once
#include "imgui.h"
#ifndef IMPLOT_VERTICAL_API
#define IMPLOT_VERTICAL_API
#endif

#define IMPLOT_VERTICAL_VERSION "0.1"
#define IMPLOT_VERTICAL_AUTO -1
#define IMPLOT_VERTICAL_AUTO_COL ImVec4(0,0,0,-1)
#define IMPLOT_VERTICAL_TMP template <typename T> IMPLOT_VERTICAL_API

typedef int ImPlotLineVFlags;
typedef int ImPlotShadedVFlags;


namespace ImPlotVertical {

//
// [SECTION] Plot Lines
//
IMPLOT_VERTICAL_TMP void PlotLineV(const char* label_id, const T* values, int count, double xscale=1, double xstart=0, ImPlotLineVFlags flags=0, int offset=0, int stride=sizeof(T));
IMPLOT_VERTICAL_TMP void PlotLineV(const char* label_id, const T* xs, const T* ys, int count, ImPlotLineVFlags flags=0, int offset=0, int stride=sizeof(T));
// IMPLOT_VERTICAL_API void PlotLineVG(const char* label_id, ImPlotGetter getter, void* data, int count, ImPlotLineVFlags flags=0); 

//
//  [SECTION] Plot Shade
//
IMPLOT_VERTICAL_TMP void PlotShadedV(const char* label_id, const T* values, int count, double yref=0, double xscale=1, double xstart=0, ImPlotShadedVFlags flags=0, int offset=0, int stride=sizeof(T));
IMPLOT_VERTICAL_TMP void PlotShadedV(const char* label_id, const T* xs, const T* ys, int count, double yref=0, ImPlotShadedVFlags flags=0, int offset=0, int stride=sizeof(T));
IMPLOT_VERTICAL_TMP void PlotShadedV(const char* label_id, const T* xs, const T* ys1, const T* ys2, int count, ImPlotShadedVFlags flags=0, int offset=0, int stride=sizeof(T));
// IMPLOT_VERTICAL_API void PlotShadedG(const char* label_id, ImPlotGetter getter1, void* data1, ImPlotGetter getter2, void* data2, int count, ImPlotShadedVFlags flags=0);

}
