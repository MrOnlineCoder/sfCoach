#include <SFML/Graphics.hpp>
#include "../src/sfCoach.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "sfCoach Test by MrOnlineCoder");
    window.setFramerateLimit(60);

    sf::CircleShape shape(50);
    shape.setPosition(150, 150);
    shape.setFillColor(sf::Color::Green);

    sfCoach::Coach coach;
    sfCoach::DisplayAnimation hide(sfCoach::createTarget(shape), 3.2f);
    sfCoach::MoveAnimation move(sfCoach::createTarget(shape),sf::Vector2f(100.0f,100.0f), sf::Vector2f(300.0f,300.0f), 0.4f);
    coach.playAnimation(&hide);
    coach.playAnimation(&move);


    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        coach.update();

        window.clear(sf::Color::Black);

        window.draw(shape);

        window.display();
    }

    return 0;
}
