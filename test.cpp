#include <SFML/Graphics.hpp>

int main()
{
    // First, I load the logo and create an sprite
    sf::Texture logo;

    if (!logo.loadFromFile("data/galaxy.jpg")){
        exit(1);
    }

    sf::Sprite sp;
    sp.setTexture(logo);
    sp.scale(0.5, 0.5); // My logo is quite big in fact (1st google result btw)

    int logoWidth = sp.getGlobalBounds().width;
    int logoHeight = sp.getGlobalBounds().height;

    // With the logo loaded, I know its size, so i create the window accordingly
    sf::RenderWindow window1(sf::VideoMode(logoWidth, logoHeight), "SFML", sf::Style::None); // <- Important!! Style=None removes title

    // To close splash window by timeout, I just suppose you want something like this, don't you?
    sf::Clock timer;
    sf::Time time = timer.restart();

    while (window1.isOpen()){
        sf::Event event;
        while (window1.pollEvent(event)){
            // event loop is always needed
        }
        // Window closed by time
        time += timer.restart();
        if (time >= sf::seconds(2.f)){
            window1.close();
        }

        window1.clear();
        window1.draw(sp);
        window1.display();
    }

    // Then, reuse the window and do things again
    sf::RenderWindow window(sf::VideoMode(600, 400), "SFML");

    while (window.isOpen()){
        sf::Event event;
        while (window.pollEvent(event)){

        }

        window.clear();
        window.draw(sp);
        window.display();
    }

    return 0;
}