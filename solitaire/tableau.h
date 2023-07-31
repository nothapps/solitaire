#pragma once
#include "card.h"
#include "textures.h"
#include <SFML/Graphics.hpp>
#include <vector>

class tableau {
	std::vector<std::string> tableau_values = { "ace", "two", "three", "four", "five", "six", "seven", "eight", "nine", "ten", "jack", "queen", "king" };
	std::vector<card> tableau_pile;
	card empty_space;
	sf::Texture empty_space_texture;
	bool isEmpty = false;
public:
	tableau(textures tex, float start_x);
	void addCard(card& new_card);
	void removeCard();
	std::vector<card>& getTableauPile();
	void drawTableauPile(sf::RenderWindow& window, textures& tex, float start_x);
	bool canAddCard(card& current_card);
};

