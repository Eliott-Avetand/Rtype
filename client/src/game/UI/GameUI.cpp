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
//  File:            GameUI.cpp                                                      //
//  Description:     GameUI Class definition                                         //
//                                                                                   //
// ********************************************************************************* //
#include "GameUI.hpp"

rtype::client::GameUI::GameUI()
{
	rtype::engine::Engine* engine = rtype::engine::Engine::getInstance();

	this->_actualLife = 3;

	this->_shootBar = std::make_unique<rtype::client::ShootBar>(sf::Vector2f(engine->options.getWindowWidth() / 2,
		engine->options.getWindowHeight() - engine->options.getWindowHeight() * 7 / 100));

	if (!this->_shootBar)
		throw rtype::settings::Errors("Not enough memories", "MemoryError:");

	this->createBackground(engine);
	this->createTexts(engine);
	this->createLifeSprite(engine);
}

void rtype::client::GameUI::createBackground(rtype::engine::Engine* engine)
{
	this->_background.setSize(sf::Vector2f(engine->options.getWindowWidth(), engine->options.getWindowHeight() * 10 / 100));
	this->_background.setOrigin(sf::Vector2f(engine->options.getWindowWidth() / 2, this->_background.getSize().y / 2));
	this->_background.setPosition(sf::Vector2f(engine->options.getWindowWidth() / 2, engine->options.getWindowHeight() - this->_background.getSize().y / 2));
	this->_background.setFillColor(sf::Color::Black);
}

void rtype::client::GameUI::createTexts(rtype::engine::Engine* engine)
{
	this->_beamText = std::make_unique<rtype::client::utilities::CustomText>("BEAM", FONT_PATH, 30, sf::Color::White, sf::Vector2f((engine->options.getWindowWidth() / 2) - this->_shootBar->getWidth(140) / 2,
		engine->options.getWindowHeight() - engine->options.getWindowHeight() * 7 / 100));

	if (!this->_beamText)
		throw rtype::settings::Errors("Not enough memories", "MemoryError:");

	this->_playerNumberText = std::make_unique<rtype::client::utilities::CustomText>("P1.", FONT_PATH, 30, sf::Color::White);

	if (!this->_playerNumberText)
		throw rtype::settings::Errors("Not enough memories", "MemoryError:");

	this->_playerNumberText->setOrigin(this->_playerNumberText->getLocalBounds().width / 2, this->_playerNumberText->getLocalBounds().height / 2);

	this->_playerNumberText->setPosition(sf::Vector2f((engine->options.getWindowWidth() / 2) - this->_playerNumberText->getLocalBounds().width / 2,
		engine->options.getWindowHeight() - engine->options.getWindowHeight() * 3 / 100));

	this->_scoreText = std::make_unique<rtype::client::utilities::CustomText>("0", FONT_PATH, 30, sf::Color::White);

	if (!this->_scoreText)
		throw rtype::settings::Errors("Not enough memories", "MemoryError:");

	this->_scoreText->setOrigin(0, this->_scoreText->getLocalBounds().height / 2);

	this->_scoreText->setPosition(sf::Vector2f((engine->options.getWindowWidth() / 2) + this->_scoreText->getLocalBounds().width / 2,
		engine->options.getWindowHeight() - engine->options.getWindowHeight() * 3 / 100 + 1));
}

void rtype::client::GameUI::removeLifeSprite()
{
	if (this->_lifeSprite.size() > 0)
		this->_lifeSprite.pop_back();
}

void rtype::client::GameUI::createLifeSprite(rtype::engine::Engine* engine)
{
	for (size_t i = 0; i < 3; i++)
		this->addSpriteLife(engine);
}

void rtype::client::GameUI::addSpriteLife(rtype::engine::Engine* engine)
{
	int nbrLife = this->_lifeSprite.size() + 1;

	this->_lifeSprite.push_back(rtype::client::utilities::StaticSprite(LIFE_TEXTURE, sf::Vector2f(0, 0)));

	this->_lifeSprite[this->_lifeSprite.size() - 1].setScale(1.5, 1.5);

	this->_lifeSprite[this->_lifeSprite.size() - 1].setPosition(sf::Vector2f((this->_lifeSprite[this->_lifeSprite.size() - 1].getWidth() * 1.5 * nbrLife),
		engine->options.getWindowHeight() - engine->options.getWindowHeight() * 7 / 100));
}

void rtype::client::GameUI::draw(rtype::engine::Engine* engine)
{
	engine->window.draw(this->_background);

	this->_beamText->draw(engine);
	this->_playerNumberText->draw(engine);
	this->_scoreText->draw(engine);

	for (size_t i = 0; i < this->_lifeSprite.size(); i++)
		this->_lifeSprite[i].draw(engine);

	this->_shootBar->draw(engine);
}

void rtype::client::GameUI::updateScoreText(rtype::engine::Engine* engine, rtype::actor::Actor* player)
{
	std::string scoreInText = std::to_string(player->getScore());

	this->_scoreText->setText(scoreInText);
	this->_scoreText->setOrigin(0, this->_scoreText->getLocalBounds().height / 2);
}

void rtype::client::GameUI::update(rtype::engine::Engine* engine, rtype::actor::Actor *player)
{
	int playerHealth = player->getHealth();	

	if (playerHealth < this->_actualLife) {
		while (playerHealth < this->_actualLife) {
			this->removeLifeSprite();
			this->_actualLife--;
		}
	}
	else if (playerHealth > this->_actualLife) {
		while (playerHealth > this->_actualLife) {
			this->addSpriteLife(engine);
			this->_actualLife++;
		}
	}

	this->_shootBar->update(engine, player);
	this->updateScoreText(engine, player);
}