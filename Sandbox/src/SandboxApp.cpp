#include <MGE.h>

class Sandbox : public MGE::Application {
public:
	Sandbox() {
	
	}

	~Sandbox() {
	
	
	}

};

MGE::Application* MGE::CreateApplication() {
	return new Sandbox();
}