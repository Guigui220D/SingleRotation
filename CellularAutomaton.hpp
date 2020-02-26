#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <array>

#include <assert.h>


template <unsigned int WIDTH, unsigned int HEIGHT, unsigned int PIXEL_SIZE>
class CellularAutomaton
{
    public:
        CellularAutomaton();
        virtual ~CellularAutomaton();

        /**
         * Start the cellular automaton
         * Inits and start the main loop
        */
        void run();

        /**
         * Sets the framerate of the simulation
         * @param framerate : the framerate of the animation
        */
        inline void setFramerate(float framerate) { timeBetweenFrames = 1.f / framerate; }

    protected:
        /**
         * Inits the the simulation
         * Must be overridden
        */
        virtual void onInit() = 0;
        /**
         * Updates the simulation
         * Called regularly depending on the framerate
         */
        virtual void onUpdate() = 0;

        /**
         * Sets the color of a specific state
         * @param state : the state you want to set the color of
         * @param color : the new color of that state
        */
        void setStateColor(uint8_t state, sf::Color color) { colors.at(state) = color; }
        /**
         * Gets the color of a state
         * @param state : the state to get the color of
        */
        inline sf::Color getStateColor(uint8_t state) const { return colors.at(state); }

        /**
         * Gets a reference to the state at a position
         * The coordinates are looped if they are out of the grid
         * @param x and y : the coordinates
        */
        inline uint8_t& cellStateAt(unsigned int x, unsigned int y)
        {
            while (x < 0)
                x += WIDTH;
            while (y < 0)
                y += HEIGHT;
            while (x >= WIDTH)
                x -= WIDTH;
            while (y >= HEIGHT)
                y -= HEIGHT;
            return states[x + y * WIDTH];
        }
        /**
         * Gets the state of a cell at a position
         * The coordinates are looped if they are out of the grid
         * @param x and y : the coordinates
        */
        inline uint8_t getCellState(int x, int y) const
        {
            while (x < 0)
                x += WIDTH;
            while (y < 0)
                y += HEIGHT;
            while (x >= WIDTH)
                x -= WIDTH;
            while (y >= HEIGHT)
                y -= HEIGHT;
            return states[x + y * WIDTH];
        }

        uint8_t states[WIDTH * HEIGHT];

    private:
        float timeBetweenFrames = 1.f;

        /**
         * Updates the vertex array before drawing
         * Called after each update
        */
        void updateVAColors();
        /**
         * Prepares the vertex array coordinates
        */
        void prepareVA();

        sf::RenderWindow window;
        sf::VertexArray cells;

        std::array<sf::Color, 256> colors;
};



template <unsigned int WIDTH, unsigned int HEIGHT, unsigned int PIXEL_SIZE>
CellularAutomaton<WIDTH, HEIGHT, PIXEL_SIZE>::CellularAutomaton() :
    window(sf::VideoMode(WIDTH * PIXEL_SIZE, HEIGHT * PIXEL_SIZE), "Cellular Automaton"),
    cells(sf::Quads, HEIGHT * WIDTH * 4)
{
    window.setFramerateLimit(60);
}

template <unsigned int WIDTH, unsigned int HEIGHT, unsigned int PIXEL_SIZE>
CellularAutomaton<WIDTH, HEIGHT, PIXEL_SIZE>::~CellularAutomaton()
{

}

template <unsigned int WIDTH, unsigned int HEIGHT, unsigned int PIXEL_SIZE>
void CellularAutomaton<WIDTH, HEIGHT, PIXEL_SIZE>::run()
{
    //Initialization
    for (size_t i = 0; i < WIDTH * HEIGHT; i++)
        states[i] = 0;
    prepareVA();
    onInit();

    sf::Clock framerateClock;

    //First update
    updateVAColors();
    //Main loop
    while (window.isOpen())
    {
        { //Events
            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                    window.close();
            }
        }

        //Update
        if (framerateClock.getElapsedTime().asSeconds() >= timeBetweenFrames)
        {
            framerateClock.restart();
            onUpdate();
            updateVAColors();
        }

        //Drawing
        window.clear();
        //Draw cells
        window.draw(cells);
        window.display();
    }
}

template <unsigned int WIDTH, unsigned int HEIGHT, unsigned int PIXEL_SIZE>
void CellularAutomaton<WIDTH, HEIGHT, PIXEL_SIZE>::prepareVA()
{
    for (unsigned int x = 0; x < WIDTH; x++)
    for (unsigned int y = 0; y < HEIGHT; y++)
    {
        sf::Vector2f pos(x * PIXEL_SIZE, y * PIXEL_SIZE);
        cells[(x + y * WIDTH) * 4 + 0] = sf::Vertex(pos + sf::Vector2f(0000000000, 0000000000));
        cells[(x + y * WIDTH) * 4 + 1] = sf::Vertex(pos + sf::Vector2f(PIXEL_SIZE, 0000000000));
        cells[(x + y * WIDTH) * 4 + 2] = sf::Vertex(pos + sf::Vector2f(PIXEL_SIZE, PIXEL_SIZE));
        cells[(x + y * WIDTH) * 4 + 3] = sf::Vertex(pos + sf::Vector2f(0000000000, PIXEL_SIZE));
    }
}

template <unsigned int WIDTH, unsigned int HEIGHT, unsigned int PIXEL_SIZE>
void CellularAutomaton<WIDTH, HEIGHT, PIXEL_SIZE>::updateVAColors()
{
    for (unsigned int x = 0; x < WIDTH; x++)
    for (unsigned int y = 0; y < HEIGHT; y++)
    {
        sf::Color col = getStateColor(states[x + y * WIDTH]);
        cells[(x + y * WIDTH) * 4 + 0].color = col;
        cells[(x + y * WIDTH) * 4 + 1].color = col;
        cells[(x + y * WIDTH) * 4 + 2].color = col;
        cells[(x + y * WIDTH) * 4 + 3].color = col;
    }
}
