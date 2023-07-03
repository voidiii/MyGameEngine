#include "MGEpch.h"
#include "LayerStack.h"

namespace MGE {

    LayerStack::LayerStack()
    {
        // m_LayerInsert = m_Layers.begin();
    }

    LayerStack::~LayerStack() {
        for (auto layer : m_Layers) {
            delete layer;
        }
    }

    /* 
        The overlays are always added at the end of the vector, so we don't need to record the iterator for them.
        While the layers are added in the middle of the vector, so we need to record the iterator for them.
    */
   
    void LayerStack::PushLayer(Layer* layer) {
        m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, layer);
        m_LayerInsertIndex++;
    }

    void LayerStack::PushOverLayer(Layer* overlay) {
        m_Layers.emplace_back(overlay);
    }

    void LayerStack::PopLayer(Layer* layer) {
        auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
        if (it != m_Layers.end()) {
            m_Layers.erase(it);
            m_LayerInsertIndex--;
        }
    }

    void LayerStack::PopOverLayer(Layer* overlay) {
        auto it = std::find(m_Layers.begin(), m_Layers.end(), overlay);
        if (it != m_Layers.end()) {
            m_Layers.erase(it);
        }
    }
}