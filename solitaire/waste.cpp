#include "waste.h"

waste::waste() {
    visible_index = 0;
}

waste::waste(textures& tex) {
    card base_waste;
    base_waste.setCardBack(tex.loadWasteTexture());
    addCard(base_waste);
}

void waste::addCard(card new_card) {
    waste_pile.push_back(new_card);
}

void waste::removeCard(int idx) {
    waste_pile.erase(waste_pile.begin() + idx);
}

void waste::drawWastePile(sf::RenderWindow& window, textures& tex, float start_x) {
    float start_y = 10.f;

    for (int i = 0; i < waste_pile.size(); i++) {
        waste_pile[i].getCardSprite().setScale(1.5, 1.5);
        waste_pile[i].getCardSprite().setPosition(sf::Vector2f(start_x, start_y));
        if (i == visible_index) {
            waste_pile[i].setIsUp(true);
            sf::Texture& front = waste_pile[i].getCardFront();
            waste_pile[i].getCardSprite().setTexture(front);
            float offset = waste_pile[i].getCardSprite().getGlobalBounds().width + 10.f;
            waste_pile[i].getCardSprite().setPosition(sf::Vector2f(start_x + offset, start_y));
        } else {
            waste_pile[i].setIsUp(false);
            sf::Texture& back = waste_pile[i].getCardBack();
            waste_pile[i].getCardSprite().setTexture(back);
        }
        window.draw(waste_pile[i].getCardSprite());
    }
}

std::vector<card>& waste::getWastePile() {
    return waste_pile;
}

int waste::getVisibleIndex() {
    return visible_index;
}

void waste::setVisibleIndex(int idx) {
    visible_index = idx;
}