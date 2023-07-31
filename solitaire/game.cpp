#include "game.h"

game::game(sf::RenderWindow& window, textures& tex, menu& main_menu) : game_window(window), menu_ref(main_menu) {
    std::random_device rand;
    game_mt.seed(rand());
    createCards(tex);
    shuffleDeck(game_mt);
    dealCards(tex);
    holding_card_tableau = false;
    holding_card_foundation = false;
    holding_card_waste = false;
    is_waste_clicked = false;

    sf::Font font;
    if (!font.loadFromFile("files/ms-sans-serif.ttf"))
        throw "Can't find font - buttons.";

    //menu button 
    menu_button.setFont(font);
    menu_button.setString("Menu");
    menu_button.setCharacterSize(24);
    menu_button.setFillColor(sf::Color::White);
    menu_button.setStyle(sf::Text::Bold);
    menu_button.setPosition(30.f, 10.f);

    //reset button
    reset_button.setFont(font);
    reset_button.setString("Reset");
    reset_button.setCharacterSize(24);
    reset_button.setFillColor(sf::Color::White);
    reset_button.setStyle(sf::Text::Bold);
    reset_button.setPosition(30.f, game_window.getSize().y - 40.f);

    //undo button
    undo_button.setFont(font);
    undo_button.setString("Undo");
    undo_button.setCharacterSize(24);
    undo_button.setFillColor(sf::Color::White);
    undo_button.setStyle(sf::Text::Bold);
    undo_button.setPosition(120.f, game_window.getSize().y - 40.f);

    //quit button
    quit_button.setFont(font);
    quit_button.setString("Quit");
    quit_button.setCharacterSize(24);
    quit_button.setFillColor(sf::Color::White);
    quit_button.setStyle(sf::Text::Bold);
    quit_button.setPosition(game_window.getSize().x - 70.f, game_window.getSize().y - 40.f);

    //timer text
    timer.setFont(font); 
    timer.setCharacterSize(24);
    timer.setFillColor(sf::Color::White);
    timer.setStyle(sf::Text::Bold);
    timer.setPosition(game_window.getSize().x / 2 - timer.getGlobalBounds().width / 2 - 30.f, game_window.getSize().y - 40.f);

    runGame(tex);
}

void game::runGame(textures& tex) {
    bool game_won = false;

    while (game_window.isOpen()) {
        sf::Event event;
        while (game_window.pollEvent(event)) {
            sf::Vector2f mouse_position = game_window.mapPixelToCoords(sf::Mouse::getPosition(game_window));
            if (event.type == sf::Event::Closed)
                game_window.close();

            if (menu_button.getGlobalBounds().contains(mouse_position)) {
                menu_button.setFillColor(sf::Color::Black);
                if (sf::Mouse::isButtonPressed((sf::Mouse::Left)))
                    menu_ref.run_menu(tex);
            } else
                menu_button.setFillColor(sf::Color::White);

            if (quit_button.getGlobalBounds().contains(mouse_position)) {
                quit_button.setFillColor(sf::Color::Black);
                if (sf::Mouse::isButtonPressed((sf::Mouse::Left)))
                    game_window.close();
            } else
                quit_button.setFillColor(sf::Color::White);

            if (reset_button.getGlobalBounds().contains(mouse_position)) {
                reset_button.setFillColor(sf::Color::Black);
                if (sf::Mouse::isButtonPressed((sf::Mouse::Left)))
                    resetGame(tex);
            } else 
                reset_button.setFillColor(sf::Color::White);

            if (undo_button.getGlobalBounds().contains(mouse_position)) {
                undo_button.setFillColor(sf::Color::Black);
                if (sf::Mouse::isButtonPressed((sf::Mouse::Left))) {
                    waste_pile = current_state.waste_state;
                    tableau_piles = current_state.tableau_state;
                    foundation_piles = current_state.foundation_state;
                }
            } else
                undo_button.setFillColor(sf::Color::White);
        }
        moveCard(event);
        drawGame(tex);

        //win condition
        int all_full = 0;
        for (int i = 0; i < 4; ++i) {
            if (foundation_piles[i].getFoundationPile().size() == 13)
                ++all_full;
        }

        if (all_full == 4) {
            showWinScreen();
            std::this_thread::sleep_for(std::chrono::seconds(4));
            return;
        }

        //loss condition
        if (checkForLoss()) {
            showLossScreen();
            std::this_thread::sleep_for(std::chrono::seconds(4));
            return;
        }

        //timer
        game_time = clock.getElapsedTime();
        int minutes = static_cast<int>(game_time.asSeconds()) / 60;
        int seconds = static_cast<int>(game_time.asSeconds()) % 60;
        std::stringstream timer_ss;
        timer_ss << std::setfill('0') << std::setw(2) << minutes << ":";
        timer_ss << std::setfill('0') << std::setw(2) << seconds;
        timer.setString(timer_ss.str().c_str());
    }
}

void game::drawGame(textures& tex) {
    //rectangle at the bottom
    game_window.clear(sf::Color(0, 128, 0));
    sf::RectangleShape rectangle;
    rectangle.setSize(sf::Vector2f(game_window.getSize().x, 50.f));
    rectangle.setPosition(0.f, game_window.getSize().y - 50.f);
    sf::Color darkGreen(0, 100, 0);
    rectangle.setFillColor(darkGreen);
    game_window.draw(rectangle);

    //buttons
    game_window.draw(menu_button);
    game_window.draw(reset_button);
    game_window.draw(undo_button);
    game_window.draw(quit_button);

    float start_x_t = 150.f;
    float card_width = 71.f * 1.5f;
    float space = (1280.f - 2 * start_x_t - 7 * card_width) / 6; //spaces between the cards (2x 150 for the borders - 7x card width)

    //waste pile
    waste_pile.drawWastePile(game_window, tex, start_x_t);

    //tableau piles
    for (int i = 0; i < tableau_piles.size(); ++i) {
        if (i >= tableau_piles.size())
            break;
        tableau_piles[i].drawTableauPile(game_window, tex, start_x_t);
        start_x_t += card_width + space; 
    }

    //foundation piles
    float start_x_f = start_x_t - (card_width + space);
    for (int i = 0; i < foundation_piles.size(); ++i) {
        foundation_piles[i].drawFoundationPile(game_window, tex, start_x_f);
        start_x_f -= card_width + space;
    }

    //timer
    game_window.draw(timer);
    game_window.display();
}

void game::createCards(textures& tex) {
    float start_x = 150.f;
    float card_width = 71.f * 1.5f;
    float space = (1280.f - 2 * start_x - 7 * card_width) / 6;
    int tmp = 1;
    for (int i = 0; i < suits.size(); ++i) {
        std::string& suit = suits[i];
        for (int j = 0; j < values.size(); ++j) {
            std::string& value = values[j];
            card* new_card = new card;
            if (suit == "hearts" || suit == "diamonds")
                new_card->setColor("red");
            else
                new_card->setColor("black");
            new_card->setSuit(suit);
            new_card->setValue(value);
            deck.push_back(new_card);
            new_card->setCardFront(tex.loadCardFront(suit, value, tmp));
            new_card->setCardBack(tex.loadCardBack(tex.getBackTextureChoice()));
            ++tmp;
        }
    }

    for (int i = 0; i < 7; i++) {
        tableau_piles.push_back(tableau(tex, start_x));
        start_x += card_width + space;
    }
}

void game::shuffleDeck(std::mt19937 mt) {
    std::shuffle(deck.begin(), deck.end(), mt);
}

void game::dealCards(textures& tex) {
    //foundation
    for (int i = 0; i < 4; ++i) {
        foundation new_foundation(tex, i);
        new_foundation.setIsEmpty(true);
        foundation_piles.push_back(new_foundation);
    }

    //tableau
    for (int i = 0; i < 7; ++i) {
        for (int j = 0; j <= i; ++j) {
            card& current_card = *deck.back();
            deck.pop_back();
            if (j == i) {
                current_card.setIsUp(true);
            }
            tableau_piles[i].addCard(current_card);
        }
    }

    //waste
    waste new_waste(tex);
    for (int i = deck.size() - 1; !deck.empty() ; --i) {
        card* current_card = deck[i];
        deck.pop_back();
        waste_pile.addCard(*current_card);
    }
}

void game::moveCard(sf::Event event) {
    wasteClick();
    movefromWaste(event);
    moveToTableau(event);
    moveToFoundation(event);
    moveFromFoundation(event);
}

void game::moveToTableau(sf::Event event) {
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f mouse_position = game_window.mapPixelToCoords(sf::Mouse::getPosition(game_window));
        //find source position
        auto result = getTableauCardAtPosition(mouse_position, tableau_piles);
        sf::Sprite* source_sprite = std::get<0>(result);
        int source_tableau_index = std::get<1>(result);
        int source_index = std::get<2>(result);
        //check if it exists and is not a base
        if (source_sprite && source_tableau_index != -1 && tableau_piles[source_tableau_index].getTableauPile().back().getValue() != "base") {
            holding_card_tableau = true;
            card_offset = source_sprite->getPosition() - mouse_position;
            selected_tableau_index = source_tableau_index;
            selected_card_indexes.clear();
            std::vector<card>& current_tableau = tableau_piles[selected_tableau_index].getTableauPile();
            for (int i = source_index; i < current_tableau.size(); ++i)
                selected_card_indexes.push_back(i);
        }
    } else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left && holding_card_tableau) {
        //find destination position
        sf::Vector2f mouse_position = game_window.mapPixelToCoords(sf::Mouse::getPosition(game_window));
        auto result = getTableauCardAtPosition(mouse_position, tableau_piles);
        sf::Sprite* dest_sprite = std::get<0>(result);
        int dest_tableau_index = std::get<1>(result);
        if (dest_sprite && dest_tableau_index != -1) {
            //check if valid move, if yes then move cards
            std::vector<card>& current_tableau = tableau_piles[selected_tableau_index].getTableauPile();
            card selected_card = current_tableau[selected_card_indexes.front()];
            std::vector<card>& source_pile = tableau_piles[selected_tableau_index].getTableauPile();
            if (selected_tableau_index != dest_tableau_index && tableau_piles[dest_tableau_index].canAddCard(selected_card)) {
                //update game state
                current_state.waste_state = waste_pile;
                current_state.tableau_state = tableau_piles;
                current_state.foundation_state = foundation_piles;

                //adding cards to their destinations
                for (int i : selected_card_indexes)
                    tableau_piles[dest_tableau_index].addCard(source_pile[i]);
                //removing cards from their source
                for (int i = selected_card_indexes.size() - 1; i >= 0 && !selected_card_indexes.empty(); --i)
                    tableau_piles[selected_tableau_index].removeCard();
                //flipping up the last card in source tableau
                tableau_piles[selected_tableau_index].getTableauPile().back().setIsUp(true);
            }
            holding_card_tableau = false;
            selected_tableau_index = -1;
        }

        if (holding_card_tableau) {
            sf::Vector2f mouse_position = game_window.mapPixelToCoords(sf::Mouse::getPosition(game_window));
            std::vector<card>& source_pile = tableau_piles[selected_tableau_index].getTableauPile();
            for (int i : selected_card_indexes) {
                card& current_card = source_pile[i];
                current_card.getCardSprite().setPosition(mouse_position + card_offset);
            }
        }
    }
}

void game::moveToFoundation(sf::Event event) {
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f mouse_position = game_window.mapPixelToCoords(sf::Mouse::getPosition(game_window));
        //find source position
        auto result = getTableauCardAtPosition(mouse_position, tableau_piles);
        sf::Sprite* source_sprite = std::get<0>(result);
        int source_index = std::get<1>(result);
        //check if it exists and is not a base
        if (source_sprite && source_index != -1 && tableau_piles[source_index].getTableauPile().back().getValue() != "base") {
            holding_card_tableau = true;
            card_offset = source_sprite->getPosition() - mouse_position;
            selected_tableau_index = source_index;
        }
    } else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left && holding_card_tableau) {
        //find destination position
        sf::Vector2f mouse_position = game_window.mapPixelToCoords(sf::Mouse::getPosition(game_window));
        auto result = getFoundationAtPosition(mouse_position, foundation_piles);
        sf::Sprite* dest_sprite = std::get<0>(result);
        int dest_index = std::get<1>(result);
        if (dest_sprite && dest_index != -1) {
            //check if valid move, if yes then move card
            card back_card = tableau_piles[selected_tableau_index].getTableauPile().back();
            if (foundation_piles[dest_index].canAddCard(back_card)) {
                //update game state
                current_state.waste_state = waste_pile;
                current_state.tableau_state = tableau_piles;
                current_state.foundation_state = foundation_piles;

                foundation_piles[dest_index].addCard(back_card);
                tableau_piles[selected_tableau_index].removeCard();
                tableau_piles[selected_tableau_index].getTableauPile().back().setIsUp(true);
            }
            holding_card_tableau = false;
            selected_tableau_index = -1;
        }

        if (holding_card_tableau) {
            sf::Vector2f mouse_position = game_window.mapPixelToCoords(sf::Mouse::getPosition(game_window));
            card back_card = tableau_piles[selected_tableau_index].getTableauPile().back();
            back_card.getCardSprite().setPosition(mouse_position + card_offset);
        }
    }
}

void game::moveFromFoundation(sf::Event event) {
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        //find source position
        sf::Vector2f mouse_position = game_window.mapPixelToCoords(sf::Mouse::getPosition(game_window));
        auto result = getFoundationAtPosition(mouse_position, foundation_piles);
        sf::Sprite* source_sprite = std::get<0>(result);
        int source_index = std::get<1>(result);
        //check if it exists and is not a base
        if (source_sprite && source_index != -1 && foundation_piles[source_index].getFoundationPile().back().getValue() != "base") {
            holding_card_foundation = true;
            card_offset = source_sprite->getPosition() - mouse_position;
            selected_foundation_index = source_index;
        }
    }  else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left && holding_card_foundation) {
        //find destination position
        sf::Vector2f mouse_position = game_window.mapPixelToCoords(sf::Mouse::getPosition(game_window));
        auto result = getTableauCardAtPosition(mouse_position, tableau_piles);
        sf::Sprite* dest_sprite = std::get<0>(result);
        int dest_index = std::get<1>(result);
        if (dest_sprite && dest_index != -1) {
            //check if valid move, if yes then move card
            card back_card = foundation_piles[selected_foundation_index].getFoundationPile().back();
            if (tableau_piles[dest_index].canAddCard(back_card)) {
                //update game state
                current_state.waste_state = waste_pile;
                current_state.tableau_state = tableau_piles;
                current_state.foundation_state = foundation_piles;

                tableau_piles[dest_index].addCard(back_card);
                foundation_piles[selected_foundation_index].removeCard();
                //if the pile is empty then draw a base
                if (foundation_piles[selected_foundation_index].getIsEmpty()) 
                    foundation_piles[selected_foundation_index].addCard(foundation_piles[selected_foundation_index].getFoundationBase());
            }
            holding_card_foundation = false;
            selected_foundation_index = -1;
        }

        if (holding_card_foundation) {
            sf::Vector2f mouse_position = game_window.mapPixelToCoords(sf::Mouse::getPosition(game_window));
            card back_card = foundation_piles[selected_foundation_index].getFoundationPile().back();
            back_card.getCardSprite().setPosition(mouse_position + card_offset);
        }
    }
}

std::tuple<sf::Sprite*, int, int> game::getTableauCardAtPosition(sf::Vector2f& mouse_position, std::vector<tableau>& tableau_piles) {
    for (int i = 0; i < tableau_piles.size(); ++i) {
        std::vector<card>& current_tableau = tableau_piles[i].getTableauPile();
        if (current_tableau.size() == 1) {
            sf::FloatRect bounds = current_tableau[0].getCardSprite().getGlobalBounds();
            if (bounds.contains(mouse_position))
                return std::make_tuple(&current_tableau[0].getCardSprite(), i, 0);
        } else {
            for (int j = current_tableau.size() - 1; j >= 0; --j) {
                sf::Sprite& sprite = current_tableau[j].getCardSprite();
                sf::FloatRect bounds = sprite.getGlobalBounds();
                if (bounds.contains(mouse_position))
                    return std::make_tuple(&sprite, i, j);
            }
        }
    }
    return std::make_tuple(nullptr, -1, -1);
}


std::tuple<sf::Sprite*, int> game::getFoundationAtPosition(sf::Vector2f& mouse_position, std::vector<foundation>& foundation_piles) {
    for (int i = 0; i < foundation_piles.size(); ++i) {
        std::vector<card>& current_foundation = foundation_piles[i].getFoundationPile();
        sf::Sprite& sprite = current_foundation.back().getCardSprite();
        sf::FloatRect bounds = sprite.getGlobalBounds();
        if (bounds.contains(mouse_position))
            return std::make_tuple(&sprite, i);
    }
    return std::make_tuple(nullptr, -1);
}

void game::movefromWaste(sf::Event event) {
    sf::Vector2f mouse_position = game_window.mapPixelToCoords(sf::Mouse::getPosition(game_window));
    int visible_index = waste_pile.getVisibleIndex();
    std::vector<card> waste = waste_pile.getWastePile();

    if (!waste.empty() && visible_index >= 0 && visible_index < waste.size()) {
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            //find source position
            sf::Vector2f mouse_position = game_window.mapPixelToCoords(sf::Mouse::getPosition(game_window));
            sf::Sprite& waste_sprite = waste[visible_index].getCardSprite();
            sf::FloatRect bounds = waste_sprite.getGlobalBounds();
            if (bounds.contains(mouse_position)) {
                holding_card_waste = true;
                card_offset = waste_sprite.getPosition() - mouse_position;
            }
        }  else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left && holding_card_waste) {
            //tableau
            auto tableau_result = getTableauCardAtPosition(mouse_position, tableau_piles);
            sf::Sprite* tableau_dest_sprite = std::get<0>(tableau_result);
            int tableau_dest_index = std::get<1>(tableau_result);
            //foundation
            auto foundation_result = getFoundationAtPosition(mouse_position, foundation_piles);
            sf::Sprite* foundation_dest_sprite = std::get<0>(foundation_result);
            int foundation_dest_index = std::get<1>(foundation_result);

            if (tableau_dest_sprite && tableau_dest_index != -1) {
                //check if valid move, if yes then move card
                card& current_card = waste[visible_index];
                if (tableau_piles[tableau_dest_index].canAddCard(current_card)) {
                    //update game state
                    current_state.waste_state = waste_pile;
                    current_state.tableau_state = tableau_piles;
                    current_state.foundation_state = foundation_piles;

                    tableau_piles[tableau_dest_index].addCard(current_card);
                    waste_pile.removeCard(visible_index);
                }
                holding_card_waste = false;
            }  else if (foundation_dest_sprite && foundation_dest_index != -1) {
                //check if valid move, if yes then move card
                card& current_card = waste[visible_index];
                if (foundation_piles[foundation_dest_index].canAddCard(current_card)) {
                    //update game state
                    current_state.waste_state = waste_pile;
                    current_state.tableau_state = tableau_piles;
                    current_state.foundation_state = foundation_piles;

                    foundation_piles[foundation_dest_index].addCard(current_card);
                    waste_pile.removeCard(visible_index);
                }
                holding_card_waste = false;
            }
        }

        if (holding_card_waste) {
            if (visible_index >= 0 && visible_index < waste.size()) {
                sf::Vector2f mouse_position = game_window.mapPixelToCoords(sf::Mouse::getPosition(game_window));
                card & current_card = waste.at(visible_index);
                current_card.getCardSprite().setPosition(mouse_position + card_offset);
            }
        }
    }

}

void game::resetGame(textures& tex) {
    tableau_piles.clear();
    foundation_piles.clear();
    deck.clear();
    waste_pile.getWastePile().clear();
    createCards(tex);
    shuffleDeck(game_mt);
    dealCards(tex);
    clock.restart();

    holding_card_waste = false;
    holding_card_tableau = false;
    holding_card_foundation = false;
    selected_tableau_index = -1;
    selected_foundation_index = -1;
    selected_card_indexes.clear();
    waste_pile.setVisibleIndex(0);
}

void game::wasteClick() {
    sf::Vector2f mouse_position = game_window.mapPixelToCoords(sf::Mouse::getPosition(game_window));
    int visible_index = waste_pile.getVisibleIndex();
    std::vector<card> waste = waste_pile.getWastePile();
    sf::Sprite waste_sprite;

    if (!waste.empty()) {
        if (visible_index >= 0 && visible_index < waste.size() && !is_waste_clicked) {
            //checks so that index is never out of bounds
            if (waste.size() == 1) {
                waste_sprite = waste[0].getCardSprite();
            } else if (visible_index == waste.size() - 1) { 
                waste_sprite = waste[0].getCardSprite(); 
            }  else 
                waste_sprite = waste[visible_index + 1].getCardSprite();
            if (waste_sprite.getGlobalBounds().contains(mouse_position) && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                is_waste_clicked = true;
                waste[visible_index].setIsUp(false);
                sf::Texture& back = waste[visible_index].getCardBack();
                waste[visible_index].getCardSprite().setTexture(back);
                if (waste.size() != 1)
                    waste_pile.setVisibleIndex(visible_index + 1);
            }
        } else if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
            is_waste_clicked = false;

        // loop waste cards
        if (visible_index >= waste.size() - 1) {
            waste_pile.setVisibleIndex(0);
        }
    }
}

void game::showWinScreen() {
    sf::Font font;
    if (!font.loadFromFile("files/ms-sans-serif.ttf"))
        throw "Can't find font - win text.";

    sf::Text win_text;
    win_text.setFont(font);
    win_text.setString("You win :)");
    win_text.setCharacterSize(48);
    win_text.setFillColor(sf::Color::White);
    win_text.setStyle(sf::Text::Bold);
    win_text.setPosition(game_window.getSize().x / 2.f - win_text.getGlobalBounds().width / 2.f, game_window.getSize().y / 2.f - win_text.getGlobalBounds().height / 2.f);

    sf::Text time_text;
    time_text.setFont(font);
    int minutes = static_cast<int>(game_time.asSeconds()) / 60;
    int seconds = static_cast<int>(game_time.asSeconds()) % 60;
    std::string time_str = "Time: " + std::to_string(minutes) + "m " + std::to_string(seconds) + "s";
    time_text.setString(time_str);
    time_text.setCharacterSize(24);
    time_text.setFillColor(sf::Color::White);
    time_text.setStyle(sf::Text::Regular);
    time_text.setPosition(game_window.getSize().x / 2.f - time_text.getGlobalBounds().width / 2.f, game_window.getSize().y / 2.f + win_text.getGlobalBounds().height);

    game_window.clear(sf::Color(50, 200, 50));
    game_window.draw(win_text);
    game_window.draw(time_text);
    game_window.display();
}

void game::showLossScreen() {
    sf::Font font;
    if (!font.loadFromFile("files/ms-sans-serif.ttf"))
        throw "Can't find font - loss text.";

    sf::Text loss_text;
    loss_text.setFont(font);
    loss_text.setString("You lose :(");
    loss_text.setCharacterSize(48);
    loss_text.setFillColor(sf::Color::White);
    loss_text.setStyle(sf::Text::Bold);
    loss_text.setPosition(game_window.getSize().x / 2.f - loss_text.getGlobalBounds().width / 2.f, game_window.getSize().y / 2.f - loss_text.getGlobalBounds().height / 2.f);

    sf::Text loss_text2;
    loss_text2.setFont(font);
    loss_text2.setString("Can't make any more moves!");
    loss_text2.setCharacterSize(24);
    loss_text2.setFillColor(sf::Color::White);
    loss_text2.setStyle(sf::Text::Regular);
    loss_text2.setPosition(game_window.getSize().x / 2.f - loss_text2.getGlobalBounds().width / 2.f, game_window.getSize().y / 2.f + loss_text.getGlobalBounds().height);

    game_window.clear(sf::Color(200, 50, 50));
    game_window.draw(loss_text);
    game_window.draw(loss_text2);
    game_window.display();
}

bool game::checkForLoss() {
    //can any waste cards be moved
    std::vector<card> waste = waste_pile.getWastePile();
    if (!waste.empty()) {
        for (int i = 0; i < waste.size(); ++i) {
            //check if can be added to foundations
            for (int j = 0; j < foundation_piles.size(); ++j) {
                if (foundation_piles[j].canAddCard(waste[i]))
                    return false;
            }
            //check if can be added to tableau
            for (int k = 0; k < tableau_piles.size(); ++k) {
                if (tableau_piles[k].canAddCard(waste[i]))
                    return false;
            }
        }
    }

    //can any tableau card be moved
    for (int i = 0; i < tableau_piles.size(); ++i) {
        //check if can be added to foundations
        for (int j = 0; j < foundation_piles.size(); ++j) {
            if (!tableau_piles[i].getTableauPile().empty()) {
                if (foundation_piles[j].canAddCard(tableau_piles[i].getTableauPile().back()))
                    return false;
            }
        }
    }

    //can any foundation card be moved
    for (int i = 0; i < foundation_piles.size(); ++i) {
        //check if can be added to tableau
        for (int j = 0; j < foundation_piles.size(); ++j) {
            if (!foundation_piles[i].getFoundationPile().empty()) {
                if (tableau_piles[j].canAddCard(foundation_piles[i].getFoundationPile().back()))
                    return false;
            }
        }
    }
    return true;
}
