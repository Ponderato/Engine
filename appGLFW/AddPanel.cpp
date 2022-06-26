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
            AddLight();
            AddCamera();
            AddModel();

            ImGui::End();
        }
    }
}

void AddPanel::AddCamera() {

    //speed , fov, yaw, pitch
    ImGui::Columns(5);
    ImGui::SetColumnWidth(0, 300);

    ImGui::InputFloat("Speed", &speed, 0.1f);
    ImGui::NextColumn();

    ImGui::InputFloat("Fov", &fov, 1.0f);
    ImGui::NextColumn();

    ImGui::InputFloat("Yaw", &yaw, 1.0f);
    ImGui::NextColumn();

    ImGui::InputFloat("Pitch", &pitch, 1.0f);
    ImGui::NextColumn();

    if (ImGui::Button("Add Camera", ImVec2(80, 20))) {
        this->context->InitCamera(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 1.0f, 0.0f), (this->context->WIDTH / this->context->HEIGHT), speed, 0.1f, fov, yaw, pitch, this->context->parentNode);
    }

    ImGui::Columns(1);


}

void AddPanel::AddCube() {

    ImGui::Columns(4);
    ImGui::SetColumnWidth(0, 300);

    ImGui::InputText("Diffuse", diffusePath, sizeof(diffusePath));
    ImGui::NextColumn();

    ImGui::InputText("Specular", specPath, sizeof(specPath));
    ImGui::NextColumn();

    ImGui::InputText("Emissive", emissivePath, sizeof(emissivePath));
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

    ImGui::Columns(1);
}

void AddPanel::AddLight() {
    
    ImGui::Columns(4);
    ImGui::SetColumnWidth(0, 300);

    ImGui::InputFloat("Red", &red, 0.1f);
    if (red < 0.0f)
        red = 0.0f;
    ImGui::NextColumn();

    ImGui::InputFloat("Green", &green, 0.1f);
    if (green < 0.0f)
        green = 0.0f;
    ImGui::NextColumn();
    
    ImGui::InputFloat("Blue", &blue, 0.1f);
    if (blue < 0.0f)
        blue = 0.0f;
    ImGui::NextColumn();

    if (ImGui::Button("Add Light", ImVec2(80, 20))) {
        this->context->InitLightCube(glm::vec3(0.0f), glm::vec3(red, green, blue), this->context->parentNode);
    }

    ImGui::Columns(1);
}

void AddPanel::AddModel() {

    ImGui::Columns(2);
    ImGui::SetColumnWidth(0, 300);

    ImGui::InputText("Model Path", modelPath, sizeof(modelPath));
    ImGui::NextColumn();


    if (ImGui::Button("Add Model", ImVec2(80, 20))) {

        if (std::string(modelPath) != "") {
            this->context->InitModel(std::string(modelPath), glm::vec3(0.0f), this->context->parentNode);
        }
    }

    ImGui::Columns(1);
}