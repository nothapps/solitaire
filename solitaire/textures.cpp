#include "textures.h"
#include <SFML/Graphics.hpp>
#include <iostream>

sf::Texture& textures::loadIcon() {
    if (!icon_texture.loadFromFile("files/solitaire-icon.png")) 
        throw "Can't find texture - icon.";

    return icon_texture;
}

sf::Texture& textures::loadCardBack(int i) {
    std::string name;
    switch (i) {
    case 0:
        name = "bats";
        break;
    case 1:
        name = "fish";
        break;
    case 2:
        name = "hand";
        break;
    case 3:
        name = "lines";
        break;
    case 4:
        name = "robot";
        break;
    case 5:
        name = "roses";
        break;
    case 6:
        name = "shell";
        break;
    case 7:
        name = "sun";
        break;
    case 8:
        name = "vines";
        break;
    }

    if (!card_back_texture[i].loadFromFile("files/backs/" + name + ".png"))
        throw "Can't find texture - card back.";
    return card_back_texture[i];
}

void textures::setBackTextureChoice(int i) {
    back_texture_choice = i;
}

int textures::getBackTextureChoice() {
    return back_texture_choice;
}

sf::Texture& textures::loadCardFront(std::string suit, std::string value, int i) {
    std::string file_name = "files/" + suit + "-" + value + ".png";
    if (!card_front_texture[i].loadFromFile(file_name))
        throw "Can't find texture - card front.";
    return card_front_texture[i];
}

sf::Texture& textures::loadFoundationTexture() {
    if (!foundation_texture.loadFromFile("files/foundation.png"))
        throw "Can't find texture - foundation.";
    return foundation_texture;
}

sf::Texture& textures::loadWasteTexture() {
    if (!waste_texture.loadFromFile("files/waste.png"))
        throw "Can't find texture - waste.";
    return waste_texture;
}

sf::Texture& textures::loadChoiceOverlay() {
    if (!choice_overlay_texture.loadFromFile("files/choice_overlay.png"))
        throw "Can't find texture - choice overlay.";
    return choice_overlay_texture;
}
