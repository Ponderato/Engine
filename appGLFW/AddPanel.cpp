#include "AddPanel.h"

AddPanel::AddPanel(Context* context) : Panel(context) {}

void AddPanel::OnImGuiRender() {

    if (close) {

        ImGui::SetNextWindowSize(ImVec2(500, 500));

        if (!ImGui::Begin("Add Node", &close))
        {
            close = false;
            ImGui::End();
        }
        else
        {
            ImGui::End();
        }
    }

}