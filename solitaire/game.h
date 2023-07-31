#pragma once
#include <SFML/Graphics.hpp>
#include "menu.h"
#include "card.h"
#include "textures.h"
#include "tableau.h"
#include "foundation.h"
#include "waste.h"
#include <vector>
#include <algorithm>
#include <random>
#include <tuple>
#include <iostream>
#include <chrono>
#include <thread>
#include <iomanip>
#include <sstream>

struct game_state {
	waste waste_state;
	std::vector<tableau> tableau_state;
	std::vector<foundation> foundation_state;
};

class menu;

class game {
	std::vector<std::string> suits = { "hearts", "diamonds", "spades", "clubs" };
	std::vector<std::string> values = { "ace", "two", "three", "four", "five", "six", "seven", "eight", "nine", "ten", "jack", "queen", "king" };
	sf::RenderWindow& game_window;
	std::vector<card*> deck;
	std::vector<tableau> tableau_piles;
	std::vector<foundation> foundation_piles;
	waste waste_pile;
	std::mt19937 game_mt;
	sf::Text quit_button;
	sf::Text reset_button;
	sf::Text timer;
	sf::Clock clock;
	sf::Time game_time;
	sf::Text menu_button;
	menu& menu_ref;
	sf::Text undo_button;
	game_state current_state;
	bool is_waste_clicked;
	bool holding_card_tableau;
	bool holding_card_foundation;
	bool holding_card_waste;
	sf::Sprite* source_sprite;
	sf::Vector2f card_offset;
	int selected_tableau_index;
	int selected_foundation_index;
	std::vector<int> selected_card_indexes;
public:
	game(sf::RenderWindow& window, textures& tex, menu& main_menu);
	void runGame(textures& tex);
	void drawGame(textures& tex);
	void shuffleDeck(std::mt19937 mt);
	void dealCards(textures& tex);
	void createCards(textures& tex);
	void moveCard(sf::Event event);
	void moveToTableau(sf::Event event);
	void moveToFoundation(sf::Event event);
	void moveFromFoundation(sf::Event event);
	void movefromWaste(sf::Event event);
	std::tuple<sf::Sprite*, int, int> getTableauCardAtPosition(sf::Vector2f& mouse_position, std::vector<tableau>& tableau_piles);
	std::tuple<sf::Sprite*, int> getFoundationAtPosition(sf::Vector2f& mouse_position, std::vector<foundation>& foundation_piles);
	void resetGame(textures& tex);
	void wasteClick();
	void showWinScreen();
	void showLossScreen();
	bool checkForLoss();
};

