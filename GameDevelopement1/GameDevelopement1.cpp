#include <cstdlib>
#include <SFML/Graphics.hpp>
#include <windows.h>
int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML works!");
    //sf::CircleShape shape(100.f);   
    //shape.setFillColor(sf::Color::Green);
    //std::vector<sf::Vertex> vertices(2);
    //vertices[0].position = sf::Vector2f(400.f, 300.f);
    //vertices[0].color = sf::Color::Red;
    ////vertices[0].texCoords = sf::Vector2f(100.f, 100.f);

    //vertices[1].position = sf::Vector2f(100.f, 100.f);
    //vertices[1].color = sf::Color::Blue;
    //vertices[1].texCoords = sf::Vector2f(100.f, 100.f);

    /* lets create a vector of values and it will be associated to vertices array which are lines to be sorted */
    int N = 140;
    std::vector<int> arr(N);
    std::vector<sf::Vertex>vertices;

    srand((unsigned)time(NULL)); // to ensure we get different random number from rand at different times

    int startPoint = 50;
    for (int i = 0; i < N; i++) {

        int Random = 100 + (rand() % 300); // the random ranges from 100 - 400
        arr[i] = Random;
        startPoint = startPoint + 5;

        sf::Vertex vertex1;
        vertex1.position = sf::Vector2f((float)startPoint,0.f);
        vertex1.color = sf::Color::Red;

        sf::Vertex vertex2;
        vertex2.position = sf::Vector2f((float)startPoint, (float)Random);
        vertex2.color = sf::Color::Red;

        vertices.push_back(vertex1);
        vertices.push_back(vertex2);
    }

    bool ifComplete = false;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        if (ifComplete == false) {
            for (int i = 0; i < 2 * N; i++) {
                for (int j = 1; j < 2 * N - 2; j = j + 2) {
                    if (vertices[j].position.y > vertices[j + 2].position.y) {
                        std::swap(vertices[j].position.x, vertices[j + 2].position.x);
                        std::swap(vertices[j], vertices[j + 2]);
                    }
                    window.clear();
                    sf::sleep(sf::seconds(0.0008f));
                    window.draw(&vertices[0], 2 * N, sf::Lines);
                    window.display();
                }
            }
            ifComplete = true;
        }

        window.display();
    }


    return 0;
}