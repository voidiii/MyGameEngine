#pragma once
#include "MGE/Core.h"
#include "MGE/Layer.h"

#include <vector>
/* 
    LayerStack should be a class that stores the layers and overlays.
    It should be able to push and pop layers and overlays.
    Overlays are the layers that rendered on top of the layers, like UI, debug info, etc.
*/

namespace MGE {

    class MGE_API LayerStack
    {
    public:
        LayerStack();
        ~LayerStack();

        void PushLayer(Layer* layer);
        void PushOverLayer(Layer* overlay);
        void PopLayer(Layer* layer);
        void PopOverLayer(Layer* overlayer);

        std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
        std::vector<Layer*>::iterator end() { return m_Layers.end(); }

    private:
        std::vector<Layer*> m_Layers;
        std::vector<Layer*>::iterator m_LayerInsert;

    };

}