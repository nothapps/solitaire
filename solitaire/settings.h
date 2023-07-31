#pragma once
#include "textures.h"
#include <SFML/Graphics.hpp>
#include "menu.h"

class menu;

class settings {
	sf::RenderWindow& settings_window;
	sf::Sprite back_selection[9];
	sf::Texture card_backs[9];
	sf::Font settings_font;
	sf::Text selection_text;
	sf::Text back_button;
	sf::Sprite choice_overlay;
	menu& menu_ref;
public:
	settings(sf::RenderWindow& window, textures& tex, menu& main_menu);
	void setSettings(textures& tex);
	void drawSettings();
	void chooseBack(textures& tex);
};

