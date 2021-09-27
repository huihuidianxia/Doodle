//
// Created by TD on 2021/9/14.
//

#include "doodle_app.h"

#include <DoodleLib/Gui/main_windwos.h>
#include <DoodleLib/libWarp/boost_locale_warp.h>
#include <DoodleLib/libWarp/imgui_warp.h>
// Helper functions
#include <d3d11.h>
#include <tchar.h>
// Data
static ID3D11Device* g_pd3dDevice                     = nullptr;
static ID3D11DeviceContext* g_pd3dDeviceContext       = nullptr;
static IDXGISwapChain* g_pSwapChain                   = nullptr;
static ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;
// Forward declarations of helper functions
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
bool CreateDeviceD3D(HWND hWnd) {
  // Setup swap chain
  DXGI_SWAP_CHAIN_DESC sd;
  ZeroMemory(&sd, sizeof(sd));
  sd.BufferCount                        = 2;
  sd.BufferDesc.Width                   = 0;
  sd.BufferDesc.Height                  = 0;
  sd.BufferDesc.Format                  = DXGI_FORMAT_R8G8B8A8_UNORM;
  sd.BufferDesc.RefreshRate.Numerator   = 60;
  sd.BufferDesc.RefreshRate.Denominator = 1;
  sd.Flags                              = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
  sd.BufferUsage                        = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  sd.OutputWindow                       = hWnd;
  sd.SampleDesc.Count                   = 1;
  sd.SampleDesc.Quality                 = 0;
  sd.Windowed                           = TRUE;
  sd.SwapEffect                         = DXGI_SWAP_EFFECT_DISCARD;

  UINT createDeviceFlags = 0;
  // createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
  D3D_FEATURE_LEVEL featureLevel;
  const D3D_FEATURE_LEVEL featureLevelArray[2] = {
      D3D_FEATURE_LEVEL_11_0,
      D3D_FEATURE_LEVEL_10_0,
  };
  if (D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext) != S_OK)
    return false;

  CreateRenderTarget();
  return true;
}

void CleanupDeviceD3D() {
  CleanupRenderTarget();
  if (g_pSwapChain) {
    g_pSwapChain->Release();
    g_pSwapChain = nullptr;
  }
  if (g_pd3dDeviceContext) {
    g_pd3dDeviceContext->Release();
    g_pd3dDeviceContext = nullptr;
  }
  if (g_pd3dDevice) {
    g_pd3dDevice->Release();
    g_pd3dDevice = nullptr;
  }
}

void CreateRenderTarget() {
  ID3D11Texture2D* pBackBuffer;
  g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
  g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView);
  pBackBuffer->Release();
}

void CleanupRenderTarget() {
  if (g_mainRenderTargetView) {
    g_mainRenderTargetView->Release();
    g_mainRenderTargetView = nullptr;
  }
}

#ifndef WM_DPICHANGED
#define WM_DPICHANGED 0x02E0  // From Windows SDK 8.1+ headers
#endif

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Win32 message handler
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
  if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
    return true;

  switch (msg) {
    case WM_SIZE:
      if (g_pd3dDevice != nullptr && wParam != SIZE_MINIMIZED) {
        CleanupRenderTarget();
        g_pSwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
        CreateRenderTarget();
      }
      return 0;
    case WM_SYSCOMMAND:
      if ((wParam & 0xfff0) == SC_KEYMENU)  // Disable ALT application menu
        return 0;
      break;
    case WM_DESTROY:
      ::PostQuitMessage(0);
      return 0;
    case WM_DPICHANGED:
      if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DpiEnableScaleViewports) {
        // const int dpi = HIWORD(wParam);
        // printf("WM_DPICHANGED to %d (%.0f%%)\n", dpi, (float)dpi / 96.0f * 100.0f);
        const RECT* suggested_rect = (RECT*)lParam;
        ::SetWindowPos(hWnd, nullptr, suggested_rect->left, suggested_rect->top, suggested_rect->right - suggested_rect->left, suggested_rect->bottom - suggested_rect->top, SWP_NOZORDER | SWP_NOACTIVATE);
      }
      break;
      //    case WM_IME_CHAR: {
      //      auto& io    = ImGui::GetIO();
      //      DWORD wChar = wParam;
      //      if (wChar <= 127) {
      //        io.AddInputCharacter(wChar);
      //      } else {
      //        // swap lower and upper part.
      //        BYTE low  = (BYTE)(wChar & 0x00FF);
      //        BYTE high = (BYTE)((wChar & 0xFF00) >> 8);
      //        wChar     = MAKEWORD(high, low);
      //        wchar_t ch[6];
      //        MultiByteToWideChar(CP_OEMCP, 0, (LPCSTR)&wChar, 4, ch, 3);
      //        io.AddInputCharacter(ch[0]);
      //      }
      //    }
    default:
      break;
  }
  return ::DefWindowProc(hWnd, msg, wParam, lParam);
}
#include <toolkit/toolkit.h>
#include <windows.h>

namespace doodle {

doodle_app* doodle_app::self;

doodle_app::doodle_app()
    : p_hwnd{},
      p_win_class{sizeof(WNDCLASSEX),
                  CS_CLASSDC, WndProc,
                  0L,
                  0L,
                  GetModuleHandle(nullptr),
                  nullptr,
                  nullptr,
                  nullptr,
                  nullptr,
                  _T("ImGui Example"),
                  nullptr},
      p_done(false) {
  // Create application window
  // ImGui_ImplWin32_EnableDpiAwareness();
  ::RegisterClassEx(&p_win_class);
  p_hwnd = ::CreateWindow(p_win_class.lpszClassName,
                          _T("doodle main"),
                          WS_OVERLAPPEDWINDOW,
                          100, 100, 1280, 800,
                          nullptr, nullptr,
                          p_win_class.hInstance,
                          nullptr);

  // Initialize Direct3D
  if (!CreateDeviceD3D(p_hwnd)) {
    CleanupDeviceD3D();
    ::UnregisterClass(p_win_class.lpszClassName, p_win_class.hInstance);
  }

  // Show the window
  ::ShowWindow(p_hwnd, SW_SHOWDEFAULT);
  ::UpdateWindow(p_hwnd);

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();

  (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
  // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;    // Enable Docking
  io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;  // Enable Multi-Viewport / Platform Windows
  // io.ConfigViewportsNoAutoMerge = true;
  // io.ConfigViewportsNoTaskBarIcon = true;
  // io.ConfigViewportsNoDefaultParent = true;
  // io.ConfigDockingAlwaysTabBar = true;
  // io.ConfigDockingTransparentPayload = true;
  // io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleFonts;     // FIXME-DPI: Experimental. THIS CURRENTLY DOESN'T WORK AS EXPECTED. DON'T USE IN USER APP!
  // io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleViewports; // FIXME-DPI: Experimental.

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();
  // ImGui::StyleColorsClassic();

  // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
  ImGuiStyle& style = ImGui::GetStyle();
  if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
    style.WindowRounding              = 0.0f;
    style.Colors[ImGuiCol_WindowBg].w = 1.0f;
  }

  // Setup Platform/Renderer backends
  ImGui_ImplWin32_Init(p_hwnd);
  ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);
}
doodle_app::~doodle_app() {
  p_hwnd = static_cast<HWND>(p_hwnd);
  // Cleanup
  ImGui_ImplDX11_Shutdown();
  ImGui_ImplWin32_Shutdown();
  ImGui::DestroyContext();

  CleanupDeviceD3D();
  ::DestroyWindow(p_hwnd);
  ::UnregisterClass(p_win_class.lpszClassName, p_win_class.hInstance);
}
std::int32_t doodle_app::run() {
  // Load Fonts
  // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
  // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
  // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
  // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
  // - Read 'docs/FONTS.md' for more instructions and details.
  // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
  // io.Fonts->AddFontDefault();
  // io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
  // io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
  // io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
  // io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
  // ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
  // IM_ASSERT(font != NULL);
  ImGuiIO& io = ImGui::GetIO();
  io.Fonts->AddFontFromFileTTF(R"(C:\Windows\Fonts\simkai.ttf)", 16.0f, nullptr, io.Fonts->GetGlyphRangesChineseFull());
  io.Fonts->AddFontFromFileTTF(R"(C:\Windows\Fonts\simhei.ttf)", 16.0f, nullptr, io.Fonts->GetGlyphRangesChineseFull());

  main_windows k_main_windows{};
  // Our state
  bool show_demo_window    = true;
  bool show_another_window = false;
  ImVec4 clear_color       = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
  // Main loop
  while (!p_done) {
    // Poll and handle messages (inputs, window resize, etc.)
    // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
    // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
    // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
    // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
    MSG msg;
    while (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) {
      ::TranslateMessage(&msg);
      ::DispatchMessage(&msg);
      if (msg.message == WM_QUIT)
        p_done = true;
    }
    if (p_done)
      break;

    // Start the Dear ImGui frame
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    imgui::DockSpaceOverViewport(imgui::GetMainViewport());
    static bool show_info = false;
    static std::string str{};
    try {
      k_main_windows.frame_render();
      main_loop();
    } catch (doodle_error& err) {
      show_info = true;
      str       = err.what();
      imgui::OpenPopup("警告");
    }
    //    catch (std::runtime_error& err) {
    //      show_info = true;
    //      std::wstring str2{err.what(), err.what() + std::strlen(err.what())};
    //      str = conv::locale_to_utf<char>(str2);
    //
    //      imgui::OpenPopup("警告");
    //    }
    dear::PopupModal{"警告", &show_info} && [str1 = str]() {
      dear::Text(str);
      if (ImGui::Button("OK")) {
        ImGui::CloseCurrentPopup();
      }
    };

    // Rendering
    ImGui::Render();
    const float clear_color_with_alpha[4] = {clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w};
    g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
    g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    // Update and Render additional Platform Windows
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
      ImGui::UpdatePlatformWindows();
      ImGui::RenderPlatformWindowsDefault();
    }

    g_pSwapChain->Present(1, 0);  // Present with vsync
                                  // g_pSwapChain->Present(0, 0); // Present without vsync
  }
  return 0;
}
std::unique_ptr<doodle_app> doodle_app::make_this() {
  auto k_          = std::unique_ptr<doodle_app>(new doodle_app{});
  doodle_app::self = k_.get();
  return k_;
}
doodle_app* doodle_app::Get() {
  return doodle_app::self;
}
}  // namespace doodle
