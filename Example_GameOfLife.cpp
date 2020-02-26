#include "CellularAutomaton.hpp"

#include <iostream>

#include <stdio.h>

//Example of how to use the Cellular Automaton class to create one
class GameOfLife : public CellularAutomaton<100, 100, 8> //The template arguments are for the height of the grid, the width, and the cell size in pixels
{
    public:
        GameOfLife();
        ~GameOfLife();

    protected:
        void onInit() override;
        void onUpdate() override;

    private:
};

//Main
int main(int argc, char** argv)
{
    srand(time(0));

    //Create the game of life and run it
    GameOfLife gol;
    gol.run();
}




//Functions definitions for Game Of Life

GameOfLife::GameOfLife()
{

}

GameOfLife::~GameOfLife()
{

}

void GameOfLife::onInit()
{
    //Framerate
    setFramerate(6);
    //Prepare what colors will be used
    setStateColor(1, sf::Color::White); //Assigns the color white to the state 1
    //All other states are default, the default color is black.
    //setDefaultColor(sf::Color::Blue);

    //Fill the grid with random states
    for (int x = 1; x < 99; x++)
    for (int y = 1; y < 99; y++)
    {
        cellStateAt(x, y) = rand() % 2;
    }
}

void GameOfLife::onUpdate()
{
    //Game of life
    uint8_t nextState[100 * 100];

    for (int x = 0; x < 100; x++)
    for (int y = 0; y < 100; y++)
    {
        int neighbors = 0;
        for (int i = -1; i <= 1; i++)
        for (int j = -1; j <= 1; j++)
        {
            if (i == 0 && j == 0)
                continue;
            if (getCellState(x + i, y + j))
                neighbors++;
        }

        if (getCellState(x, y))
        {
            if (neighbors == 2 || neighbors == 3)
            {
                nextState[x + y * 100] = 1;
            }
            else
                nextState[x + y * 100] = 0;
        }
        else
        {
            if (neighbors == 3)
            {
                nextState[x + y * 100] = 1;
            }
            else
                nextState[x + y * 100] = 0;
        }
    }

    memcpy(states, nextState, 100 * 100);
}



