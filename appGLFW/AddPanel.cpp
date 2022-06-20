#include "AddPanel.h"

AddPanel::AddPanel(Context* context) : Panel(context) {}

void AddPanel::OnImGuiRender() {

    if (close) {

        ImGui::SetNextWindowSize(ImVec2(800, 600));

        if (!ImGui::Begin("Add Node", &close))
        {
            close = false;
            ImGui::End();
        }
        else
        {
            AddCube();
            ImGui::End();
        }
    }

}

void AddPanel::AddCube() {

    ImGui::Columns(4);

    ImGui::SetColumnWidth(0, 250);

    ImGui::InputText("Diffuse", diffusePath, sizeof(diffusePath));
    ImGui::NextColumn();

    ImGui::InputText("Specular", specPath, sizeof(specPath));
    ImGui::NextColumn();

    ImGui::InputText("Emmisisve", emissivePath, sizeof(emissivePath));
    ImGui::NextColumn();

    if (ImGui::Button("Add Cube", ImVec2(80, 20))) {

        if (std::string(diffusePath) == "") {
            this->context->InitCube(glm::vec3(0.0f), this->context->parentNode);
        }
        else if (std::string(emissivePath) == "") {
            this->context->InitCube(std::string(diffusePath), std::string(specPath), glm::vec3(0.0f), this->context->parentNode);
        }
        else {
            this->context->InitCube(std::string(diffusePath), std::string(specPath), std::string(emissivePath), glm::vec3(0.0f), this->context->parentNode);
        }
    }

}