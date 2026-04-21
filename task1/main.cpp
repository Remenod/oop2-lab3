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
std::vector<std::string> action_logs;
bool scroll_to_bottom = false;

int create_buttons_logic(int val_from, int val_to, int step)
{
    static const int max_buttons = 10000;
    if (step <= 0 || val_from > val_to)
        return 1;
    if ((val_to - val_from) / step > max_buttons)
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
            int status = create_buttons_logic(from, to, step);
            switch (status)
            {
            case 0:
                action_logs.push_back("[System] Created buttons from " + std::to_string(from) +
                                      " to " + std::to_string(to) + " with step " + std::to_string(step));
                break;
            case 1:
                action_logs.push_back("[System] Invalid create config");
                break;
            case 2:
                action_logs.push_back("[System] Too many buttons");
                break;
            }
            scroll_to_bottom = true;
        }

        ImGui::Separator();

        ImGui::Text("Deleting:");
        ImGui::SetNextItemWidth(100);
        ImGui::InputInt("n", &erase_divisor);
        ImGui::SameLine();
        if (ImGui::Button("ERASE", ImVec2(100, 0)))
        {
            remove_buttons_logic(erase_divisor);
            action_logs.push_back("[System] Erased buttons divisible by " + std::to_string(erase_divisor));
            scroll_to_bottom = true;
        }

        ImGui::Separator();

        ImGui::Text("Buttons (%zu):", buttons.size());

        float available_y = ImGui::GetContentRegionAvail().y;
        float buttons_area_height = available_y * 0.65f;

        if (ImGui::BeginChild("ScrollingRegion", ImVec2(0, buttons_area_height), true))
        {
            float window_visible_x2 = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;
            ImGuiStyle &style = ImGui::GetStyle();

            for (size_t i = 0; i < buttons.size(); i++)
            {
                ImGui::PushID(static_cast<int>(i));

                bool pushed = false;
                if (buttons[i].already_pressed)
                {
                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.7f, 0.2f, 1.0f));
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.8f, 0.3f, 1.0f));
                    pushed = true;
                }

                std::string label = std::to_string(buttons[i].value);
                if (ImGui::Button(label.c_str(), ImVec2(60, 40)))
                {
                    std::string log_message;

                    if (!buttons[i].already_pressed)
                    {
                        buttons[i].cached_divisor = check_is_prime(buttons[i].value);
                        buttons[i].already_pressed = true;

                        if (buttons[i].cached_divisor == buttons[i].value)
                            log_message = "[New] Value " + std::to_string(buttons[i].value) + " is prime.";
                        else
                            log_message = "[New] Smallest divisor of " + std::to_string(buttons[i].value) + " is " + std::to_string(buttons[i].cached_divisor) + ".";
                    }
                    else
                    {
                        if (buttons[i].cached_divisor == buttons[i].value)
                            log_message = "[Cached] Value " + std::to_string(buttons[i].value) + " is prime.";
                        else
                            log_message = "[Cached] Smallest divisor of " + std::to_string(buttons[i].value) + " is " + std::to_string(buttons[i].cached_divisor) + ".";
                    }

                    action_logs.push_back(log_message);
                    std::cout << log_message << std::endl;
                    scroll_to_bottom = true;
                }

                if (pushed)
                {
                    ImGui::PopStyleColor(2);
                }

                float last_button_x2 = ImGui::GetItemRectMax().x;
                float next_button_x2 = last_button_x2 + ImGui::GetStyle().ItemSpacing.x + 60;
                if (i + 1 < buttons.size() && next_button_x2 < window_visible_x2)
                {
                    ImGui::SameLine();
                }

                ImGui::PopID();
            }
        }
        ImGui::EndChild();

        ImGui::Separator();

        ImGui::Text("Action Logs:");
        if (ImGui::BeginChild("LogRegion", ImVec2(0, 0), true))
        {
            for (const auto &log : action_logs)
            {
                ImGui::TextUnformatted(log.c_str());
            }

            if (scroll_to_bottom)
            {
                ImGui::SetScrollHereY(1.0f);
                scroll_to_bottom = false;
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