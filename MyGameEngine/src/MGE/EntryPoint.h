#pragma once

#ifdef MGE_PLATFORM_WINDOWS

extern MGE::Application* MGE::CreateApplication();

int main(int argc, char** argv) {

	MGE::Log::Init();
	MGE_CORE_WARN("Initialized Log!");
	MGE_CLIENT_INFO("Hello!");

	auto app = MGE::CreateApplication();

	app->Run();

	delete app;
}

#endif 