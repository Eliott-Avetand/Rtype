// ********************************************************************************* //
//      __                        __   _  __      _____  __              __ _        //
//     / /   ____   __  __ _____ / /_ (_)/ /__   / ___/ / /_ __  __ ____/ /(_)____   //
//    / /   / __ \ / / / // ___// __// // //_/   \__ \ / __// / / // __  // // __ \  //
//   / /___/ /_/ // /_/ /(__  )/ /_ / // ,<     ___/ // /_ / /_/ // /_/ // // /_/ /  //
//  /_____/\____/ \__,_//____/ \__//_//_/|_|   /____/ \__/ \__,_/ \__,_//_/ \____/   //
//                                                                                   //
// ********************************************************************************* //
//                                                                                   //
//  Project:         R-Type                                                          //
//  File:            MainMenu.cpp                                                    //
//  Description:     File for the MainMenu Class                                     //
//                                                                                   //
// ********************************************************************************* //

#include <iostream>

#include "Credits.hpp"
#include "Errors.hpp"

rtype::client::Credits *rtype::client::Credits::_creditsInstance{nullptr};
std::mutex rtype::client::Credits::_mutex;

void rtype::client::Credits::init(rtype::engine::Engine* engine)
{
    std::cout << "Initializing the Credits class..." << std::endl;

    engine->options.music.changeMusic("client/assets/musics/levels_musics/credits.ogg");

    this->_creditsInstance = this;

    this->_background.toggleAsteroidsSpawn(true);
    this->_background.manageAsteroidsSpawnTime(0.1, 1.5);
    this->_background.togglePlanetsSpawn(true);
    this->_background.toggleBackgroundMove(true);
    this->_background.managePlanetsSpawnTime(20, 40);

    // Programmeur : nous
    std::vector<std::string> str = {"Programmers", "Alexis Devloo", "Eliott Avetand", "Simon Auduberteau", "Anthonoire Becarne", "Gaygoire Biendine", "Eleonore Sycz"};
    std::vector<std::string> font = {FONT_PATH, FONT_PATH, FONT_PATH, FONT_PATH, FONT_PATH, FONT_PATH, FONT_PATH};
    std::vector<int> size = {TITLE_SIZE, TEXT_SIZE, TEXT_SIZE, TEXT_SIZE, TEXT_SIZE, TEXT_SIZE, TEXT_SIZE};
    std::vector<sf::Color> color = {sf::Color::White, sf::Color::White, sf::Color::White, sf::Color::White, sf::Color::White, sf::Color::White, sf::Color::White};

    float beginPos = 900;

    for (int i = 0; i < str.size(); i++) {
        if (i >= 1)
            this->_programmers.push_back(std::make_unique<rtype::client::utilities::CustomText>(str[i], font[i], size[i], color[i], sf::Vector2f(engine->options.getWindowWidth() / 2, beginPos + 75 + (i * 60))));
        else
            this->_programmers.push_back(std::make_unique<rtype::client::utilities::CustomText>(str[i], font[i], size[i], color[i], sf::Vector2f(engine->options.getWindowWidth() / 2, beginPos)));
    }

    // Producteur : Epitech
    str.clear();
    font.clear();
    size.clear();
    color.clear();
    str = {"Producer", "Epitech"};
    font = {FONT_PATH, FONT_PATH};
    size = {TITLE_SIZE, TEXT_SIZE};
    color = {sf::Color::White, sf::Color::White};
    beginPos = this->_programmers.back().get()->getPosition().y + 175;

    for (int i = 0; i < str.size(); i++) {
        if (i >= 1)
            this->_producer.push_back(std::make_unique<rtype::client::utilities::CustomText>(str[i], font[i], size[i], color[i], sf::Vector2f(engine->options.getWindowWidth() / 2, beginPos + 75 + (i * 60))));
        else
            this->_producer.push_back(std::make_unique<rtype::client::utilities::CustomText>(str[i], font[i], size[i], color[i], sf::Vector2f(engine->options.getWindowWidth() / 2, beginPos)));
    }

    // Directeur Artistique : Alexis Devloo

    str.clear();
    font.clear();
    size.clear();
    color.clear();
    str = {"Art Director", "Alexis Devloo"};
    font = {FONT_PATH, FONT_PATH};
    size = {TITLE_SIZE, TEXT_SIZE};
    color = {sf::Color::White, sf::Color::White};
    beginPos = this->_producer.back().get()->getPosition().y + 175;

    for (int i = 0; i < str.size(); i++) {
        if (i >= 1)
            this->_artDirector.push_back(std::make_unique<rtype::client::utilities::CustomText>(str[i], font[i], size[i], color[i], sf::Vector2f(engine->options.getWindowWidth() / 2, beginPos + 75 + (i * 60))));
        else
            this->_artDirector.push_back(std::make_unique<rtype::client::utilities::CustomText>(str[i], font[i], size[i], color[i], sf::Vector2f(engine->options.getWindowWidth() / 2, beginPos)));
    }

    // Musique : Alexis Devloo

    str.clear();
    font.clear();
    size.clear();
    color.clear();
    str = {"Music", "Alexis Devloo"};
    font = {FONT_PATH, FONT_PATH};
    size = {TITLE_SIZE, TEXT_SIZE};
    color = {sf::Color::White, sf::Color::White};
    beginPos = this->_artDirector.back().get()->getPosition().y + 175;

    for (int i = 0; i < str.size(); i++) {
        if (i >= 1)
            this->_music.push_back(std::make_unique<rtype::client::utilities::CustomText>(str[i], font[i], size[i], color[i], sf::Vector2f(engine->options.getWindowWidth() / 2, beginPos + 75 + (i * 60))));
        else
            this->_music.push_back(std::make_unique<rtype::client::utilities::CustomText>(str[i], font[i], size[i], color[i], sf::Vector2f(engine->options.getWindowWidth() / 2, beginPos)));
    }

    // Graphiste : Alexis Devloo

    str.clear();
    font.clear();
    size.clear();
    color.clear();
    str = {"Designer", "Alexis Devloo"};
    font = {FONT_PATH, FONT_PATH};
    size = {TITLE_SIZE, TEXT_SIZE};
    color = {sf::Color::White, sf::Color::White};
    beginPos = this->_music.back().get()->getPosition().y + 175;

    for (int i = 0; i < str.size(); i++) {
        if (i >= 1)
            this->_graphicDesigner.push_back(std::make_unique<rtype::client::utilities::CustomText>(str[i], font[i], size[i], color[i], sf::Vector2f(engine->options.getWindowWidth() / 2, beginPos + 75 + (i * 60))));
        else
            this->_graphicDesigner.push_back(std::make_unique<rtype::client::utilities::CustomText>(str[i], font[i], size[i], color[i], sf::Vector2f(engine->options.getWindowWidth() / 2, beginPos)));
    }

    // Animateur : Alexis Devloo / Eliott Avetand

    str.clear();
    font.clear();
    size.clear();
    color.clear();
    str = {"Animator", "Alexis Devloo", "Eliott Avetand"};
    font = {FONT_PATH, FONT_PATH, FONT_PATH};
    size = {TITLE_SIZE, TEXT_SIZE, TEXT_SIZE};
    color = {sf::Color::White, sf::Color::White, sf::Color::White};
    beginPos = this->_graphicDesigner.back().get()->getPosition().y + 175;

    for (int i = 0; i < str.size(); i++) {
        if (i >= 1)
            this->_animator.push_back(std::make_unique<rtype::client::utilities::CustomText>(str[i], font[i], size[i], color[i], sf::Vector2f(engine->options.getWindowWidth() / 2, beginPos + 75 + (i * 60))));
        else
            this->_animator.push_back(std::make_unique<rtype::client::utilities::CustomText>(str[i], font[i], size[i], color[i], sf::Vector2f(engine->options.getWindowWidth() / 2, beginPos)));
    }

    // Game Designer : Alexis Devloo / Anthony Becarne / Grégory Biendine

    str.clear();
    font.clear();
    size.clear();
    color.clear();
    str = {"Game Designer", "Alexis Devloo", "Anthony Becarne", "Gregoire Biendine"};
    font = {FONT_PATH, FONT_PATH, FONT_PATH, FONT_PATH};
    size = {TITLE_SIZE, TEXT_SIZE, TEXT_SIZE, TEXT_SIZE};
    color = {sf::Color::White, sf::Color::White, sf::Color::White, sf::Color::White};
    beginPos = this->_animator.back().get()->getPosition().y + 175;

    for (int i = 0; i < str.size(); i++) {
        if (i >= 1)
            this->_gameDesigner.push_back(std::make_unique<rtype::client::utilities::CustomText>(str[i], font[i], size[i], color[i], sf::Vector2f(engine->options.getWindowWidth() / 2, beginPos + 75 + (i * 60))));
        else
            this->_gameDesigner.push_back(std::make_unique<rtype::client::utilities::CustomText>(str[i], font[i], size[i], color[i], sf::Vector2f(engine->options.getWindowWidth() / 2, beginPos)));
    }
    // Directeur de Projet : Eliott Avetand

    str.clear();
    font.clear();
    size.clear();
    color.clear();
    str = {"Project Director", "Eliott Avetand"};
    font = {FONT_PATH, FONT_PATH};
    size = {TITLE_SIZE, TEXT_SIZE};
    color = {sf::Color::White, sf::Color::White};
    beginPos = this->_gameDesigner.back().get()->getPosition().y + 175;

    for (int i = 0; i < str.size(); i++) {
        if (i >= 1)
            this->_projectDirector.push_back(std::make_unique<rtype::client::utilities::CustomText>(str[i], font[i], size[i], color[i], sf::Vector2f(engine->options.getWindowWidth() / 2, beginPos + 75 + (i * 60))));
        else
            this->_projectDirector.push_back(std::make_unique<rtype::client::utilities::CustomText>(str[i], font[i], size[i], color[i], sf::Vector2f(engine->options.getWindowWidth() / 2, beginPos)));
    }

    // Directeur Technique : Augustin Mistral / Adam Cyfka

    str.clear();
    font.clear();
    size.clear();
    color.clear();
    str = {"Technical Director", "Augustin Mistral", "Adam Cyfka"};
    font = {FONT_PATH, FONT_PATH, FONT_PATH};
    size = {TITLE_SIZE - 20, TEXT_SIZE, TEXT_SIZE};
    color = {sf::Color::White, sf::Color::White, sf::Color::White};
    beginPos = this->_projectDirector.back().get()->getPosition().y + 175;

    for (int i = 0; i < str.size(); i++) {
        if (i >= 1)
            this->_technicalDirector.push_back(std::make_unique<rtype::client::utilities::CustomText>(str[i], font[i], size[i], color[i], sf::Vector2f(engine->options.getWindowWidth() / 2, beginPos + 75 + (i * 60))));
        else
            this->_technicalDirector.push_back(std::make_unique<rtype::client::utilities::CustomText>(str[i], font[i], size[i], color[i], sf::Vector2f(engine->options.getWindowWidth() / 2, beginPos)));
    }

    // Network Designer : Simon Auduberteau / Anthony Becarne

    str.clear();
    font.clear();
    size.clear();
    color.clear();
    str = {"Network Designer", "Saymon Auduberteau", "Anthony Becarne"};
    font = {FONT_PATH, FONT_PATH, FONT_PATH};
    size = {TITLE_SIZE, TEXT_SIZE, TEXT_SIZE};
    color = {sf::Color::White, sf::Color::White, sf::Color::White};
    beginPos = this->_technicalDirector.back().get()->getPosition().y + 175;

    for (int i = 0; i < str.size(); i++) {
        if (i >= 1)
            this->_networkDesigner.push_back(std::make_unique<rtype::client::utilities::CustomText>(str[i], font[i], size[i], color[i], sf::Vector2f(engine->options.getWindowWidth() / 2, beginPos + 75 + (i * 60))));
        else
            this->_networkDesigner.push_back(std::make_unique<rtype::client::utilities::CustomText>(str[i], font[i], size[i], color[i], sf::Vector2f(engine->options.getWindowWidth() / 2, beginPos)));
    }

    // Server Conceptor : Simon Auduberteau / Anthony Becarne

    str.clear();
    font.clear();
    size.clear();
    color.clear();
    str = {"Server Conceptor", "Simon Auduberteau", "Anthony Becarne"};
    font = {FONT_PATH, FONT_PATH, FONT_PATH};
    size = {TITLE_SIZE, TEXT_SIZE, TEXT_SIZE};
    color = {sf::Color::White, sf::Color::White, sf::Color::White};
    beginPos = this->_networkDesigner.back().get()->getPosition().y + 175;

    for (int i = 0; i < str.size(); i++) {
        if (i >= 1)
            this->_serverConceptor.push_back(std::make_unique<rtype::client::utilities::CustomText>(str[i], font[i], size[i], color[i], sf::Vector2f(engine->options.getWindowWidth() / 2, beginPos + 75 + (i * 60))));
        else
            this->_serverConceptor.push_back(std::make_unique<rtype::client::utilities::CustomText>(str[i], font[i], size[i], color[i], sf::Vector2f(engine->options.getWindowWidth() / 2, beginPos)));
    }

    // Engine Conceptor : Eliott Avetand

    str.clear();
    font.clear();
    size.clear();
    color.clear();
    str = {"Engine Conceptor", "Eliott Avetand"};
    font = {FONT_PATH, FONT_PATH};
    size = {TITLE_SIZE, TEXT_SIZE};
    color = {sf::Color::White, sf::Color::White};
    beginPos = this->_serverConceptor.back().get()->getPosition().y + 175;

    for (int i = 0; i < str.size(); i++) {
        if (i >= 1)
            this->_engineConceptor.push_back(std::make_unique<rtype::client::utilities::CustomText>(str[i], font[i], size[i], color[i], sf::Vector2f(engine->options.getWindowWidth() / 2, beginPos + 75 + (i * 60))));
        else
            this->_engineConceptor.push_back(std::make_unique<rtype::client::utilities::CustomText>(str[i], font[i], size[i], color[i], sf::Vector2f(engine->options.getWindowWidth() / 2, beginPos)));
    }

    // Level Designer : Alexis Devloo / Grégoire Biendine

    str.clear();
    font.clear();
    size.clear();
    color.clear();
    str = {"Level Designer", "Alexis Devloo", "Gregoire Biendine"};
    font = {FONT_PATH, FONT_PATH, FONT_PATH};
    size = {TITLE_SIZE, TEXT_SIZE, TEXT_SIZE};
    color = {sf::Color::White, sf::Color::White, sf::Color::White};
    beginPos = this->_engineConceptor.back().get()->getPosition().y + 175;

    for (int i = 0; i < str.size(); i++) {
        if (i >= 1)
            this->_levelDesigner.push_back(std::make_unique<rtype::client::utilities::CustomText>(str[i], font[i], size[i], color[i], sf::Vector2f(engine->options.getWindowWidth() / 2, beginPos + 75 + (i * 60))));
        else
            this->_levelDesigner.push_back(std::make_unique<rtype::client::utilities::CustomText>(str[i], font[i], size[i], color[i], sf::Vector2f(engine->options.getWindowWidth() / 2, beginPos)));
    }

    // Documentation : Eléonore Sycz

    str.clear();
    font.clear();
    size.clear();
    color.clear();
    str = {"Documentation", "Eleonore Sycz", "Anthony Becarne"};
    font = {FONT_PATH, FONT_PATH, FONT_PATH};
    size = {TITLE_SIZE, TEXT_SIZE, TEXT_SIZE};
    color = {sf::Color::White, sf::Color::White, sf::Color::White};
    beginPos = this->_levelDesigner.back().get()->getPosition().y + 175;

    for (int i = 0; i < str.size(); i++) {
        if (i >= 1)
            this->_documentation.push_back(std::make_unique<rtype::client::utilities::CustomText>(str[i], font[i], size[i], color[i], sf::Vector2f(engine->options.getWindowWidth() / 2, beginPos + 75 + (i * 60))));
        else
            this->_documentation.push_back(std::make_unique<rtype::client::utilities::CustomText>(str[i], font[i], size[i], color[i], sf::Vector2f(engine->options.getWindowWidth() / 2, beginPos)));
    }

    // Testeur : Eléonore Sycz / Anthony Becarne / Grégoire Biendine / Eliott Avetand / Augustin Mistral / Adam Cyfka / Simon Auduberteau / Alexis Devloo

    str.clear();
    font.clear();
    size.clear();
    color.clear();
    str = {"Tester", "Eleonore Sycz", "Anthony Becarne", "Gaygoire Biendine", "Eliott Avetand", "Alexos Devskii", "Simon Auduberteau", "Augustin Mistral", "Adam Cyfka"};
    font = {FONT_PATH, FONT_PATH, FONT_PATH, FONT_PATH, FONT_PATH, FONT_PATH, FONT_PATH, FONT_PATH, FONT_PATH};
    size = {TITLE_SIZE, TEXT_SIZE, TEXT_SIZE, TEXT_SIZE, TEXT_SIZE, TEXT_SIZE, TEXT_SIZE, TEXT_SIZE, TEXT_SIZE};
    color = {sf::Color::White, sf::Color::White, sf::Color::White, sf::Color::White, sf::Color::White, sf::Color::White, sf::Color::White, sf::Color::White, sf::Color::White};
    beginPos = this->_documentation.back().get()->getPosition().y + 175;

    for (int i = 0; i < str.size(); i++) {
        if (i >= 1)
            this->_tester.push_back(std::make_unique<rtype::client::utilities::CustomText>(str[i], font[i], size[i], color[i], sf::Vector2f(engine->options.getWindowWidth() / 2, beginPos + 75 + (i * 60))));
        else
            this->_tester.push_back(std::make_unique<rtype::client::utilities::CustomText>(str[i], font[i], size[i], color[i], sf::Vector2f(engine->options.getWindowWidth() / 2, beginPos)));
    }

    // Traductor : Google Translate

    str.clear();
    font.clear();
    size.clear();
    color.clear();
    str = {"Traductor", "Google Translate"};
    font = {FONT_PATH, FONT_PATH};
    size = {TITLE_SIZE, TEXT_SIZE};
    color = {sf::Color::White, sf::Color::White};
    beginPos = this->_tester.back().get()->getPosition().y + 175;

    for (int i = 0; i < str.size(); i++) {
        if (i >= 1)
            this->_traductor.push_back(std::make_unique<rtype::client::utilities::CustomText>(str[i], font[i], size[i], color[i], sf::Vector2f(engine->options.getWindowWidth() / 2, beginPos + 75 + (i * 60))));
        else
            this->_traductor.push_back(std::make_unique<rtype::client::utilities::CustomText>(str[i], font[i], size[i], color[i], sf::Vector2f(engine->options.getWindowWidth() / 2, beginPos)));
    }

    // Mentions honorables
    // Language : C++
    // Librairy : SFML / Boost
    // IDE : Visual Studio

    std::cout << "Loading logo..." << std::endl;
    this->_logo.push_back(std::make_unique<rtype::client::utilities::StaticSprite>(CPP_TEXTURE, sf::Vector2f(engine->options.getWindowWidth() / 2, 0)));
    this->_logo.back().get()->setScale(0.15f, 0.15f);

    this->_logo.push_back(std::make_unique<rtype::client::utilities::StaticSprite>(SFML_TEXTURE, sf::Vector2f(engine->options.getWindowWidth() / 2, 0)));
    this->_logo.back().get()->setScale(0.25f, 0.25f);

    this->_logo.push_back(std::make_unique<rtype::client::utilities::StaticSprite>(BOOST_TEXTURE, sf::Vector2f(engine->options.getWindowWidth() / 2, 0)));

    this->_logo.push_back(std::make_unique<rtype::client::utilities::StaticSprite>(VISUALSTUDIO_TEXTURE, sf::Vector2f(engine->options.getWindowWidth() / 2, 0)));
    this->_logo.back().get()->setScale(0.10f, 0.10f);

    this->_logo.push_back(std::make_unique<rtype::client::utilities::StaticSprite>(GITHUB_TEXTURE, sf::Vector2f(engine->options.getWindowWidth() / 2, 0)));
    this->_logo.back().get()->setScale(0.10f, 0.10f);

    this->_logo.push_back(std::make_unique<rtype::client::utilities::StaticSprite>(TRELLO_TEXTURE, sf::Vector2f(engine->options.getWindowWidth() / 2, 0)));
    this->_logo.back().get()->setScale(0.08f, 0.08f);

    this->_logo.push_back(std::make_unique<rtype::client::utilities::StaticSprite>(DISCORD_TEXTURE, sf::Vector2f(engine->options.getWindowWidth() / 2, 0.0f)));
    this->_logo.back().get()->setScale(0.50f, 0.50f);

    this->_logo.push_back(std::make_unique<rtype::client::utilities::StaticSprite>(POWERPOINT_TEXTURE, sf::Vector2f(engine->options.getWindowWidth() / 2, 0.0f)));
    this->_logo.back().get()->setScale(0.50f, 0.50f);
    std::cout << "Logo loaded" << std::endl;

    str.clear();
    font.clear();
    size.clear();
    color.clear();
    str = {"Honorable Mentions", "C++", "SFML", "Boost", "Visual Studio", "Git", "Trello", "Discord", "PowerPoint"};
    font = {FONT_PATH, FONT_PATH, FONT_PATH, FONT_PATH, FONT_PATH, FONT_PATH, FONT_PATH, FONT_PATH, FONT_PATH};
    size = {TITLE_SIZE - 20, TEXT_SIZE, TEXT_SIZE, TEXT_SIZE, TEXT_SIZE, TEXT_SIZE, TEXT_SIZE, TEXT_SIZE, TEXT_SIZE};
    color = {sf::Color::White, sf::Color::White, sf::Color::White, sf::Color::White, sf::Color::White, sf::Color::White, sf::Color::White, sf::Color::White, sf::Color::White};
    beginPos = this->_traductor.back().get()->getPosition().y + 175;

    std::vector<sf::Vector2f> pos;

    this->_mentionsHonorables.push_back(std::make_unique<rtype::client::utilities::CustomText>(str[0], font[0], size[0], color[0], sf::Vector2f(engine->options.getWindowWidth() / 2, beginPos)));
    pos.push_back(sf::Vector2f(this->_logo[0]->getPosition().x, beginPos + 100 + (1 * 60)));
    this->_logo[0]->setPosition(sf::Vector2f(pos.back().x, pos.back().y));
    this->_mentionsHonorables.push_back(std::make_unique<rtype::client::utilities::CustomText>(str[1], font[1], size[1], color[1],
    sf::Vector2f(engine->options.getWindowWidth() / 2, this->_logo[0]->getPosition().y + this->_logo[0]->getSprite().getGlobalBounds().height / 2 + 40)));

    pos.push_back(sf::Vector2f(this->_logo[1]->getPosition().x, this->_mentionsHonorables.back()->getPosition().y + 100));
    this->_logo[1]->setPosition(sf::Vector2f(pos.back().x, pos.back().y));
    this->_mentionsHonorables.push_back(std::make_unique<rtype::client::utilities::CustomText>(str[2], font[2], size[2], color[2],
    sf::Vector2f(engine->options.getWindowWidth() / 2, this->_logo[1]->getPosition().y + this->_logo[1]->getSprite().getGlobalBounds().height / 2 + 40)));

    pos.push_back(sf::Vector2f(this->_logo[2]->getPosition().x, this->_mentionsHonorables.back()->getPosition().y + 100));
    this->_logo[2]->setPosition(sf::Vector2f(pos.back().x, pos.back().y));
    this->_mentionsHonorables.push_back(std::make_unique<rtype::client::utilities::CustomText>(str[3], font[3], size[3], color[3],
    sf::Vector2f(engine->options.getWindowWidth() / 2, this->_logo[2]->getPosition().y + this->_logo[2]->getSprite().getGlobalBounds().height / 2 + 40)));

    pos.push_back(sf::Vector2f(this->_logo[3]->getPosition().x, this->_mentionsHonorables.back()->getPosition().y + 100));
    this->_logo[3]->setPosition(sf::Vector2f(pos.back().x, pos.back().y));
    this->_mentionsHonorables.push_back(std::make_unique<rtype::client::utilities::CustomText>(str[4], font[4], size[4], color[4],
    sf::Vector2f(engine->options.getWindowWidth() / 2, this->_logo[3]->getPosition().y + this->_logo[3]->getSprite().getGlobalBounds().height / 2 + 40)));

    pos.push_back(sf::Vector2f(this->_logo[3]->getPosition().x, this->_mentionsHonorables.back()->getPosition().y + 100));
    this->_logo[4]->setPosition(sf::Vector2f(pos.back().x, pos.back().y));
    this->_mentionsHonorables.push_back(std::make_unique<rtype::client::utilities::CustomText>(str[5], font[5], size[5], color[5],
    sf::Vector2f(engine->options.getWindowWidth() / 2, this->_logo[4]->getPosition().y + this->_logo[4]->getSprite().getGlobalBounds().height / 2 + 40)));

    pos.push_back(sf::Vector2f(this->_logo[3]->getPosition().x, this->_mentionsHonorables.back()->getPosition().y + 100));
    this->_logo[5]->setPosition(sf::Vector2f(pos.back().x, pos.back().y));
    this->_mentionsHonorables.push_back(std::make_unique<rtype::client::utilities::CustomText>(str[6], font[6], size[6], color[6],
    sf::Vector2f(engine->options.getWindowWidth() / 2, this->_logo[5]->getPosition().y + this->_logo[5]->getSprite().getGlobalBounds().height / 2 + 40)));

    pos.push_back(sf::Vector2f(this->_logo[3]->getPosition().x, this->_mentionsHonorables.back()->getPosition().y + 100));
    this->_logo[6]->setPosition(sf::Vector2f(pos.back().x, pos.back().y));
    this->_mentionsHonorables.push_back(std::make_unique<rtype::client::utilities::CustomText>(str[7], font[7], size[7], color[7],
    sf::Vector2f(engine->options.getWindowWidth() / 2, this->_logo[6]->getPosition().y + this->_logo[6]->getSprite().getGlobalBounds().height / 2 + 40)));

    pos.push_back(sf::Vector2f(this->_logo[3]->getPosition().x, this->_mentionsHonorables.back()->getPosition().y + 100));
    this->_logo[7]->setPosition(sf::Vector2f(pos.back().x, pos.back().y));
    this->_mentionsHonorables.push_back(std::make_unique<rtype::client::utilities::CustomText>(str[8], font[8], size[8], color[8],
    sf::Vector2f(engine->options.getWindowWidth() / 2, this->_logo[7]->getPosition().y + this->_logo[7]->getSprite().getGlobalBounds().height / 2 + 40)));

    // Studio : Loustik Studio

    str.clear();
    font.clear();
    size.clear();
    color.clear();
    str = {"Game Studio", "Loustik Studio"};
    font = {FONT_PATH, FONT_PATH};
    size = {TITLE_SIZE, TEXT_SIZE};
    color = {sf::Color::White, sf::Color::White};
    beginPos = this->_mentionsHonorables.back().get()->getPosition().y + 175;

    for (int i = 0; i < str.size(); i++) {
        if (i >= 1)
            this->_studio.push_back(std::make_unique<rtype::client::utilities::CustomText>(str[i], font[i], size[i], color[i], sf::Vector2f(engine->options.getWindowWidth() / 2, beginPos + 75 + (i * 60))));
        else
            this->_studio.push_back(std::make_unique<rtype::client::utilities::CustomText>(str[i], font[i], size[i], color[i], sf::Vector2f(engine->options.getWindowWidth() / 2, beginPos)));
    }

    this->_speed = 3;

    sf::Vector2f windowSize(engine->options.getWindowWidth(), engine->options.getWindowHeight());
    this->_backgroundBlack.setSize(sf::Vector2f(windowSize.x, windowSize.y));
    this->_backgroundBlack.setOrigin(sf::Vector2f(windowSize.x / 2, windowSize.y / 2));
    this->_backgroundBlack.setFillColor(sf::Color(0, 0, 0, 0));
    this->_backgroundBlack.setPosition(sf::Vector2f(windowSize.x / 2, windowSize.y / 2));
    std::cout << "Credits class loaded." << std::endl;
}

void rtype::client::Credits::cleanup()
{
    std::cout << "Cleaning the MainMenu class..." << std::endl;
    this->releaseInstance();
    std::cout << "MainMenu class cleaned up." << std::endl;
}

void rtype::client::Credits::pause()
{
}

void rtype::client::Credits::resume()
{
}

void rtype::client::Credits::networkReceiver(rtype::engine::Engine *engine)
{
}

void rtype::client::Credits::networkSender(rtype::engine::Engine *engine)
{
}

void rtype::client::Credits::processInput(sf::Event event, rtype::engine::Engine *engine)
{
    this->_background.eventUpdate(event, engine);

    if (sf::Joystick::isConnected(0)) {
        if (event.type == sf::Event::JoystickButtonPressed && event.joystickButton.button == rtype::action::joystickButton::START) {
            engine->options.music.setMusicVolume(100);
            engine->changeState(rtype::client::BeginingScreen::getInstance());
        }


        if (event.joystickMove.axis == sf::Joystick::Y && (event.joystickMove.position >= (int)rtype::action::joystickAxis::UP && event.joystickMove.position < -50)) {
            engine->options.music.changePitch(3);
            this->_speed = -20;
        }

        if (event.joystickMove.axis == sf::Joystick::Y && (event.joystickMove.position <= (int)rtype::action::joystickAxis::DOWN && event.joystickMove.position > 50)) {
            engine->options.music.changePitch(3);
            this->_speed = 20;
        }

        if (event.joystickMove.axis == sf::Joystick::Y && (event.joystickMove.position > -20 && event.joystickMove.position < 20)) {
            engine->options.music.changePitch(1);
            this->_speed = 3;
        }

    }
    else {
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
            engine->options.music.setMusicVolume(100);
            engine->changeState(rtype::client::BeginingScreen::getInstance());
        }

        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Down) {
            engine->options.music.changePitch(3);
            this->_speed = 20;
        } else if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Down) {
            engine->options.music.changePitch(1);
            this->_speed = 3;
        }

        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Up) {
            engine->options.music.changePitch(3);
            this->_speed = -20;
        } else if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Up) {
            engine->options.music.changePitch(1);
            this->_speed = 3;
        }
    }
}

void rtype::client::Credits::update(rtype::engine::Engine *engine)
{
    if (this->_studio.back()->getPosition().y < -300) {
        engine->options.music.setMusicVolume(100);
        engine->changeState(rtype::client::BeginingScreen::getInstance());
    }

    this->_background.update(engine);

    for (auto &programmer : this->_programmers)
        programmer->move(sf::Vector2f(0, -(this->_speed)));

    for (auto &producer : this->_producer)
        producer->move(sf::Vector2f(0, -(this->_speed)));
    
    for (auto &studio : this->_studio)
        studio->move(sf::Vector2f(0, -(this->_speed)));
    
    for (auto &artDirector : this->_artDirector)
        artDirector->move(sf::Vector2f(0, -(this->_speed)));
    
    for (auto &music : this->_music)
        music->move(sf::Vector2f(0, -(this->_speed)));

    for (auto &graphicDesigner : this->_graphicDesigner)
        graphicDesigner->move(sf::Vector2f(0, -(this->_speed)));
    
    for (auto &animator : this->_animator)
        animator->move(sf::Vector2f(0, -(this->_speed)));
    
    for (auto &gameDesigner : this->_gameDesigner)
        gameDesigner->move(sf::Vector2f(0, -(this->_speed)));

    for (auto &projectDirector : this->_projectDirector)
        projectDirector->move(sf::Vector2f(0, -(this->_speed)));
    
    for (auto &technicalDirector : this->_technicalDirector)
        technicalDirector->move(sf::Vector2f(0, -(this->_speed)));

    for (auto &networdDesigner : this->_networkDesigner)
        networdDesigner->move(sf::Vector2f(0, -(this->_speed)));
    
    for (auto &serverConceptor : this->_serverConceptor)
        serverConceptor->move(sf::Vector2f(0, -(this->_speed)));
    
    for (auto &engineConceptor : this->_engineConceptor)
        engineConceptor->move(sf::Vector2f(0, -(this->_speed)));
    
    for (auto &levelDesigner : this->_levelDesigner)
        levelDesigner->move(sf::Vector2f(0, -(this->_speed)));
    
    for (auto &documentation : this->_documentation)
        documentation->move(sf::Vector2f(0, -(this->_speed)));
    
    for (auto &tester : this->_tester)
        tester->move(sf::Vector2f(0, -(this->_speed)));

    for (auto &traductor : this->_traductor)
        traductor->move(sf::Vector2f(0, -(this->_speed)));
    
    for (auto &mentionHonorable : this->_mentionsHonorables)
        mentionHonorable->move(sf::Vector2f(0, -(this->_speed)));
    
    // logo

    for (auto &logo : this->_logo)
        logo->setPosition(sf::Vector2f(logo->getPosition().x, logo->getPosition().y - (this->_speed)));

    sf::Time time = this->_backgroundClock.getElapsedTime();
    float milliseconds = time.asMilliseconds();

    sf::Color backgroundColor = this->_backgroundBlack.getFillColor();

    if (milliseconds > 20 && this->_studio.back()->getPosition().y < 500) {
        if (backgroundColor.a + _speed >= 255)
            backgroundColor.a = 255;
        else if (backgroundColor.a + _speed <= 0)
            backgroundColor.a = 0;
        else
            backgroundColor.a += _speed;
            engine->options.music.setMusicVolume(engine->options.music.getMusicVolume() - (int)(_speed / 3));
        this->_backgroundBlack.setFillColor(backgroundColor);
        this->_backgroundClock.restart();
    }
}

void rtype::client::Credits::renderLevel(rtype::engine::Engine* engine)
{
    this->_background.display(engine);

    for (auto &programmer : this->_programmers)
        programmer->draw(engine);

    for (auto &producer : this->_producer)
        producer->draw(engine);

    for (auto &studio : this->_studio)
        studio->draw(engine);

    for (auto &artDirector : this->_artDirector)
        artDirector->draw(engine);
    
    for (auto &music : this->_music)
        music->draw(engine);

    for (auto &graphicDesigner : this->_graphicDesigner)
        graphicDesigner->draw(engine);
    
    for (auto &animator : this->_animator)
        animator->draw(engine);
    
    for (auto &gameDesigner : this->_gameDesigner)
        gameDesigner->draw(engine);

    for (auto &projectDirector : this->_projectDirector)
        projectDirector->draw(engine);
    
    for (auto &technicalDirector : this->_technicalDirector)
        technicalDirector->draw(engine);

    for (auto &networdDesigner : this->_networkDesigner)
        networdDesigner->draw(engine);
    
    for (auto &serverConceptor : this->_serverConceptor)
        serverConceptor->draw(engine);
    
    for (auto &engineConceptor : this->_engineConceptor)
        engineConceptor->draw(engine);
    
    for (auto &levelDesigner : this->_levelDesigner)
        levelDesigner->draw(engine);
    
    for (auto &documentation : this->_documentation)
        documentation->draw(engine);
    
    for (auto &tester : this->_tester)
        tester->draw(engine);

    for (auto &traductor : this->_traductor)
        traductor->draw(engine);
    
    for (auto &mentionHonorable : this->_mentionsHonorables)
        mentionHonorable->draw(engine);
    // logo

    for (auto &logo : this->_logo)
        logo->draw(engine);
    engine->window.draw(_backgroundBlack);
}

void rtype::client::Credits::render(rtype::engine::Engine *engine)
{
    engine->window.clear();

    this->renderLevel(engine);

    engine->window.display();
}

void rtype::client::Credits::updateObjectsPosition(sf::Vector2f deltaSize)
{
    return;
}