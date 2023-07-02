#pragma once
#include "MGE.h"
#include "MathAlias.h"

class ExampleLayer : public MGE::Layer 
{
public:
	ExampleLayer()
		: Layer("Example") {
			MGE::Vec2 a = { 1, 2 };
			MGE::Vec3 v1 = { 1, 2, 3 };
			MGE::Vec3 v2 = { a, 3 };
			MGE::Vec4 v3 = a.xxyy;
			MGE::Mat33 mdecl = {
			  1, 0, 0,
			  0, 1, 0,
			  0, 0, 1,
			};
			MGE_CORE_INFO("mdecl[0]: [{0}, {1}]", mdecl(0, 0), mdecl(0, 1));
		}

	void OnUpdate() override {
		//MGE_CORE_INFO("ExampleLayer::Update");
	}

	void OnEvent(MGE::Event& event) override {
		if (event.GetEventType() == MGE::EventType::KeyPressed) {
			MGE::KeyPressedEvent& e = (MGE::KeyPressedEvent&)event;
			if (e.GetKeyCode() == MGE_KEY_TAB) {
				MGE_CORE_TRACE("Tab key is pressed (event)!");
			}
			MGE_CORE_TRACE("{0}", (char)e.GetKeyCode());
		}
	}

};

class Sandbox : public MGE::Application {
public:
	Sandbox() {
		PushLayer(new ExampleLayer());
		PushOverlay(new MGE::ImGuiLayer());
	}

	~Sandbox() {
	
	
	}

};

MGE::Application* MGE::CreateApplication() {
	return new Sandbox();
}