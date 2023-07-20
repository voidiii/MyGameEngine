#pragma once

#ifdef MGE_PLATFORM_WINDOWS

extern MGE::Application* MGE::CreateApplication();

int main(int argc, char** argv) {

	MGE::Log::Init();
	MGE_CORE_WARN("Initialized Log!");
	MGE_CLIENT_INFO("Hello!");

	MGE_PROFILE_BEGIN_SESSION("Startup", "MGEProfile-Startup.json");
	auto app = MGE::CreateApplication();
	MGE_PROFILE_END_SESSION();

	MGE_PROFILE_BEGIN_SESSION("Runtime", "MGEProfile-Runtime.json");
	app->Run();
	MGE_PROFILE_END_SESSION();

	MGE_PROFILE_BEGIN_SESSION("Shutdown", "MGEProfile-Shutdown.json");
	delete app;
	MGE_PROFILE_END_SESSION();
}

#endif 