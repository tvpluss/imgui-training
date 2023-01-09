#pragma once
#include "implot_vertical.h"

template <typename _Getter>
void PlotLineEx(const char* label_id, const _Getter& getter, ImPlotLineVFlags flags) {
    if (BeginItemEx(label_id, Fitter1<_Getter>(getter), flags, ImPlotCol_Line)) {
        const ImPlotNextItemData& s = GetItemData();
        if (getter.Count > 1) {
            if (ImHasFlag(flags, ImPlotLineFlags_Shaded) && s.RenderFill) {
                const ImU32 col_fill = ImGui::GetColorU32(s.Colors[ImPlotCol_Fill]);
                GetterOverrideY<_Getter> getter2(getter, 0);
                RenderPrimitives2<RendererShaded>(getter, getter2, col_fill);
            }
            if (s.RenderLine) {
                const ImU32 col_line = ImGui::GetColorU32(s.Colors[ImPlotCol_Line]);
                if (ImHasFlag(flags, ImPlotLineFlags_Segments)) {
                    RenderPrimitives1<RendererLineSegments1>(getter, col_line, s.LineWeight);
                }
                else if (ImHasFlag(flags, ImPlotLineFlags_Loop)) {
                    if (ImHasFlag(flags, ImPlotLineFlags_SkipNaN))
                        RenderPrimitives1<RendererLineStripSkip>(GetterLoop<_Getter>(getter), col_line, s.LineWeight);
                    else
                        RenderPrimitives1<RendererLineStrip>(GetterLoop<_Getter>(getter), col_line, s.LineWeight);
                }
                else {
                    if (ImHasFlag(flags, ImPlotLineFlags_SkipNaN))
                        RenderPrimitives1<RendererLineStripSkip>(getter, col_line, s.LineWeight);
                    else
                        RenderPrimitives1<RendererLineStrip>(getter, col_line, s.LineWeight);
                }
            }
        }
        // render markers
        if (s.Marker != ImPlotMarker_None) {
            if (ImHasFlag(flags, ImPlotLineFlags_NoClip)) {
                PopPlotClipRect();
                PushPlotClipRect(s.MarkerSize);
            }
            const ImU32 col_line = ImGui::GetColorU32(s.Colors[ImPlotCol_MarkerOutline]);
            const ImU32 col_fill = ImGui::GetColorU32(s.Colors[ImPlotCol_MarkerFill]);
            RenderMarkers<_Getter>(getter, s.Marker, s.MarkerSize, s.RenderMarkerFill, col_fill, s.RenderMarkerLine, col_line, s.MarkerWeight);
        }
        EndItem();
    }
}

namespace ImPlotVertical {

    template<typename T>
    void PlotLineV(const char* label_id, const T* values, int count, double xscale, double xstart, ImPlotLineVFlags flags, int offset, int stride)
    {
        GetterXY<IndexerIdx, IndexerLin<T>> getter(IndexerIdx<T>(values, count, offset, stride), IndexerLin(xscale, x0), count);
        PlotLineEx(label_id, getter, flags);

    }

    template<typename T>
    void PlotLineV(const char* label_id, const T* xs, const T* ys, int count, ImPlotLineVFlags flags, int offset, int stride)
    {
        GetterXY<IndexerIdx<T>, IndexerIdx<T>> getter(IndexerIdx<T>(ys, count, offset, stride), IndexerIdx<T>(xs, count, offset, stride), count);
        PlotLineEx(label_id, getter, flags);
    }

    template<typename T>
     void PlotShadedV(const char* label_id, const T* values, int count, double yref, double xscale, double xstart, ImPlotShadedVFlags flags, int offset, int stride)
    {
        return IMPLOT_VERTICAL_TMP void();
    }
    template<typename T>
    void PlotShadedV(const char* label_id, const T* xs, const T* ys, int count, double yref, ImPlotShadedVFlags flags, int offset, int stride)
    {
        return IMPLOT_VERTICAL_TMP void();
    }
    template<typename T>
    void PlotShadedV(const char* label_id, const T* xs, const T* ys1, const T* ys2, int count, ImPlotShadedVFlags flags, int offset, int stride)
    {
        return IMPLOT_VERTICAL_TMP void();
    }

}
