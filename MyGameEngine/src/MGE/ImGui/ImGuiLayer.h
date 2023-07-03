#pragma once

#include "MGE/Layer.h"
#include "MGE/Event/MouseEvent.h"
#include "MGE/Event/KeyEvent.h"  
#include "MGE/Event/ApplicationEvent.h"

 namespace MGE {

      class MGE_API ImGuiLayer : public Layer
      {
      public:
         ImGuiLayer();
         ~ImGuiLayer(); 
         virtual void OnAttach() override;
         virtual void OnDetach() override;
         virtual void OnImGuiRender() override;

         void Begin();
         void End();

      private:
          float m_Time = 0.0f;
      };
 }