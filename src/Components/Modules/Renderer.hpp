#pragma once

namespace Components
{
	class Renderer : public Component
	{
	public:
		typedef void(BackendCallback)(IDirect3DDevice9*);
		typedef void(Callback)();

		static int Width();
		static int Height();

		Renderer();
		~Renderer();
	private:
		static void PostPreVidRestart();
		static void PreVidRestart();
		static void PostVidRestart();
		static void PostVidRestartStub();

		static Utils::Signal<Renderer::Callback> EndRecoverDeviceSignal;
		static Utils::Signal<Renderer::Callback> BeginRecoverDeviceSignal;

		static Utils::Signal<BackendCallback> BackendFrameSignal;
		static Utils::Signal<BackendCallback> SingleBackendFrameSignal;
	};
}