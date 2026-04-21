#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <imgui.h>
#include <imgui-SFML.h>

#include <iostream>
#include <random>
#include <optional>
#include <cmath>
#include <algorithm>

int main()
{
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Trick Button Program");
    window.setFramerateLimit(240);

    if (!ImGui::SFML::Init(window))
    {
        std::cerr << "Failed to initialize ImGui-SFML\n";
        return -1;
    }

    sf::Clock deltaClock;
    ImVec2 noButtonPos(200.0f, 100.0f);

    std::random_device rd;
    std::mt19937 gen(rd());

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            ImGui::SFML::ProcessEvent(window, *event);

            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }

        sf::Time dt = deltaClock.restart();
        ImGui::SFML::Update(window, dt);
        float dtSeconds = dt.asSeconds();

        ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
        ImGui::Begin("Important Question", nullptr, ImGuiWindowFlags_NoCollapse);

        ImGui::Text("Do you like programming in C++?");
        ImGui::Spacing();

        if (ImGui::Button("Yes", ImVec2(100, 30)))
        {
            ImGui::OpenPopup("Result");
        }

        ImVec2 mousePos = ImGui::GetMousePos();
        ImVec2 windowPos = ImGui::GetWindowPos();
        ImVec2 localMousePos(mousePos.x - windowPos.x, mousePos.y - windowPos.y);

        ImVec2 buttonCenter(noButtonPos.x + 50.0f, noButtonPos.y + 15.0f);

        float dx = buttonCenter.x - localMousePos.x;
        float dy = buttonCenter.y - localMousePos.y;
        float dist = std::sqrt(dx * dx + dy * dy);

        float evadeRadius = 130.0f;
        ImVec2 winSize = ImGui::GetWindowSize();

        if (dist < evadeRadius && dist > 0.1f)
        {
            float force = (evadeRadius - dist) / evadeRadius;
            float moveSpeed = 800.0f * dtSeconds;

            noButtonPos.x += (dx / dist) * force * moveSpeed;
            noButtonPos.y += (dy / dist) * force * moveSpeed;

            noButtonPos.x = std::max(10.0f, std::min(noButtonPos.x, winSize.x - 110.0f));
            noButtonPos.y = std::max(30.0f, std::min(noButtonPos.y, winSize.y - 40.0f));
        }

        ImGui::SetCursorPos(noButtonPos);
        ImGui::Button("No", ImVec2(100, 30));

        if (ImGui::IsItemHovered())
        {
            std::uniform_real_distribution<float> distX(10.0f, winSize.x - 110.0f);
            std::uniform_real_distribution<float> distY(30.0f, winSize.y - 40.0f);
            noButtonPos = ImVec2(distX(gen), distY(gen));
        }

        if (ImGui::BeginPopupModal("Result", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::Text("As expected!");
            ImGui::Spacing();

            if (ImGui::Button("OK", ImVec2(120, 0)))
            {
                ImGui::CloseCurrentPopup();
                window.close();
            }
            ImGui::EndPopup();
        }

        ImGui::End();

        window.clear(sf::Color({40, 40, 40}));
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;
}