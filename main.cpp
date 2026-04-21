#include <iostream>
#include <vector>
#include <ranges>
#include <algorithm>

struct Button
{
    bool already_pressed;
    int value;
};

std::vector<Button> buttons;

int create_buttons(int val_from, int val_to, int step)
{
    static const int max_buttons = 1000;
    if (val_from > val_to)
        return 1;
    if ((val_to - val_from) / step > max_buttons)
        return 2;

    for (int i = val_from; i < val_to; i += step)
    {
        buttons.emplace_back(new Button{
            already_pressed : false,
            value : i,
        });
    }
    return 0;
}

int remove_buttons(int n)
{
    std::erase_if(buttons, [n](const Button &b)
                  { return b.value % n == 0; });
    return 0;
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

int main(void)
{

    return 0;
}