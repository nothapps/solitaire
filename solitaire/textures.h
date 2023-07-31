#pragma once
#include <SFML/Graphics.hpp>

class textures {
	sf::Texture icon_texture;
	sf::Texture card_back_texture[9];
	sf::Texture card_front_texture[52];
	sf::Texture foundation_texture;
	sf::Texture waste_texture;
	sf::Texture choice_overlay_texture;
	int back_texture_choice = 4;
public:
	sf::Texture& loadIcon();
	sf::Texture& loadCardBack(int i);
	void setBackTextureChoice(int i);
	int getBackTextureChoice();
	sf::Texture& loadCardFront(std::string suit, std::string value, int tmp);
	sf::Texture& loadFoundationTexture();
	sf::Texture& loadWasteTexture();
	sf::Texture& loadChoiceOverlay();
};

