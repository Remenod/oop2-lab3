#include <iostream>
#include <vector>
#include <ranges>
#include <algorithm>

struct Button
{
    bool already_pressed;
    size_t value;
};

std::vector<Button> buttons;

int create_buttons(size_t val_from, size_t val_to, size_t step)
{
    static const int max_buttons = 1000;
    if (val_from > val_to)
        return 1;
    if ((val_to - val_from) / step > max_buttons)
        return 2;

    for (size_t i = val_from; i < val_to; i += step)
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
    std::erase_if(buttons, [n](int x)
                  { return x % n == 0; });
    return 0;
}

size_t check_is_prime(size_t number)
{
    if (number < 2)
        return number;

    if (number % 2 == 0)
        return (number == 2 ? number : 2);

    for (size_t i = 3; i * i <= number; i += 2)
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