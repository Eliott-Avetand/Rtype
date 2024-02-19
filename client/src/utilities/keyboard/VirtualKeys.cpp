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
//  File:            VirtualKeys.cpp                                            //
//  Description:     File for the VirtualKeys Class                             //
//                                                                                   //
// ********************************************************************************* //
#include "VirtualKeys.hpp"

rtype::client::utilities::VirtualKeys::VirtualKeys(sf::Vector2f position, int id, std::string key)
{
	rtype::engine::Engine* engine = rtype::engine::Engine::getInstance();

	this->_position = position;

	this->_id = id;
	this->_rightElementsID = -1;
	this->_leftElementsID = -1;
	this->_upElementsID = -1;
	this->_downElementsID = -1;

	this->_virtualKeyState = rtype::client::utilities::VirtualKeys::virtualKeyState::NORMAL;
	this->_joystickState = rtype::virtualkeysutils::ARRIVED;

	this->_keyShape.setFillColor(sf::Color::Transparent);
	this->_keyShape.setOutlineColor(sf::Color::White);
	this->_keyShape.setOutlineThickness(3);
	this->_keyShape.setSize(sf::Vector2f((engine->options.getWindowHeight() / 2.5) / 8, (engine->options.getWindowHeight() / 2.5) / 8));
	this->_keyShape.setOrigin(sf::Vector2f(this->_keyShape.getSize().x / 2, this->_keyShape.getSize().y / 2));
	this->_keyShape.setPosition(this->_position);

	this->_bounds = this->_keyShape.getGlobalBounds();

	this->_keyText = std::make_unique<rtype::client::utilities::CustomText>(key, FONT_PATH, 20, sf::Color::White, position);

	if (!this->_keyText)
		throw rtype::settings::Errors("Not enough memories", "MemoryError:");
}

bool rtype::client::utilities::VirtualKeys::isHover(sf::Vector2f mousePos)
{
	if (this->_bounds.contains(mousePos))
		return true;
	return false;
}

void rtype::client::utilities::VirtualKeys::hover(bool isSound, rtype::engine::Engine* engine)
{
	if (isSound && this->_virtualKeyState != rtype::client::utilities::VirtualKeys::virtualKeyState::HOVER) {
		engine->options.music.playHoverSound();
		this->_joystickClock.restart();
	}
	this->_virtualKeyState = rtype::client::utilities::VirtualKeys::virtualKeyState::HOVER;
	this->_keyShape.setOutlineColor(sf::Color(128, 128, 128));
	this->_keyText->setColor(sf::Color(128, 128, 128));
}

void rtype::client::utilities::VirtualKeys::released()
{
	this->_virtualKeyState = rtype::client::utilities::VirtualKeys::virtualKeyState::NORMAL;
	this->_joystickState = rtype::virtualkeysutils::joystickState::ARRIVED;
	this->_keyShape.setOutlineColor(sf::Color::White);
	this->_keyText->setColor(sf::Color::White);
}

void rtype::client::utilities::VirtualKeys::update(rtype::engine::Engine* engine)
{
	sf::Time time = this->_joystickClock.getElapsedTime();
	float milliseconds = time.asMilliseconds();

	if (this->_virtualKeyState != HOVER) {
		this->_joystickClock.restart();
		return;
	}

	if (milliseconds > 10) {
		if (this->_joystickState == rtype::virtualkeysutils::joystickState::STAY)
			this->_joystickState = rtype::virtualkeysutils::joystickState::CHANGE;
	}
}

int rtype::client::utilities::VirtualKeys::eventJoystickUpdate(sf::Event event, rtype::engine::Engine* engine)
{
	int newID = -1;

	hover(engine->options.music.isSoundActive(), engine);

	if (event.type == sf::Event::JoystickMoved) {
		if (this->_joystickState == rtype::virtualkeysutils::joystickState::CHANGE) {
			if (event.joystickMove.axis == sf::Joystick::X && (event.joystickMove.position >= (int)rtype::action::joystickAxis::LEFT && event.joystickMove.position < -50))
				newID = this->_leftElementsID;
			if (event.joystickMove.axis == sf::Joystick::X && (event.joystickMove.position <= (int)rtype::action::joystickAxis::RIGHT && event.joystickMove.position > 50))
				newID = this->_rightElementsID;
			if (event.joystickMove.axis == sf::Joystick::Y && (event.joystickMove.position >= (int)rtype::action::joystickAxis::UP && event.joystickMove.position < -50))
				newID = this->_upElementsID;
			if (event.joystickMove.axis == sf::Joystick::Y && (event.joystickMove.position <= (int)rtype::action::joystickAxis::DOWN && event.joystickMove.position > 50))
				newID = this->_downElementsID;

			if (newID != -1)
				released();

			return newID;
		}
		else if (this->_joystickState == rtype::virtualkeysutils::joystickState::ARRIVED) {
			if ((event.joystickMove.axis == sf::Joystick::X && (event.joystickMove.position <= 0 && event.joystickMove.position > -5)) ||
				(event.joystickMove.axis == sf::Joystick::X && (event.joystickMove.position >= 0 && event.joystickMove.position < 5)) ||
				(event.joystickMove.axis == sf::Joystick::Y && (event.joystickMove.position <= 0 && event.joystickMove.position > 5)) ||
				(event.joystickMove.axis == sf::Joystick::Y && (event.joystickMove.position >= 0 && event.joystickMove.position < 5)))
				_joystickState = rtype::virtualkeysutils::joystickState::STAY;
		}
	}

	return newID;
}

int rtype::client::utilities::VirtualKeys::eventUpdate(sf::Event event, rtype::engine::Engine* engine)
{
	if (isHover(engine->window.mapPixelToCoords(sf::Mouse::getPosition(engine->window)))) {
		hover(engine->options.music.isSoundActive(), engine);
	}
	else
		released();
	return this->_id;
}

void rtype::client::utilities::VirtualKeys::draw(rtype::engine::Engine* engine)
{
	engine->window.draw(this->_keyShape);
	this->_keyText->draw(engine);
}

void rtype::client::utilities::VirtualKeys::setPosition(sf::Vector2f position)
{
	this->_position = position;

	this->_keyShape.setPosition(this->_position);
	this->_keyText->setPosition(this->_position);

	this->_bounds = this->_keyShape.getGlobalBounds();
}

sf::Vector2f rtype::client::utilities::VirtualKeys::getPosition(void)
{
	return this->_position;
}

sf::FloatRect rtype::client::utilities::VirtualKeys::getBounds(void)
{
	return this->_bounds;
}

int rtype::client::utilities::VirtualKeys::getId(void)
{
	return this->_id;
}

void rtype::client::utilities::VirtualKeys::setId(int Id)
{
	this->_id = Id;
}

void rtype::client::utilities::VirtualKeys::setRightElements(int Id)
{
	this->_rightElementsID = Id;
}

int rtype::client::utilities::VirtualKeys::getRightElements(void)
{
	return this->_rightElementsID;
}

void rtype::client::utilities::VirtualKeys::setLeftElements(int Id)
{
	this->_leftElementsID = Id;
}

int rtype::client::utilities::VirtualKeys::getLeftElements(void)
{
	return this->_leftElementsID;
}

void rtype::client::utilities::VirtualKeys::setUpElements(int Id)
{
	this->_upElementsID = Id;
}

int rtype::client::utilities::VirtualKeys::getUpElements(void)
{
	return this->_upElementsID;
}

void rtype::client::utilities::VirtualKeys::setDownElements(int Id)
{
	this->_downElementsID = Id;
}

int rtype::client::utilities::VirtualKeys::getDownElements(void)
{
	return this->_downElementsID;
}

std::string rtype::client::utilities::VirtualKeys::getKey(void)
{
	return this->_keyText->getText();
}

void rtype::client::utilities::VirtualKeys::setSize(sf::Vector2f size)
{
	this->_keyShape.setSize(size);
	this->_keyShape.setOrigin(sf::Vector2f(this->_keyShape.getSize().x / 2, this->_keyShape.getSize().y / 2));
	this->_keyShape.setPosition(this->_position);
	this->_bounds = this->_keyShape.getGlobalBounds();
}
