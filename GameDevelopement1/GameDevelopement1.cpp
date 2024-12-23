#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <thread>
#include <string>

// Window dimensions
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

// Sorting bars
const int NUM_BARS = 500;
// const int BAR_SPACING = 5;
const float BAR_SPACING = static_cast<float> (WINDOW_WIDTH) / NUM_BARS;

// Initialize random heights for bars and create vertices
void initializeBars(std::vector<int>& heights, std::vector<sf::Vertex>& vertices) {
    std::srand(static_cast<unsigned>(std::time(0)));
    float startPoint = 0;

    for (int i = 0; i < NUM_BARS; i++) {
        int randomHeight = 100 + (std::rand() % 300); // Random heights between 100 and 400
        heights[i] = randomHeight;
        startPoint += BAR_SPACING;

        sf::Vertex vertex1(sf::Vector2f(static_cast<float>(startPoint), 0.f), sf::Color::Red);
        sf::Vertex vertex2(sf::Vector2f(static_cast<float>(startPoint), static_cast<float>(randomHeight)), sf::Color::Red);

        vertices.push_back(vertex1);
        vertices.push_back(vertex2);
    }
}

// Perform one pass of Bubble Sort
bool bubbleSortStep(std::vector<sf::Vertex>& vertices) {
    bool swapped = false;

    for (size_t j = 1; j < vertices.size() - 2; j += 2) {
        if (vertices[j].position.y > vertices[j + 2].position.y) {
            std::swap(vertices[j].position.y, vertices[j + 2].position.y);
            swapped = true;
        }
    }
    return swapped;
}

// Render the sorting visualization
void drawVisualization(sf::RenderWindow& window, const std::vector<sf::Vertex>& vertices, sf::Text& timeText) {
    window.clear(sf::Color::Black);
    window.draw(&vertices[0], vertices.size(), sf::Lines);
    window.draw(timeText);
    window.display();
}

int main() {
    // Initialize the window
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Bubble Sort Visualization");
    window.setFramerateLimit(60);

    // Initialize bars and sorting variables
    std::vector<int> heights(NUM_BARS);
    std::vector<sf::Vertex> vertices;
    initializeBars(heights, vertices);

    // Load font for time display
    sf::Font font;
    if (!font.loadFromFile("C:\\archit_3\\open-sans\\OpenSans-Regular.ttf")) {
        std::cerr << "Error: Could not load font.\n";
        return -1;
    }

    sf::Clock clock;
    std::string timeInSeconds = "0";
    float finalTime = 0.0f;
    bool sortingComplete = false;

    // Main loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        // Perform sorting if not complete
        if (!sortingComplete) {
            if (!bubbleSortStep(vertices)) {
                sortingComplete = true;
                finalTime = clock.getElapsedTime().asSeconds();
                std::cout << "Sorting Complete in " << finalTime << " seconds!\n";
            }
        }

        // Update time display
        sf::Text timeText;
        if (!sortingComplete) {
            sf::Time elapsed = clock.getElapsedTime();
            timeInSeconds = std::to_string(elapsed.asSeconds());
        }
        else {
            timeInSeconds = std::to_string(finalTime);
        }

        timeText.setFont(font);
        timeText.setString("Time elapsed: " + timeInSeconds + " seconds");
        timeText.setCharacterSize(24);
        timeText.setFillColor(sf::Color::White);
        timeText.setPosition(10.f, 550.f);

        // Draw visualization
        drawVisualization(window, vertices, timeText);

        // Slow down sorting for better visualization
        if (!sortingComplete) {
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
    }

    return 0;
}
