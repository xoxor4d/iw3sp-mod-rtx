#pragma once

namespace Components
{
	class Window : public Component
	{
	public:
		typedef BOOL(WndProcCallback)(WPARAM wParam, LPARAM lParam);
		typedef void(CreateCallback)();

		Window();
		~Window();

		static int Width();
		static int Height();
		static int Width(HWND window);
		static int Height(HWND window);
		static void Dimension(RECT* rect);
		static void Dimension(HWND window, RECT* rect);

		static bool IsCursorWithin(HWND window);

		static HWND GetWindow();

		static void OnWndMessage(UINT Msg, Utils::Slot<WndProcCallback> callback);

		static void OnCreate(Utils::Slot<CreateCallback> callback);
	private:
		static BOOL CursorVisible;
		static std::unordered_map<UINT, Utils::Slot<WndProcCallback>> WndMessageCallbacks;
		static Utils::Signal<CreateCallback> CreateSignals;

		static HWND MainWindow;

		static void ApplyCursor();

		static bool IsNoBorder();

		static BOOL WINAPI MessageHandler(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

		static int WINAPI ShowCursorHook(BOOL show);
		static void DrawCursorStub();
		static void DrawCursor(const float* color, Game::ScreenPlacement* ScrPlace, float x, float y, float w, float h, int horzAlign, int vertAlign, Game::Material* material);

		static void StyleHookStub();
		static HWND WINAPI CreateMainWindow(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam);

		static void EnableDpiAwareness();
		static void ExternalConsoleStub();

		static void ScrPlace_CalcSafeAreaOffsets(float* realViewableMin, float* realViewableMax, float viewportX, float viewportY, float viewportWidth, float viewportHeight, float aspect, float safeAreaRatioHorz, float safeAreaRatioVert, float* virtualViewableMin, float* virtualViewableMax, int width, int height);
		static void ScrPlace_SetupViewport(Game::ScreenPlacement* scrPlace, float viewportX, float viewportY, float viewportWidth, float viewportHeight);

	};
}