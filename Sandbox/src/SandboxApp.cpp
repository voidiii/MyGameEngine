#pragma once
#include "MGE.h"

class ExampleLayer : public MGE::Layer 
{
public:
	ExampleLayer()
		: Layer("Example") {}

	void OnUpdate() override {
		//MGE_CORE_INFO("ExampleLayer::Update");
	}

	void OnEvent(MGE::Event& event) override {
		MGE_CORE_TRACE("{0}", event);
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