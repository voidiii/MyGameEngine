#pragma once
#include "MGEpch.h"
#include "Core.h"
#include "Event/Event.h"

namespace MGE {

	struct WindowProps
	{
		std::string Title;
		unsigned int Height;
		unsigned int Width;

		WindowProps(unsigned int height = 720,
					unsigned int width = 1280,
					const std::string& title = "My Game Engine")
			: Height(height), Width(width), Title(title) {}

	};

	// Interface representing a desktop system based Window, implemeneted based on the system, hence all the "virtual"
	class MGE_API Window
	{
	public:
		using EventCallBackFunction = std::function<void(Event&)>;

		virtual ~Window() {};
		
		virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		virtual void SetEventCallback(const EventCallBackFunction& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		static Window* Create(const WindowProps& props = WindowProps());
	};

}