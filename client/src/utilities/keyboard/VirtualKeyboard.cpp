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
//  File:            VirtualKeyboard.cpp                                            //
//  Description:     File for the VirtualKeyboard Class                             //
//                                                                                   //
// ********************************************************************************* //
#include "VirtualKeyboard.hpp"

rtype::client::utilities::VirtualKeyboard::VirtualKeyboard(rtype::client::utilities::VirtualKeyboard::keyboardPosition keyboardPos)
{
	rtype::engine::Engine* engine = rtype::engine::Engine::getInstance();

	this->_keyboardBackground.setFillColor(sf::Color(0, 0, 0, 200));
	this->_keyboardBackground.setSize(sf::Vector2f(engine->options.getWindowWidth(), engine->options.getWindowHeight() / 2.5));
	this->_keyboardBackground.setOrigin(sf::Vector2f(this->_keyboardBackground.getSize().x / 2, this->_keyboardBackground.getSize().y / 2));
	
	this->_keyboardPosition = keyboardPos;
	if (this->_keyboardPosition == rtype::client::utilities::VirtualKeyboard::TOP)
		this->_position = sf::Vector2f(engine->options.getWindowWidth() / 2, -this->_keyboardBackground.getSize().y / 2);
	else
		this->_position = sf::Vector2f(engine->options.getWindowWidth() / 2, engine->options.getWindowHeight() + this->_keyboardBackground.getSize().y / 2);
	this->_keyboardState = rtype::client::utilities::VirtualKeyboard::keyboardState::CLOSE;
	
	this->_actualFocusEventID = 0;

	this->createKeys(engine);
}

bool rtype::client::utilities::VirtualKeyboard::isHover(sf::Vector2f mousePos)
{
	sf::FloatRect bound = _keyboardBackground.getGlobalBounds();

	if (bound.contains(mousePos))
		return true;
	return false;
}

void rtype::client::utilities::VirtualKeyboard::createKeys(rtype::engine::Engine* engine)
{
	std::vector<std::vector<std::string>> allKeys = {
		{"1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "/"},
		{"A", "Z", "E", "R", "T", "Y", "U", "I", "O", "P", ">"},
		{"Q", "S", "D", "F", "G", "H", "J", "K", "L", "M", "<"},
		{"W", "X", "C", "V", "B", "N", ",", ".", ":", "!", "?"}};

	std::vector<std::vector<int>> allKeysID = {
		{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11},
		{12, 13, 14, 15, 16, 17, 18, 20, 21, 22, 23},
		{25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35},
		{36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46} };

	rtype::client::utilities::VirtualKeys keyExample(sf::Vector2f(0, 0));

	float winWidth = (float)engine->options.getWindowWidth();
	float winHeight = engine->options.getWindowHeight();


	for (int i = 0; i < allKeys.size(); i++) {
		float beginX = winWidth / 2 - ((int(allKeys[0].size() / 2) * keyExample.getBounds().width) + (int(allKeys[0].size() / 2) * keyExample.getBounds().width * 0.5));

		for (int j = 0; j < allKeys[i].size(); j++) {
			this->_allKeys.push_back(rtype::client::utilities::VirtualKeys(sf::Vector2f(0, 0), allKeysID[i][j], allKeys[i][j]));

			if (j > allKeys[i].size() / 2) {
				if (this->_keyboardPosition == rtype::client::utilities::VirtualKeyboard::keyboardPosition::TOP) {
					this->_allKeys[this->_allKeys.size() - 1].setPosition(sf::Vector2f(beginX, (this->_position.y - this->_keyboardBackground.getSize().y / 3) + (i * (keyExample.getBounds().height + keyExample.getBounds().height * 0.25))));
				}
				else {
					this->_allKeys[this->_allKeys.size() - 1].setPosition(sf::Vector2f(beginX, (this->_position.y - this->_keyboardBackground.getSize().y / 3) - (allKeys.size() - i * (keyExample.getBounds().height + keyExample.getBounds().height * 0.25))));
				}
			}
			else {
				if (this->_keyboardPosition == rtype::client::utilities::VirtualKeyboard::keyboardPosition::TOP) {
					this->_allKeys[this->_allKeys.size() - 1].setPosition(sf::Vector2f(beginX, (this->_position.y - this->_keyboardBackground.getSize().y / 3) + (i * (keyExample.getBounds().height + keyExample.getBounds().height * 0.25))));
				}
				else {
					this->_allKeys[this->_allKeys.size() - 1].setPosition(sf::Vector2f(beginX, (this->_position.y - this->_keyboardBackground.getSize().y / 3) - (allKeys.size() - i * (keyExample.getBounds().height + keyExample.getBounds().height * 0.25))));
				}
			}

			beginX += keyExample.getBounds().width + (keyExample.getBounds().width * 0.5);

			if (i != 0) {
				this->_allKeys[this->_allKeys.size() - 1].setUpElements(allKeysID[i - 1][j]);
			}

			if (i != allKeys.size() - 1) {
				this->_allKeys[this->_allKeys.size() - 1].setDownElements(allKeysID[i + 1][j]);
			}
			else {
				if (j < allKeys[0].size() / 2)
					this->_allKeys[this->_allKeys.size() - 1].setDownElements(47);
				else
					this->_allKeys[this->_allKeys.size() - 1].setDownElements(48);
			}

			if (j != 0) {
				this->_allKeys[this->_allKeys.size() - 1].setLeftElements(allKeysID[i][j - 1]);
			}

			if (j != allKeys[i].size() - 1) {
				this->_allKeys[this->_allKeys.size() - 1].setRightElements(allKeysID[i][j + 1]);
			}
		}
	}

	this->_allKeys.push_back(rtype::client::utilities::VirtualKeys(sf::Vector2f(0, 0), 47, " "));
	this->_allKeys[this->_allKeys.size() - 1].setSize(sf::Vector2f(keyExample.getBounds().width * 7, keyExample.getBounds().height));
	this->_allKeys[this->_allKeys.size() - 1].setPosition(sf::Vector2f(winWidth / 2 - this->_allKeys[this->_allKeys.size() - 1].getBounds().width / 2, (this->_position.y + this->_keyboardBackground.getSize().y / 2.8)));

	this->_allKeys[this->_allKeys.size() - 1].setRightElements(48);
	this->_allKeys[this->_allKeys.size() - 1].setUpElements(38);

	this->_allKeys.push_back(rtype::client::utilities::VirtualKeys(sf::Vector2f(0, 0), 48, "DEL"));
	this->_allKeys[this->_allKeys.size() - 1].setSize(sf::Vector2f(keyExample.getBounds().width * 4, keyExample.getBounds().height));
	this->_allKeys[this->_allKeys.size() - 1].setPosition(sf::Vector2f(winWidth / 2 + this->_allKeys[this->_allKeys.size() - 1].getBounds().width, (this->_position.y + this->_keyboardBackground.getSize().y / 2.8)));

	this->_allKeys[this->_allKeys.size() - 1].setLeftElements(47);
	this->_allKeys[this->_allKeys.size() - 1].setUpElements(44);
}

void rtype::client::utilities::VirtualKeyboard::draw(rtype::engine::Engine* engine)
{
	engine->window.draw(this->_keyboardBackground);
	for (unsigned i = 0; i < this->_allKeys.size(); i++) {
		this->_allKeys[i].draw(engine);
	}
}

void rtype::client::utilities::VirtualKeyboard::update(rtype::engine::Engine* engine)
{
	this->updateState(engine);

	this->_keyboardBackground.setPosition(this->_position);
}

bool rtype::client::utilities::VirtualKeyboard::isClosed(rtype::engine::Engine* engine)
{
	if (this->_keyboardPosition == rtype::client::utilities::VirtualKeyboard::keyboardPosition::TOP) {
		if (this->_position.y <= -this->_keyboardBackground.getSize().y / 2)
			return true;
	}
	else {
		if (this->_position.y >= engine->options.getWindowHeight() + this->_keyboardBackground.getSize().y / 2)
			return true;
	}
	return false;
}

bool rtype::client::utilities::VirtualKeyboard::isOpened(rtype::engine::Engine* engine)
{
	if (this->_keyboardPosition == rtype::client::utilities::VirtualKeyboard::keyboardPosition::TOP) {
		if (this->_position.y >= this->_keyboardBackground.getSize().y / 2)
			return true;
	}
	else {
		if (this->_position.y <= engine->options.getWindowHeight() - this->_keyboardBackground.getSize().y / 2)
			return true;
	}
	return false;
}

void rtype::client::utilities::VirtualKeyboard::updateState(rtype::engine::Engine* engine)
{
	sf::Time time = this->_moveClock.getElapsedTime();
	float milliseconds = time.asMilliseconds();

	for (unsigned i = 0; i < this->_allKeys.size(); i++) {
		if (this->_actualFocusEventID == this->_allKeys[i].getId())
			this->_allKeys[i].update(engine);
	}

	if (milliseconds > 5) {
		if (this->_keyboardState == rtype::client::utilities::VirtualKeyboard::keyboardState::OPENING) {
			if (this->_keyboardPosition == rtype::client::utilities::VirtualKeyboard::keyboardPosition::TOP) {
					this->setPosition(sf::Vector2f(this->_position.x, this->_position.y + 20));
					for (unsigned i = 0; i < this->_allKeys.size(); i++) {
						sf::Vector2f pos = this->_allKeys[i].getPosition();
						this->_allKeys[i].setPosition(sf::Vector2f(pos.x, pos.y + 20));
					}
			}
			else {
				this->setPosition(sf::Vector2f(this->_position.x, this->_position.y - 20));
				for (unsigned i = 0; i < this->_allKeys.size(); i++) {
					sf::Vector2f pos = this->_allKeys[i].getPosition();
					this->_allKeys[i].setPosition(sf::Vector2f(pos.x, pos.y - 20));
				}
			}
			
			if (this->isOpened(engine))
				this->_keyboardState = rtype::client::utilities::VirtualKeyboard::keyboardState::OPEN;
		}

		if (this->_keyboardState == rtype::client::utilities::VirtualKeyboard::keyboardState::CLOSING) {
			if (this->_keyboardPosition == rtype::client::utilities::VirtualKeyboard::keyboardPosition::TOP) {
				this->setPosition(sf::Vector2f(this->_position.x, this->_position.y - 20));
				for (unsigned i = 0; i < this->_allKeys.size(); i++) {
					sf::Vector2f pos = this->_allKeys[i].getPosition();
					this->_allKeys[i].setPosition(sf::Vector2f(pos.x, pos.y - 20));
				}
			}
			else {
				this->setPosition(sf::Vector2f(this->_position.x, this->_position.y + 20));
				for (unsigned i = 0; i < this->_allKeys.size(); i++) {
					sf::Vector2f pos = this->_allKeys[i].getPosition();
					this->_allKeys[i].setPosition(sf::Vector2f(pos.x, pos.y + 20));
				}
			}

			if (this->isClosed(engine))
				this->_keyboardState = rtype::client::utilities::VirtualKeyboard::keyboardState::CLOSE;
		}
		this->_moveClock.restart();
	}
}

void rtype::client::utilities::VirtualKeyboard::updateJoystickEvent(sf::Event event, rtype::engine::Engine* engine)
{
	int newId = 0;

	for (unsigned i = 0; i < this->_allKeys.size(); i++) {
		if (this->_actualFocusEventID == this->_allKeys[i].getId())
			newId = this->_allKeys[i].eventJoystickUpdate(event, engine);

	}

	if (newId != -1) {
		this->_actualFocusEventID = newId;
	}
	if (newId == 0)
		this->_actualFocusEventID = 1;
}

void rtype::client::utilities::VirtualKeyboard::updateEvent(sf::Event event, rtype::engine::Engine* engine)
{
	int newId = 0;

	for (unsigned i = 0; i < this->_allKeys.size(); i++)
		newId = this->_allKeys[i].eventUpdate(event, engine);

	if (newId != -1) {
		this->_actualFocusEventID = newId;
	}
	if (newId == 0)
		this->_actualFocusEventID = 1;
}

void rtype::client::utilities::VirtualKeyboard::openKeyboard(void)
{
	this->_moveClock.restart();
	this->_keyboardState = rtype::client::utilities::VirtualKeyboard::OPENING;
}

void rtype::client::utilities::VirtualKeyboard::closeKeyboard(void)
{
	this->_moveClock.restart();
	this->_keyboardState = rtype::client::utilities::VirtualKeyboard::CLOSING;
}

rtype::client::utilities::VirtualKeyboard::keyboardState rtype::client::utilities::VirtualKeyboard::getState(void)
{
	return this->_keyboardState;
}

void rtype::client::utilities::VirtualKeyboard::setPosition(sf::Vector2f position)
{
	this->_position = position;
}

std::string rtype::client::utilities::VirtualKeyboard::getLastKeyPressed(void)
{
	for (unsigned i = 0; i < this->_allKeys.size(); i++) {
		if (this->_actualFocusEventID == this->_allKeys[i].getId())
			return this->_allKeys[i].getKey();
	}
	return "";
}

rtype::client::utilities::VirtualKeyboard::keyboardState rtype::client::utilities::VirtualKeyboard::getKeyboardState(void)
{
	return this->_keyboardState;
}
