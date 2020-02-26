#include "CellularAutomaton.hpp"

#include <iostream>

#include <stdio.h>

class SingleRotation : public CellularAutomaton<100, 100, 8>
{
    public:
        SingleRotation();
        ~SingleRotation();

    private:
        void onInit() override;
        void onUpdate() override;

        bool shift = false;
};

int main(int argc, char** argv)
{
    srand(time(0));

    SingleRotation automaton;
    automaton.run();
}




SingleRotation::SingleRotation() {}

SingleRotation::~SingleRotation() {}

void SingleRotation::onInit()
{
    setFramerate(60);

    setStateColor(255, sf::Color::Red);
    for (uint8_t i = 0; i <= 254; i++)
        setStateColor(i, sf::Color(0, 0, i));

    //Fill the grid with random states
    for (int x = 1; x < 30; x++)
    for (int y = 1; y < 99; y++)
        cellStateAt(x, y) = !(rand() % 3) * 255;
}

void SingleRotation::onUpdate()
{
    for (int x = shift; x < 100; x += 2)
    for (int y = shift; y < 100; y += 2)
    {
        int count = 0;
        for (int i = 0; i <= 1; i++)
        for (int j = 0; j <= 1; j++)
            if (getCellState(x + i, y + j) == 255)
                count++;

        if (count == 1)
        {
            uint8_t first = getCellState(x, y);

            if (cellStateAt(x, y + 1) == 255) { cellStateAt(x, y) = 255; cellStateAt(x, y + 1)--; }
            if (cellStateAt(x + 1, y + 1) == 255) { cellStateAt(x, y + 1) = 255; cellStateAt(x + 1, y + 1)--; }
            if (cellStateAt(x + 1, y) == 255) { cellStateAt(x + 1, y + 1) = 255; cellStateAt(x + 1, y)--; }
            if (first == 255) { cellStateAt(x + 1, y) = 255; cellStateAt(x, y)--; }
        }
    }

    for (int x = 0; x < 100; x ++)
    for (int y = 0; y < 100; y ++)
        if (getCellState(x, y) && getCellState(x, y) != 255)
            cellStateAt(x, y)--;

    shift = !shift;
}



