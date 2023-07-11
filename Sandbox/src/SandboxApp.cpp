#pragma once
#include "MGE.h"

#include "imgui.h"
#include "MGE/ImGui/ImGuiLayer.h"

class ExampleLayer : public MGE::Layer 
{
public:
	ExampleLayer()
		: Layer("Example") {
		}

	void OnUpdate() override {
		//MGE_CORE_INFO("ExampleLayer::Update");
	}

	void virtual OnImGuiRender() override {
		//MGE_CORE_INFO("ExampleLayer::ImGuiRender");
		ImGui::Begin("Test");
		ImGui::Text("Hello World");
		ImGui::End();
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
		//PushOverlay(new MGE::ImGuiLayer());
	}

	~Sandbox() {
	
	
	}

};

MGE::Application* MGE::CreateApplication() {
	return new Sandbox();
}