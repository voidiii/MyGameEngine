#pragma once
#include "MGE/Core.h"
#include "MGE/Event/Event.h"
#include "MGE/Core/Timestep.h"

namespace MGE {

    class MGE_API Layer
    {
    public:
        Layer(const std::string& name = "Layer");
        virtual ~Layer();

        virtual void OnAttach() {}
        virtual void OnDetach() {}
        virtual void OnUpdate(Timestep ts) {}
        virtual void OnEvent(Event& event) {}
        virtual void OnImGuiRender() {}

        inline const std::string& GetName() const { return m_DebugName; }
    private:
        std::string m_DebugName;
    };
}