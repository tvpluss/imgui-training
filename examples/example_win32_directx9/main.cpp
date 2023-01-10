// Dear ImGui: standalone example application for DirectX 9
// If you are new to Dear ImGui, read documentation from the docs/ folder + read the top of imgui.cpp.
// Read online: https://github.com/ocornut/imgui/tree/master/docs

#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include <d3d9.h>
#include <tchar.h>
#include <math.h>
//#include "implot.h"
#include <time.h>
#pragma once
#include <implot.h>
#include "implot_vertical.h"
// Data
static LPDIRECT3D9              g_pD3D = NULL;
static LPDIRECT3DDEVICE9        g_pd3dDevice = NULL;
static D3DPRESENT_PARAMETERS    g_d3dpp = {};

// Forward declarations of helper functions
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void ResetDevice();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
// Main code
float RandomRange(float min, float max) {
    float scale = rand() / (float)RAND_MAX;
    return min + scale * (max - min);
}
void TextCentered(char* text) {
    auto windowWidth = ImGui::GetColumnWidth();
    auto textWidth = ImGui::CalcTextSize(text).x;

    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (windowWidth - textWidth) * 0.5f);
    ImGui::Text(text);
}
void DrawBasicTable(char* label, char* contents[], ImVec4 colors[], int row, int col) {
    if (ImGui::BeginTable(label, col)) {

    for (int i = 0; i < row; i++) {
        ImGui::TableNextRow();
        for (int j = 0; j < col; j++) {
            ImGui::TableSetColumnIndex(j);
                ImGui::PushItemWidth(-FLT_MIN);
                ImGui::PushStyleColor(ImGuiCol_Text, colors[i]);
                TextCentered(contents[i * col + j]);
                ImGui::PopStyleColor();
                ImGui::PopItemWidth();
        }
    }
    ImGui::EndTable();
    }
};

int main(int, char**)
{
    // Create application window
    //ImGui_ImplWin32_EnableDpiAwareness();
    WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, L"ImGui Example", NULL };
    ::RegisterClassExW(&wc);
    HWND hwnd = ::CreateWindowW(wc.lpszClassName, L"Dear ImGui DirectX9 Example", WS_OVERLAPPEDWINDOW, 100, 100, 1280, 800, NULL, NULL, wc.hInstance, NULL);

    // Initialize Direct3D
    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    // Show the window
    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsLight();
    ImGuiStyle* style = &ImGui::GetStyle();
    style->Colors[ImGuiCol_Text] = ImVec4(0.3f, 0.3f, 0.0f, 0.7f);
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX9_Init(g_pd3dDevice);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    ImFont* font_h0 = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 30.0f);
    ImFont* font_h1 = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 24.0f);

    ImFont* font_h2 = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 20.0f);
    ImFont* font_text = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 16.0f);
    IM_ASSERT(font_h0 != NULL);
    IM_ASSERT(font_h1 != NULL);
    IM_ASSERT(font_h2 != NULL);
    IM_ASSERT(font_text != NULL);

    // Our state
    bool show_demo_window = false;
    bool show_another_window = false;
    bool my_tool_active = false;
    bool show_implot_demo_window = false;
    bool my_form = false;
    bool my_table = true;
    const int rand_data_count = 200;
    ImVec4 clear_color = ImVec4(0.25f, 0.35f, 0.00f, 1.00f);

    // Fake datas
    static double x[rand_data_count];
    static double y[rand_data_count];
    for (int j = 0; j < rand_data_count; j++) {
        if (j > 0) {

            x[j] = x[j - 1] + RandomRange(-10, 10);
            y[j] = y[j - 1] + 1;
        }
        else {
            x[j] = RandomRange(70, 80);
            y[j] = 1;
    }
    }
    static double x1[rand_data_count];
    static double y1[rand_data_count];
    for (int j = 0; j < rand_data_count; j++) {
        if (j > 0) {
            x1[j] = x1[j - 1] + RandomRange(-1, 1);
            y1[j] = y1[j - 1] + 1;
        }
        else {
            x1[j] = 50;
            y1[j] = 1;
        }

    }
    static double resist_y[rand_data_count];
    static double resist_resdeep[rand_data_count];
    static double resist_resmed[rand_data_count];
    for (int j = 0; j < rand_data_count; j++) {
        if (j > 0) {
            resist_resdeep[j] = resist_resdeep[j - 1] + RandomRange(-100, 100);
            resist_resmed[j] = resist_resdeep[j] + RandomRange(-100, 100);
            resist_y[j] = resist_y[j - 1] + 1;
        }
        else {
            resist_resdeep[j] = RandomRange(2, 2000);
            resist_resmed[j] = resist_resdeep[j] + RandomRange(-100, 100);
            resist_y[j] = 1;
        }

    }

    static double neutron_y[rand_data_count];
    static double neutron_nphi[rand_data_count];
    static double neutron_rhob[rand_data_count];
    static double neutron_pe[rand_data_count];

    for (int j = 0; j < rand_data_count; j++) {
        if (j > 0) {
            neutron_pe[j] = neutron_pe[j-1] + RandomRange(-1, 1);
            neutron_nphi[j] = neutron_pe[j] + RandomRange(-5, 5);
            neutron_rhob[j] = neutron_pe[j] + RandomRange(1.95, 2.95);
            neutron_y[j] = neutron_y[j - 1] + 1;
        }
        else {
            neutron_pe[j] = 5;
            neutron_nphi[j] = neutron_pe[j] + RandomRange(-5, 5);
            neutron_rhob[j] = neutron_pe[j] + RandomRange(1.95, 2.95);
            neutron_y[j] = 1;
        }
    }


    // Predefined Colors
    ImVec4 color_green = ImVec4(0.0, 0.8, 0.0, 1.0);
    ImVec4 color_red = ImVec4(0.8, 0.0, 0.0, 1.0);
    ImVec4 color_blue = ImVec4(0.0, 0.6, 1.0, 1.0);
    ImVec4 color_black = ImVec4(0.0, 0.0, 0.0, 1.0);
    // Main loop
    bool done = false;
    while (!done)
    {
        // Poll and handle messages (inputs, window resize, etc.)
        // See the WndProc() function below for our to dispatch events to the Win32 backend.
        MSG msg;
        while (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }
        if (done)
            break;

        // Start the Dear ImGui frame
        ImGui_ImplDX9_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);
        if (show_implot_demo_window)
            ImPlot::ShowDemoWindow(&show_implot_demo_window);

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
            ImGui::Checkbox("Implot Demo", &show_implot_demo_window);
            ImGui::Checkbox("Another Window", &show_another_window);
            ImGui::Checkbox("My first Window", &my_tool_active);
            ImGui::Checkbox("Appeal Application Form", &my_form);
            ImGui::Checkbox("Demo Table", &my_table);
            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }
        static double now = (double)time(0);
        if (my_table) {
            ImGui::Begin("My Table", &my_table);
            // TODO: Remove Resizable flags after developments
            static ImGuiTableFlags flags_petropy = ImGuiTableFlags_Resizable | ImGuiTableFlags_ScrollY | ImGuiTableFlags_SizingFixedFit;
            static ImGuiTableColumnFlags column_flags = ImGuiTableColumnFlags_NoHeaderWidth | ImGuiTableColumnFlags_WidthFixed;
            if (ImGui::BeginTable("petropy", 8, flags_petropy)) {

                // Header Row
                //ImGui::TableSetupScrollFreeze(0, 1);
                ImGui::TableSetupColumn("GAMMA RAY", column_flags);
                ImGui::TableSetupColumn("DEPTH", column_flags);
                ImGui::TableSetupColumn("RESISTIVITY", column_flags);
                ImGui::TableSetupColumn("NEUTRON DENSITY", column_flags);
                ImGui::TableSetupColumn("MINERALOGOY", column_flags);
                ImGui::TableSetupColumn("POROSITY SATURATION", column_flags);
                ImGui::TableSetupColumn("OIL IN PLACE ", column_flags);
                ImGui::TableSetupColumn("ELECTROFACIES", column_flags);
                ImGui::TableHeadersRow();
                // Legend Row
                ImGui::TableNextRow();

                // Sub-table GAMMA RAY
                char* gamma_contents[6] = { "0", "GR", "150", "0", "CAL", "16" };
                ImVec4 gama_colors[2] = { color_green, color_red };
                ImGui::TableSetColumnIndex(0);
                DrawBasicTable("gamma-ray", gamma_contents, gama_colors, 2, 3);

                // Sub-table Resist
                char* resist_contents[6] = { "2","RESDEEP", "2000", "2","RESMED", "2000" };
                ImVec4 resist_colors[2] = { color_black , color_red };
                ImGui::TableSetColumnIndex(2);
                DrawBasicTable("resist", resist_contents, resist_colors, 2, 3);

                // Sub-table Neutron
                char* neutron_contents[9] = { "0.45", "NPHI", "-0.15", "1.95", "RHOB", "2.95", "0", "PE", "10" };
                ImVec4 neutron_colors[3] = { color_blue, color_red, color_black };
                ImGui::TableSetColumnIndex(3);
                DrawBasicTable("neutron", neutron_contents, neutron_colors, 3, 3);

                // Sub-table Mineral
                char* mineral_contents[9] = { "VCLAY", "VQTZ", "VDOL", "VCLC", "VPYR", "VOM", "BVH", "BVWF", "BVWI" };
                ImVec4 mineral_colors[1] = { color_green };
                ImGui::TableSetColumnIndex(4);
                DrawBasicTable("mineral", mineral_contents, mineral_colors, 1, 9);

                // Sub-table Porosity
                char* porosity_contents[3] = { "1", "Sw", "0" };
                ImVec4 porosity_colors[1] = { color_green };
                ImGui::TableSetColumnIndex(5);
                DrawBasicTable("porosity", porosity_contents, porosity_colors, 1, 3);

                char* oil_contents[6] = { "0", "OIP", "0.25", "50", "SUM OIP", "0" };
                ImVec4 oil_colors[2] = { color_green, color_red };
                ImGui::TableSetColumnIndex(6);
                DrawBasicTable("oil", oil_contents, oil_colors, 2, 1);

                char* electro_contents[2] = { "0", "1" };
                ImVec4 electro_colors[1] = { color_green };
                ImGui::TableSetColumnIndex(7);
                DrawBasicTable("electro", electro_contents, electro_colors, 1, 2);

                //-----------------------------------------------------
                // GRAPH ROW
                //-----------------------------------------------------

                //Gamma
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImPlot::PushStyleVar(ImPlotStyleVar_PlotDefaultSize, ImVec2(300, 1000));
                ImPlot::PushStyleVar(ImPlotStyleVar_PlotMinSize, ImVec2(300, 1000));
                if (ImPlot::BeginPlot("##Test", ImVec2(0, 0))) {
                    // Set opactity of shade to 25%
                    ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 0.25f);
                    ImPlot::SetupAxes("X", "Y", ImPlotAxisFlags_NoLabel | ImPlotAxisFlags_NoTickLabels | ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_NoDecorations | ImPlotAxisFlags_AutoFit);
                    ImPlot::PlotShaded("##Test", x, y, rand_data_count, 0, ImPlotShadedFlags_Vertical);
                    ImPlot::SetNextLineStyle(color_green);
                    ImPlot::PlotLine("##Test", x, y, rand_data_count);
                    ImPlot::SetNextLineStyle(color_red);
                    ImPlot::PlotLine("##Test1", x1, y1, rand_data_count);
                    ImPlot::PopStyleVar();
                    ImPlot::EndPlot();
                }
                ImPlot::PopStyleVar(2);

                //Depth
                ImGui::TableSetColumnIndex(1);
                int depth_start = 7050;
                char buffer[6];
                for (int i = 0; i < 30; i++) {
                    ImGui::Dummy(ImVec2(0.0f, 20.0f));
                    ImGui::Text(itoa(depth_start, buffer, 10));
                    depth_start += 50;

                }

                //Resist
                ImGui::TableSetColumnIndex(2);
                ImPlot::PushStyleVar(ImPlotStyleVar_PlotDefaultSize, ImVec2(300, 1000));
                ImPlot::PushStyleVar(ImPlotStyleVar_PlotMinSize, ImVec2(300, 1000));
                if (ImPlot::BeginPlot("##Test_Depth", ImVec2(0, 0))) {
                    // Set opactity of shade to 25%
                    ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 0.25f);
                    ImPlot::SetupAxes("X", "Y", ImPlotAxisFlags_NoLabel | ImPlotAxisFlags_NoTickLabels | ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_NoDecorations | ImPlotAxisFlags_AutoFit);
                    ImPlot::PlotShaded("##Resist_resmed", resist_resmed, resist_y, rand_data_count, 1200, ImPlotShadedFlags_Vertical);
                    ImPlot::SetNextLineStyle(color_red);
                    ImPlot::PlotLine("##Resist_resmed", resist_resmed, resist_y, rand_data_count);
                    ImPlot::SetNextLineStyle(color_black);
                    ImPlot::PlotLine("##Resist_resdeep", resist_resdeep, resist_y, rand_data_count);
                    ImPlot::PopStyleVar();
                    ImPlot::EndPlot();
                }
                ImPlot::PopStyleVar(2);

                //Neutron
                ImGui::TableSetColumnIndex(3);
                ImPlot::PushStyleVar(ImPlotStyleVar_PlotDefaultSize, ImVec2(300, 1000));
                ImPlot::PushStyleVar(ImPlotStyleVar_PlotMinSize, ImVec2(300, 1000));
                if (ImPlot::BeginPlot("##Test_Neutron", ImVec2(0, 0))) {
                    ImPlot::SetupAxes("X", "Y", ImPlotAxisFlags_NoLabel | ImPlotAxisFlags_NoTickLabels | ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_NoDecorations | ImPlotAxisFlags_AutoFit);
                    ImPlot::SetNextLineStyle(color_blue);
                    ImPlot::PlotLine("##Neutron_nphi", neutron_nphi, neutron_y, rand_data_count);
                    ImPlot::SetNextLineStyle(color_red);
                    ImPlot::PlotLine("##Neutron_rhob", neutron_rhob, neutron_y, rand_data_count);
                    ImPlot::SetNextLineStyle(color_black);
                    ImPlot::PlotLine("##Neutron_pe", neutron_pe, neutron_y, rand_data_count);
                    ImPlot::EndPlot();
                }
                ImGui::EndTable();
            }

            ImGui::End();
        }
        if (my_form) {
            ImGui::Begin("Appeal Application Form", &my_form);

            ImGui::PushFont(font_h0);
            ImGui::Text("Appeal Application Form");
            ImGui::PopFont(); ImGui::PushFont(font_h1);
            ImGui::Text("Personal Information");

            ImGui::PushFont(font_h2);
            ImGui::Text("Name");

            ImGui::PushFont(font_text);
            if (ImGui::BeginTable("table1", 2)) {
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::PushItemWidth(-1);
                static char buf1[64] = ""; ImGui::InputTextWithHint("##First", "First", buf1, 64);
                ImGui::PopItemWidth();
                ImGui::TableSetColumnIndex(1);
                ImGui::PushItemWidth(-1);
                static char buf2[64] = ""; ImGui::InputTextWithHint("##Last", "Last", buf2, 64);
                ImGui::PopItemWidth();
                ImGui::EndTable();
            }


            ImGui::PopFont();

            ImGui::Text("Student Number");

            ImGui::PushFont(font_text);
            ImGui::PushItemWidth(-1);
            static char buf3[64] = ""; ImGui::InputText("##Number", buf3, 64, ImGuiInputTextFlags_CharsDecimal);
            ImGui::PopItemWidth();
            ImGui::PopFont();

            ImGui::PopFont();

            ImGui::Text("Appeal Information");

            ImGui::PushFont(font_h2);
            ImGui::Text("Request Summary");

            ImGui::PushFont(font_text);
            static char text[1024 * 16] = "";
            ImGui::InputTextMultiline("##source", text, IM_ARRAYSIZE(text), ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 4));
            ImGui::PopFont();

            ImGui::Text("Letter of Appeal Upload");

            ImGui::PushFont(font_text);
            ImGui::PushItemWidth(-1);
            static char upload[64] = "Choose File or Upload"; ImGui::InputText("##Upload", upload, 64);
            ImGui::PopItemWidth();
            ImGui::PopFont();

            ImGui::Text("Do you have supporting documents");
            ImGui::PushFont(font_text);
            static int item_current_2 = 0;
            ImGui::PushItemWidth(-1);
            ImGui::Combo("##combo", &item_current_2, "Yes\0No\0\0");
            ImGui::PopItemWidth();
            ImGui::PopFont();

            ImGui::PopFont();

            ImGui::PopFont();
            ImGui::End();


        }
        if (my_tool_active)
        {
            ImGui::Begin("My First Tool", &my_tool_active, ImGuiWindowFlags_MenuBar);

            // Edit a color stored as 4 floats
            float my_color[4];
            ImGui::ColorEdit4("Color", my_color);

            // Generate samples and plot them
            float samples[100];
            for (int n = 0; n < 100; n++)
                samples[n] = sinf(n * 0.2f + ImGui::GetTime() * 1.5f);
            ImGui::PlotLines("Samples", samples, 100);

            // Display contents in a scrolling region
            ImGui::TextColored(ImVec4(1, 1, 0, 1), "Important Stuff");
            ImGui::BeginChild("Scrolling");
            for (int n = 0; n < 50; n++)
                ImGui::Text("%04d: Some text", n);
            ImGui::EndChild();
            ImGui::End();
        }
        // 3. Show another simple window.
        if (show_another_window)
        {
            ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::Text("Hello from another window!, this text has been changed");
            if (ImGui::Button("Close Me"))
                show_another_window = false;
            ImGui::End();
        }

        // Rendering
        ImGui::EndFrame();
        g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
        g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
        g_pd3dDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
        D3DCOLOR clear_col_dx = D3DCOLOR_RGBA((int)(clear_color.x * clear_color.w * 255.0f), (int)(clear_color.y * clear_color.w * 255.0f), (int)(clear_color.z * clear_color.w * 255.0f), (int)(clear_color.w * 255.0f));
        g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clear_col_dx, 1.0f, 0);
        if (g_pd3dDevice->BeginScene() >= 0)
        {
            ImGui::Render();
            ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
            g_pd3dDevice->EndScene();
        }
        HRESULT result = g_pd3dDevice->Present(NULL, NULL, NULL, NULL);

        // Handle loss of D3D9 device
        if (result == D3DERR_DEVICELOST && g_pd3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
            ResetDevice();
    }

    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);

    return 0;
}

// Helper functions

bool CreateDeviceD3D(HWND hWnd)
{
    if ((g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)
        return false;

    // Create the D3DDevice
    ZeroMemory(&g_d3dpp, sizeof(g_d3dpp));
    g_d3dpp.Windowed = TRUE;
    g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    g_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN; // Need to use an explicit format with alpha if needing per-pixel alpha composition.
    g_d3dpp.EnableAutoDepthStencil = TRUE;
    g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;           // Present with vsync
    //g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;   // Present without vsync, maximum unthrottled framerate
    if (g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &g_d3dpp, &g_pd3dDevice) < 0)
        return false;

    return true;
}

void CleanupDeviceD3D()
{
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
    if (g_pD3D) { g_pD3D->Release(); g_pD3D = NULL; }
}

void ResetDevice()
{
    ImGui_ImplDX9_InvalidateDeviceObjects();
    HRESULT hr = g_pd3dDevice->Reset(&g_d3dpp);
    if (hr == D3DERR_INVALIDCALL)
        IM_ASSERT(0);
    ImGui_ImplDX9_CreateDeviceObjects();
}

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Win32 message handler
// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
        {
            g_d3dpp.BackBufferWidth = LOWORD(lParam);
            g_d3dpp.BackBufferHeight = HIWORD(lParam);
            ResetDevice();
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProc(hWnd, msg, wParam, lParam);
}
