#include "game.h"
#include "menu.h"
#include "textures.h"
#include <SFML/Graphics.hpp>

int main() {
    sf::RenderWindow window(sf::VideoMode(1280, 720), "solitaire", sf::Style::Close);
    sf::Image icon;
    icon.loadFromFile("files/solitaire-icon.png");
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    window.setFramerateLimit(60);
    textures tex;
    menu main_menu(window, tex);
	return 0;
}