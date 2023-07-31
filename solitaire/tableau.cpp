#include "tableau.h"
#include <iostream>

tableau::tableau(textures tex, float start_x) {
    empty_space_texture = tex.loadFoundationTexture();
    empty_space.setCardFront(empty_space_texture);
    empty_space.setValue("base");
    empty_space.getCardSprite().setPosition(sf::Vector2f(start_x, 210.f));
}

void tableau::addCard(card& new_card) {
    if (isEmpty) {
        tableau_pile.clear();
        isEmpty = false;
    }
    tableau_pile.push_back(new_card);
}

void tableau::removeCard() {
    if (!tableau_pile.empty()) {
        tableau_pile.pop_back();
        if (tableau_pile.empty()) {
            isEmpty = true;
            tableau_pile.push_back(empty_space);
        }
    }
}

std::vector<card>& tableau::getTableauPile() {
    return tableau_pile;
}

void tableau::drawTableauPile(sf::RenderWindow& window, textures& tex, float start_x) {
    float start_y = 210.f;
    for (int i = 0; i < tableau_pile.size(); ++i) {
        if (isEmpty && i == tableau_pile.size() - 1) {
            empty_space.getCardSprite().setTexture(tex.loadFoundationTexture());
            empty_space.getCardSprite().setScale(1.5, 1.5);
            window.draw(empty_space.getCardSprite());
        } else {
            float x = start_x;
            float y = start_y + (25.f * i);
            if (tableau_pile[i].getIsUp()) {
                sf::Texture& front = tableau_pile[i].getCardFront();
                tableau_pile[i].getCardSprite().setTexture(front);
            } else {
                sf::Texture& back = tableau_pile[i].getCardBack();
                tableau_pile[i].getCardSprite().setTexture(back);
            }
            tableau_pile[i].getCardSprite().setPosition(sf::Vector2f(x, y));
            tableau_pile[i].getCardSprite().setScale(1.5, 1.5);
            window.draw(tableau_pile[i].getCardSprite());
        }
    }
}

bool tableau::canAddCard(card& current_card) {
    if (isEmpty) {
        return current_card.getValue() == "king";
    } else {
        std::string last_card_color = tableau_pile.back().getColor();
        std::string last_card_value = tableau_pile.back().getValue();
        auto place = std::find(tableau_values.begin(), tableau_values.end(), last_card_value);
        auto index = std::distance(tableau_values.begin(), place);
        if (index == 0)
            return current_card.getColor() != last_card_color && current_card.getValue() == "two";
        return current_card.getColor() != last_card_color && current_card.getValue() == tableau_values[index - 1];
    }
}
