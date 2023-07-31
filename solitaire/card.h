#pragma once
#include <SFML/Graphics.hpp>
#include <cstring>
#include <vector>

class card {
	std::string card_color;
	std::string card_suit;
	std::string card_value;
	sf::Sprite card_sprite;
	sf::Texture card_front;
	sf::Texture card_back;
	bool is_up;
public:
	card();
	void setColor(std::string color);
	std::string getColor();
	void setSuit(std::string suit);
	std::string getSuit();
	void setValue(std::string value);
	std::string getValue();
	sf::Sprite& getCardSprite();
	void setCardFront(sf::Texture& tex);
	sf::Texture& getCardFront();
	void setCardBack(sf::Texture& tex);
	sf::Texture& getCardBack();
	void setIsUp(bool up);
	bool getIsUp();
};


