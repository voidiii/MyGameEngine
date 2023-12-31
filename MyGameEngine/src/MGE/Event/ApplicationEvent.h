#pragma once
#include "Event.h"

#include <sstream>

namespace MGE {

	class MGE_API WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(unsigned int width, unsigned int height)
			:m_Width(width), m_Height(height) {}

		inline unsigned int GetWidth() const { return m_Width; }
		inline unsigned int GetHeight() const { return m_Height; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowResizeEvent: " << m_Width << "," << m_Height;
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowResize)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)

	protected:
		unsigned int m_Width, m_Height;
	};

	class MGE_API WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowCloseEvent";
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowClose)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class MGE_API WindowFocusEvent : public Event
	{
	public:
		WindowFocusEvent() {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowFocusEvent";
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowFocus)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class MGE_API WindowLostFocusEvent : public Event
	{
	public:
		WindowLostFocusEvent() {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowLostFocusEvent";
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowLostFocus)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryApplication)
	};

	class MGE_API WindowMovedEvent : public Event
	{
	public:
		WindowMovedEvent(float x, float y) 
			: X(x), Y(y) {}

		inline float GetX() const { return X; }
		inline float GetY() const { return Y; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowMovedEvent: " << X << ", " << Y;
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowMoved)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)

	private:
		float X, Y;
	};

	class MGE_API AppUpdateEvent : public Event
	{
	public:
		AppUpdateEvent() {}

		EVENT_CLASS_TYPE(AppUpdate)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class MGE_API AppRenderEvent : public Event
	{
	public:
		AppRenderEvent() {}

		EVENT_CLASS_TYPE(AppRender)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class MGE_API AppTickEvent : public Event
	{
	public:
		AppTickEvent() {}

		EVENT_CLASS_TYPE(AppTick)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};


}