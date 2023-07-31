#include "card.h"

card::card() {
	card_color = "";  
	card_suit = "";  
	card_value = "";  
	is_up = false;     
}

void card::setColor(std::string color) {
	card_color = color;
}

std::string card::getColor() {
	return card_color;
}

void card::setSuit(std::string suit) {
	card_suit = suit;
}
 
std::string card::getSuit() {
	return card_suit;
}

void card::setValue(std::string value) {
	card_value = value;
}

std::string card::getValue() {
	return card_value;
}
sf::Sprite& card::getCardSprite() {
	return card_sprite;
}

void card::setCardFront(sf::Texture& tex) {
	card_front = tex;
	card_sprite.setTexture(tex);
}

sf::Texture& card::getCardFront() {
	return card_front;
}

void card::setCardBack(sf::Texture& tex) {
	card_back = tex;
	card_sprite.setTexture(tex);
}

sf::Texture& card::getCardBack() {
	return card_back;
}

void card::setIsUp(bool up) {
	is_up = up;
}

bool card::getIsUp() {
	return is_up;
}
