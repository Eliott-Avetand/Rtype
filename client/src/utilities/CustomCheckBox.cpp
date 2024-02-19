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
//  File:            CustomCheckBox.cpp                                              //
//  Description:     File for the CustomCheckBox Class                               //
//                                                                                   //
// ********************************************************************************* //

#include "CustomCheckBox.hpp"

rtype::client::utilities::CustomCheckBox::CustomCheckBox(sf::Vector2f position, float size, rtype::buttonutils::callback_t callback, bool isEnable, int id) :
	_checkBoxSprite(CHECK_BOX_TEXTURE, position),
	_isEnableSprite(CHECKED_TEXTURE, position)
{
	_isEnable = isEnable;
	_size = size;
	_callback = callback;

	this->_joystickState = rtype::checkboxutils::joystickState::ARRIVED;
	this->_checkboxState = rtype::client::utilities::CustomCheckBox::NORMAL;

	this->_id = id;
	this->_rightElementsID = -1;
	this->_leftElementsID = -1;
	this->_upElementsID = -1;
	this->_downElementsID = -1;

	_checkBoxSprite.getSprite().scale(sf::Vector2f(_size, _size));
	_isEnableSprite.getSprite().scale(sf::Vector2f(_size, _size));
}

void rtype::client::utilities::CustomCheckBox::hover(bool isSound, rtype::engine::Engine* engine)
{
	if (isSound && this->_checkboxState != rtype::client::utilities::CustomCheckBox::checkboxState::HOVER) {
		engine->options.music.playHoverSound();
		this->_joystickClock.restart();
	}
	this->_checkboxState = rtype::client::utilities::CustomCheckBox::checkboxState::HOVER;
	_checkBoxSprite.setColor(sf::Color(128, 128, 128));
	_isEnableSprite.setColor(sf::Color(128, 128, 128));
}

void rtype::client::utilities::CustomCheckBox::setCallback(rtype::buttonutils::callback_t callback)
{
	_callback = callback;
}

void rtype::client::utilities::CustomCheckBox::setPosition(sf::Vector2f position)
{
	_checkBoxSprite.setPosition(position);
	_isEnableSprite.setPosition(position);
}

void rtype::client::utilities::CustomCheckBox::move(sf::Vector2f offset)
{
	this->_checkBoxSprite.getSprite().move(offset);
	this->_isEnableSprite.getSprite().move(offset);
}

bool rtype::client::utilities::CustomCheckBox::isEnable(void)
{
	return _isEnable;
}

void rtype::client::utilities::CustomCheckBox::draw(rtype::engine::Engine* engine)
{
	if (_isEnable)
		_isEnableSprite.draw(engine);
	_checkBoxSprite.draw(engine);
}

bool rtype::client::utilities::CustomCheckBox::isHover(sf::Vector2f mousePosition)
{
	sf::FloatRect bound = _checkBoxSprite.getSprite().getGlobalBounds();

	if (bound.contains(mousePosition))
		return true;
	return false;
}

void rtype::client::utilities::CustomCheckBox::click(bool isSound, rtype::engine::Engine* engine)
{
	rtype::buttonutils::CallbackData data; //A PATCH

	engine->options.music.playClickSound();
	if (this->_callback)
		this->_callback(engine, data);
	_isEnable = !_isEnable;
}

void rtype::client::utilities::CustomCheckBox::update(rtype::engine::Engine* engine)
{
	sf::Time time = this->_joystickClock.getElapsedTime();
	float milliseconds = time.asMilliseconds();

	if (this->_checkboxState != HOVER) {
		this->_joystickClock.restart();
		return;
	}

	if (milliseconds > 10) {
		if (this->_joystickState == rtype::checkboxutils::joystickState::STAY)
			this->_joystickState = rtype::checkboxutils::joystickState::CHANGE;
	}

}

int rtype::client::utilities::CustomCheckBox::eventUpdate(sf::Event event, rtype::engine::Engine* engine)
{
	if (isHover(engine->window.mapPixelToCoords(sf::Mouse::getPosition(engine->window)))) {
		hover(engine->options.music.isSoundActive(), engine);
		if (event.type == sf::Event::MouseButtonPressed)
			click(engine->options.music.isSoundActive(), engine);
	}
	else
		this->released();
	return this->_id;
}

void rtype::client::utilities::CustomCheckBox::released()
{
	this->_checkboxState = rtype::client::utilities::CustomCheckBox::checkboxState::NORMAL;
	this->_joystickState = rtype::checkboxutils::joystickState::ARRIVED;
	this->_checkBoxSprite.setColor(sf::Color::White);
	this->_isEnableSprite.setColor(sf::Color::White);
}

int rtype::client::utilities::CustomCheckBox::eventUpdateJoystick(sf::Event event, rtype::engine::Engine* engine)
{
	int newID = -1;

	hover(engine->options.music.isSoundActive(), engine);

	if (event.type == sf::Event::JoystickButtonPressed && event.joystickButton.button == rtype::action::joystickButton::A)
		click(engine->options.music.isSoundActive(), engine);

	if (event.type == sf::Event::JoystickMoved) {
		if (this->_joystickState == rtype::checkboxutils::joystickState::CHANGE) {
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
		else if (this->_joystickState == rtype::checkboxutils::joystickState::ARRIVED) {
			if ((event.joystickMove.axis == sf::Joystick::X && (event.joystickMove.position <= 0 && event.joystickMove.position > -5)) ||
				(event.joystickMove.axis == sf::Joystick::X && (event.joystickMove.position >= 0 && event.joystickMove.position < 5)) ||
				(event.joystickMove.axis == sf::Joystick::Y && (event.joystickMove.position <= 0 && event.joystickMove.position > 5)) ||
				(event.joystickMove.axis == sf::Joystick::Y && (event.joystickMove.position >= 0 && event.joystickMove.position < 5)))
				_joystickState = rtype::checkboxutils::joystickState::STAY;
		}
	}

	return this->_id;
}

int rtype::client::utilities::CustomCheckBox::getId(void)
{
	return this->_id;
}

void rtype::client::utilities::CustomCheckBox::setId(int Id)
{
	this->_id = Id;
}

void rtype::client::utilities::CustomCheckBox::setRightElements(int Id)
{
	this->_rightElementsID = Id;
}

int rtype::client::utilities::CustomCheckBox::getRightElements(void)
{
	return this->_rightElementsID;
}

void rtype::client::utilities::CustomCheckBox::setLeftElements(int Id)
{
	this->_leftElementsID = Id;
}

int rtype::client::utilities::CustomCheckBox::getLeftElements(void)
{
	return this->_leftElementsID;
}

void rtype::client::utilities::CustomCheckBox::setUpElements(int Id)
{
	this->_upElementsID = Id;
}

int rtype::client::utilities::CustomCheckBox::getUpElements(void)
{
	return this->_upElementsID;
}

void rtype::client::utilities::CustomCheckBox::setDownElements(int Id)
{
	this->_downElementsID = Id;
}

int rtype::client::utilities::CustomCheckBox::getDownElements(void)
{
	return this->_downElementsID;
}