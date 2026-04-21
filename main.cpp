#include <iostream>
#include <vector>
#include <ranges>
#include <algorithm>
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <imgui.h>
#include <imgui-SFML.h>

struct Button
{
    bool already_pressed;
    int value;
    int cached_divisor;
};

std::vector<Button> buttons;

int create_buttons_logic(int val_from, int val_to, int step)
{
    static const int max_buttons = 1000;
    if (val_from > val_to)
        return 1;
    if (step <= 0 || (val_to - val_from) / step > max_buttons)
        return 2;

    for (int i = val_from; i <= val_to; i += step)
    {
        buttons.emplace_back(Button{
            .already_pressed = false,
            .value = i,
            .cached_divisor = 0});
    }
    return 0;
}

void remove_buttons_logic(int n)
{
    if (n == 0)
        return;
    std::erase_if(buttons, [n](const Button &b)
                  { return b.value % n == 0; });
}

int check_is_prime(int number)
{
    if (number < 2)
        return number;

    if (number % 2 == 0)
        return (number == 2 ? number : 2);

    for (int i = 3; i * i <= number; i += 2)
    {
        if (number % i == 0)
            return i;
    }

    return number;
}

int main()
{
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Lab 3 - Button Manager");
    window.setFramerateLimit(60);

    if (!ImGui::SFML::Init(window))
        return -1;

    int from = 0, to = 100, step = 1;
    int erase_divisor = 1;

    sf::Clock deltaClock;
    while (window.isOpen())
    {
        while (const std::optional<sf::Event> event = window.pollEvent())
        {
            ImGui::SFML::ProcessEvent(window, *event);
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        ImGui::SFML::Update(window, deltaClock.restart());

        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);

        ImGui::Begin("Main Control", nullptr,
                     ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

        ImGui::Text("Creating:");
        ImGui::SetNextItemWidth(100);
        ImGui::InputInt("From", &from);
        ImGui::SameLine();
        ImGui::SetNextItemWidth(100);
        ImGui::InputInt("To", &to);
        ImGui::SameLine();
        ImGui::SetNextItemWidth(100);
        ImGui::InputInt("Step", &step);
        ImGui::SameLine();

        if (ImGui::Button("CREATE", ImVec2(100, 0)))
        {
            create_buttons_logic(from, to, step);
        }

        ImGui::Separator();

        ImGui::Text("Deleting:");
        ImGui::SetNextItemWidth(100);
        ImGui::InputInt("n", &erase_divisor);
        ImGui::SameLine();
        if (ImGui::Button("ERASE", ImVec2(100, 0)))
        {
            remove_buttons_logic(erase_divisor);
        }

        ImGui::Separator();

        ImGui::Text("Buttons (%zu):", buttons.size());

        float child_height = ImGui::GetContentRegionAvail().y - 20.0f;

        if (ImGui::BeginChild("ScrollingRegion", ImVec2(0, child_height), true))
        {
            float window_visible_x2 = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;
            ImGuiStyle &style = ImGui::GetStyle();

            for (size_t i = 0; i < buttons.size(); i++)
            {
                ImGui::PushID(static_cast<int>(i));

                if (buttons[i].already_pressed)
                {
                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.7f, 0.2f, 1.0f));
                }

                std::string label = std::to_string(buttons[i].value);
                if (ImGui::Button(label.c_str(), ImVec2(60, 40)))
                {
                    buttons[i].already_pressed = !buttons[i].already_pressed;
                }

                if (buttons[i].already_pressed)
                {
                    ImGui::PopStyleColor();
                }

                float last_button_x2 = ImGui::GetItemRectMax().x;
                float next_button_x2 = last_button_x2 + style.ItemSpacing.x + 60;
                if (i + 1 < buttons.size() && next_button_x2 < window_visible_x2)
                {
                    ImGui::SameLine();
                }

                ImGui::PopID();
            }
        }
        ImGui::EndChild();

        ImGui::End();

        window.clear(sf::Color(30, 30, 30));
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;
}