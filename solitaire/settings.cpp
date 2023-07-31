#include "settings.h"
#include <iostream>

settings::settings(sf::RenderWindow& window, textures& tex, menu& main_menu) : settings_window(window), menu_ref(main_menu) {
    setSettings(tex);
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        drawSettings();
        chooseBack(tex);
    }
}

void settings::setSettings(textures& tex) {
    if (!settings_font.loadFromFile("files/ms-sans-serif.ttf"))
        throw "Can't find font.";
    
    //selection text
    selection_text.setFont(settings_font);
    selection_text.setString("Select card back:");
    selection_text.setCharacterSize(70);
    selection_text.setFillColor(sf::Color::White);
    sf::FloatRect selection_bounds = selection_text.getLocalBounds();
    float title_x = (settings_window.getSize().x - selection_bounds.width) / 2; //centering text
    selection_text.setPosition(title_x, 30.f);

    //card backs
    for (int i = 0; i < 9; ++i)
        card_backs[i] = tex.loadCardBack(i);

    float scale = 1.90f;
    float start_x = title_x;
    float start_y = 130.f;
    float card_height = 96.f * scale;
    float card_width = 71.f * scale;
    float space = 45.f;
    int tmp = 0;

    for (int i = 0; i < 9; ++i) {
        back_selection[i].setTexture(card_backs[i]);
        back_selection[i].setPosition(sf::Vector2f(start_x, start_y));
        back_selection[i].setScale(scale,scale);
        ++tmp;
        if (tmp % 3 == 0 && tmp != 0) {
            start_x = title_x;
            start_y += card_height + 15.f;
        } else 
            start_x += card_width + space;
    }

    //card choice overlay
    choice_overlay.setTexture(tex.loadChoiceOverlay());
    choice_overlay.setScale(scale, scale);
    choice_overlay.setPosition(back_selection[4].getPosition());  

    //back button
    back_button.setFont(settings_font);
    back_button.setString("Back");
    back_button.setCharacterSize(35);
    back_button.setFillColor(sf::Color::White);
    sf::FloatRect back_bounds = back_button.getLocalBounds();
    float back_x = 20.f; 
    float back_y = 20.f;
    back_button.setPosition(back_x, back_y);
}

void settings::drawSettings() {
    settings_window.clear(sf::Color::Color(0, 128, 0));
    settings_window.draw(selection_text);
    for (int i = 0; i < 9; ++i)
        settings_window.draw(back_selection[i]);
    settings_window.draw(choice_overlay);
    settings_window.draw(back_button);
    settings_window.display();
}

void settings::chooseBack(textures& tex) {
    sf::Cursor cursor;
    sf::Vector2f mouse_position = settings_window.mapPixelToCoords(sf::Mouse::getPosition(settings_window));
    for (int i = 0; i < 9; ++i) {
        if (back_selection[i].getGlobalBounds().contains(mouse_position)) {
            back_selection[i].setColor(sf::Color::White);
            if (sf::Mouse::isButtonPressed((sf::Mouse::Left))) {
                back_selection[i].setColor(sf::Color::White);
                tex.setBackTextureChoice(i);
                choice_overlay.setPosition(back_selection[i].getPosition());
            }
        }
    }

    if (back_button.getGlobalBounds().contains(mouse_position)) {
        back_button.setFillColor(sf::Color::Black);
        if (sf::Mouse::isButtonPressed((sf::Mouse::Left)))
            menu_ref.run_menu(tex);
    } else
        back_button.setFillColor(sf::Color::White);
}