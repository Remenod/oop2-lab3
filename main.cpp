#include <iostream>
#include <vector>

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
            value : i,
        });
    }
}

int main()
{
    return 0;
}