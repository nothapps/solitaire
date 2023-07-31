#pragma once
#include <SFML/Graphics.hpp>
#include "textures.h"
#include "game.h"
#include "settings.h"

class menu {
	sf::RenderWindow& menu_window;
	sf::Sprite icon;
	sf::Font menu_font;
	sf::Text title_text;
	sf::Text play_text;
	sf::Text settings_text;
	sf::Text quit_text;
	bool return_to_menu;
public:
	menu(sf::RenderWindow& window, textures& tex);
	void run_menu(textures& tex);
	void setMenu(textures& tex);
	void drawMenu();
	void userInputMenu(textures& tex);
};


