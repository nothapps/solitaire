#pragma once
#include "card.h"
#include "textures.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>
#include <random>

class waste {
    std::vector<card> waste_pile;
    int visible_index; 

public:
    waste();
    waste(textures& tex);
    void addCard(card new_card);
    void removeCard(int idx);
    void drawWastePile(sf::RenderWindow& window, textures& tex, float start_x);
    std::vector<card>& getWastePile();
    int getVisibleIndex();
    void setVisibleIndex(int idx);
};

