#include "menu.h"
#include <iostream>

#define windowWidth 1280
#define windowHeight 720

menu::menu(sf::RenderWindow& window, textures& tex) : menu_window(window) {
    setMenu(tex);
    run_menu(tex);
}

void menu::run_menu(textures& tex) {
    while (menu_window.isOpen()) {
        sf::Event event;
        while (menu_window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                menu_window.close();
        }
        drawMenu();
        userInputMenu(tex);
    }
}

void menu::setMenu(textures& tex) {
    if (!menu_font.loadFromFile("files/ms-sans-serif.ttf"))
        throw "Can't find font.";
  
    //title
    title_text.setFont(menu_font);
    title_text.setString("SOLITAIRE");
    title_text.setCharacterSize(100);
    title_text.setFillColor(sf::Color::White);
    sf::FloatRect title_bounds = title_text.getLocalBounds();
    float title_x = (menu_window.getSize().x - title_bounds.width) / 2; //centering text
    title_text.setPosition(title_x - 50.f, 160.f);

    //icon
    icon.setTexture(tex.loadIcon());
    icon.setPosition(title_x + 440.f, 173.f);
    icon.setScale(3.f, 3.f);

    //play 
    play_text.setFont(menu_font);
    play_text.setString("PLAY");
    play_text.setCharacterSize(50);
    play_text.setFillColor(sf::Color::White);
    sf::FloatRect play_bounds = play_text.getLocalBounds();
    float playX = (menu_window.getSize().x - play_bounds.width) / 2; //centering text
    play_text.setPosition(playX, 320.f);

    //settings
    settings_text.setFont(menu_font);
    settings_text.setString("SETTINGS");
    settings_text.setCharacterSize(50);
    settings_text.setFillColor(sf::Color::White);
    sf::FloatRect settings_bounds = settings_text.getLocalBounds();
    float settingsX = (menu_window.getSize().x - settings_bounds.width) / 2; //centering text
    settings_text.setPosition(settingsX, 400.f);

    //quit
    quit_text.setFont(menu_font);
    quit_text.setString("QUIT");
    quit_text.setCharacterSize(50);
    quit_text.setFillColor(sf::Color::White);
    sf::FloatRect quit_bounds = quit_text.getLocalBounds();
    float quitX = (menu_window.getSize().x - quit_bounds.width) / 2; //centering text
    quit_text.setPosition(quitX, 480.f);
}

void menu::drawMenu() {
    menu_window.clear(sf::Color::Color(0, 128, 0));
    menu_window.draw(title_text);
    menu_window.draw(play_text);
    menu_window.draw(settings_text);
    menu_window.draw(quit_text);
    menu_window.draw(icon);
    menu_window.display();
}

void menu::userInputMenu(textures& tex) {
    sf::Cursor cursor;
    sf::Vector2f mouse_position = menu_window.mapPixelToCoords(sf::Mouse::getPosition(menu_window));
    if (play_text.getGlobalBounds().contains(mouse_position)) {
        play_text.setFillColor(sf::Color::Black);
        if (sf::Mouse::isButtonPressed((sf::Mouse::Left)))
            game g(menu_window,tex, *this);
    } else
        play_text.setFillColor(sf::Color::White);
 
    if (settings_text.getGlobalBounds().contains(mouse_position)) {
        settings_text.setFillColor(sf::Color::Black);
        if (sf::Mouse::isButtonPressed((sf::Mouse::Left)))
            settings s(menu_window, tex, *this);
    } else 
        settings_text.setFillColor(sf::Color::White);

    if (quit_text.getGlobalBounds().contains(mouse_position)) {
        quit_text.setFillColor(sf::Color::Black);
        if (sf::Mouse::isButtonPressed((sf::Mouse::Left)))
            menu_window.close();
    } else
        quit_text.setFillColor(sf::Color::White);
}
