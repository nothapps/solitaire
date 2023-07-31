#pragma once
#include "card.h"
#include "textures.h"
#include <SFML/Graphics.hpp>
#include <iostream>

class foundation {
	std::vector<std::string> foundation_values = { "ace", "two", "three", "four", "five", "six", "seven", "eight", "nine", "ten", "jack", "queen", "king" };
	std::vector<card> foundation_pile; 
	std::string suit; 
	bool is_empty = false;
	sf::Texture& foundation_base_texture;
	sf::Texture top_card_texture;
	card foundation_base;
public:
	foundation(textures& tex, int i);
	void addCard(card new_card);
	void removeCard();
	void drawFoundationPile(sf::RenderWindow& window, textures& tex, float start_x);
	bool canAddCard(card& current_card);
	std::vector<card>& getFoundationPile();
	card getFoundationBase();
	void setIsEmpty(bool empty);
	bool getIsEmpty();
	foundation& operator=(foundation& new_state) {
		foundation_pile = new_state.foundation_pile;
		suit = new_state.suit;
		is_empty = new_state.is_empty;
		foundation_base_texture = new_state.foundation_base_texture;
		top_card_texture = new_state.top_card_texture;
		foundation_base = new_state.foundation_base;
		return *this;
	}
};

