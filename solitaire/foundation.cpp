#include "foundation.h"

foundation::foundation(textures& tex, int i) : foundation_base_texture(tex.loadFoundationTexture()) {
    foundation_base.setCardFront(foundation_base_texture);
    foundation_base.setIsUp(true);
    foundation_base.setValue("base");
    foundation_pile.push_back(foundation_base);
}

void foundation::addCard(card new_card) {
    if (is_empty) {
        foundation_pile.pop_back();
        is_empty = false;
        suit = new_card.getSuit();
    } 
    top_card_texture = new_card.getCardFront();
    new_card.setCardFront(top_card_texture);
    foundation_pile.push_back(new_card);
}

void foundation::removeCard() {
    if (foundation_pile.size() > 1) {
        foundation_pile.pop_back();
        top_card_texture = foundation_pile.back().getCardFront();
    }  else
        is_empty = true;
}

void foundation::drawFoundationPile(sf::RenderWindow& window, textures& tex, float start_x) {
    float start_y = 10.f;
    for (int i = 0; i < foundation_pile.size(); i++) {
        foundation_pile[i].getCardSprite().setPosition(sf::Vector2f(start_x, start_y));
        foundation_pile[i].getCardSprite().setScale(1.5, 1.5);
        window.draw(foundation_pile[i].getCardSprite());
    }
}

bool foundation::canAddCard(card& current_card) {
    if (is_empty) {
        return current_card.getValue() == "ace";
    } else {
        std::string last_card_value = foundation_pile.back().getValue();
        auto place = std::find(foundation_values.begin(), foundation_values.end(), last_card_value);
        auto index = std::distance(foundation_values.begin(), place);
        return current_card.getSuit() == suit && current_card.getValue() == foundation_values[index + 1];
    }
}

std::vector<card>& foundation::getFoundationPile() {
    return foundation_pile;
}

void foundation::setIsEmpty(bool empty) {
    is_empty = empty;
}

bool foundation::getIsEmpty() {
    return is_empty;
}

card foundation::getFoundationBase() {
    return foundation_base;
}
