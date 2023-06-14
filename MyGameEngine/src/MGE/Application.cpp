#include "MGEpch.h"
#include "Application.h"
#include "./Event/ApplicatonEvent.h"
#include "./Log.h"

namespace MGE {

	Application::Application() {
	
	}

	Application::~Application() {

	}

	void Application::Run() {

		WindowResizeEvent test(1280, 720);
		if (test.IsInCategory(EventCategoryApplication)) {
			MGE_CLIENT_TRACE(test);
		}

		if (test.IsInCategory(EventCategoryInput)) {
			MGE_CLIENT_TRACE(test);
		}

		while (true);
	}

}