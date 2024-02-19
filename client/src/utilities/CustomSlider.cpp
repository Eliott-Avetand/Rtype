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
//  File:            CustomSlider.cpp                                                //
//  Description:     CustomSlider Class Definition                                   //
//                                                                                   //
// ********************************************************************************* //
#include "CustomSlider.hpp"

void basicCallback(rtype::engine::Engine* engine, rtype::sliderutils::CallbackData& data)
{
	// DoNothing
}

rtype::client::utilities::CustomSlider::CustomSlider(sf::Vector2f position, sf::Vector2f scale, float percentage, rtype::sliderutils::callback_t callback, int id)
{
	this->_scale = scale;
	this->_position = position;
	this->_percentage = percentage;
	this->_callback = callback;
	this->_state = rtype::client::utilities::CustomSlider::NORMAL;
	this->_joystickState = rtype::sliderutils::joystickState::ARRIVED;

	this->_baseRectangle.setFillColor(sf::Color::White);
	this->_baseRectangle.setSize(sf::Vector2f(200 * this->_scale.x, 10 * this->_scale.y));
	this->_baseRectangle.setOrigin(sf::Vector2f(this->_baseRectangle.getSize().x / 2, this->_baseRectangle.getSize().y / 2));
	this->_baseRectangle.setPosition(this->_position);

	this->_bounds = this->_baseRectangle.getGlobalBounds();

	this->_id = id;
	this->_rightElementsID = -1;
	this->_leftElementsID = -1;
	this->_upElementsID = -1;
	this->_downElementsID = -1;

	this->_isHover = false;

	this->_slider.setFillColor(sf::Color::White);
	this->_slider.setSize(sf::Vector2f(10 * this->_scale.x, 30 * this->_scale.y));
	this->_slider.setOrigin(sf::Vector2f(this->_slider.getSize().x / 2, this->_slider.getSize().y / 2));
	this->_slider.setPosition(sf::Vector2f(this->_position.x - this->_baseRectangle.getSize().x / 2 + this->_baseRectangle.getSize().x * percentage / 100, this->_position.y));
}

bool rtype::client::utilities::CustomSlider::isHover(sf::Vector2f mousePosition)
{
	if (_bounds.contains(mousePosition))
		return true;
	return false;
}

void rtype::client::utilities::CustomSlider::click(rtype::engine::Engine* engine, sf::Vector2f mousePosition)
{
	rtype::sliderutils::CallbackData datas;

	this->setPercentage((mousePosition.x - (this->_position.x - this->_baseRectangle.getSize().x / 2)) / this->_baseRectangle.getSize().x * 100);

	datas.percentage = this->_percentage;
	this->_state = rtype::client::utilities::CustomSlider::CLICK;

	if (_callback)
		_callback(engine, datas);
}

void rtype::client::utilities::CustomSlider::click(rtype::engine::Engine* engine, float percentage)
{
	rtype::sliderutils::CallbackData datas;

	datas.percentage = percentage;

	if (_callback)
		_callback(engine, datas);
}

void rtype::client::utilities::CustomSlider::released(void)
{
	this->_state = rtype::client::utilities::CustomSlider::NORMAL;
	this->_joystickState = rtype::sliderutils::joystickState::ARRIVED;

	_baseRectangle.setFillColor(sf::Color::White);
	_slider.setFillColor(sf::Color::White);

	this->_isHover = false;
}

void rtype::client::utilities::CustomSlider::hover(bool isSound, rtype::engine::Engine* engine)
{
	if (isSound && !this->_isHover) {
		engine->options.music.playHoverSound();
		this->_joystickClock.restart();
	}
	this->_isHover = true;
	_baseRectangle.setFillColor(sf::Color(128, 128, 128));
	_slider.setFillColor(sf::Color(128, 128, 128));
}

void rtype::client::utilities::CustomSlider::draw(rtype::engine::Engine* engine)
{
	engine->window.draw(this->_baseRectangle);
	engine->window.draw(this->_slider);
}

int rtype::client::utilities::CustomSlider::eventUpdate(sf::Event event, rtype::engine::Engine* engine)
{
	if (event.type == sf::Event::MouseButtonReleased)
		this->released();
	if (isHover(engine->window.mapPixelToCoords(sf::Mouse::getPosition(engine->window)))) {
		this->hover(engine->options.music.isSoundActive(), engine);
		if (event.type == sf::Event::MouseMoved && this->_state == rtype::client::utilities::CustomSlider::CLICK) {
			click(engine, engine->window.mapPixelToCoords(sf::Mouse::getPosition(engine->window)));
			return this->_id;
		}
		if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
			click(engine, engine->window.mapPixelToCoords(sf::Mouse::getPosition(engine->window)));
	} else {
		if (event.type == sf::Event::MouseMoved && this->_state == rtype::client::utilities::CustomSlider::CLICK) {
			click(engine, engine->window.mapPixelToCoords(sf::Mouse::getPosition(engine->window)));
			return this->_id;
		} else
			this->released();
	}

	return this->_id;
}

int rtype::client::utilities::CustomSlider::eventUpdateJoystick(sf::Event event, rtype::engine::Engine* engine)
{
	int newID = -1;

	this->hover(engine->options.music.isSoundActive(), engine);

	if (event.type == sf::Event::JoystickButtonPressed && event.joystickButton.button == rtype::action::joystickButton::A) {
		if (this->_joystickState != rtype::sliderutils::joystickState::FOCUS) {
			if (engine->options.music.isSoundActive())
				engine->options.music.playClickSound();
			this->_joystickState = rtype::sliderutils::joystickState::FOCUS;

		}
	}

	if (event.type == sf::Event::JoystickButtonPressed && event.joystickButton.button == rtype::action::joystickButton::B) {
		if (this->_joystickState == rtype::sliderutils::joystickState::FOCUS) {
			this->_joystickState = rtype::sliderutils::joystickState::STAY;
		}
	}

	if (event.type == sf::Event::JoystickMoved) {
		if (this->_joystickState == rtype::sliderutils::joystickState::CHANGE) {
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
		else if (this->_joystickState == rtype::sliderutils::joystickState::ARRIVED) {
			if ((event.joystickMove.axis == sf::Joystick::X && (event.joystickMove.position <= 0 && event.joystickMove.position > -5)) ||
				(event.joystickMove.axis == sf::Joystick::X && (event.joystickMove.position >= 0 && event.joystickMove.position < 5)) ||
				(event.joystickMove.axis == sf::Joystick::Y && (event.joystickMove.position <= 0 && event.joystickMove.position > 5)) ||
				(event.joystickMove.axis == sf::Joystick::Y && (event.joystickMove.position >= 0 && event.joystickMove.position < 5)))
				_joystickState = rtype::sliderutils::joystickState::STAY;
		}
	}

	return this->_id;
}

void rtype::client::utilities::CustomSlider::update(rtype::engine::Engine* engine)
{
	sf::Time time = this->_joystickClock.getElapsedTime();
	float milliseconds = time.asMilliseconds();

	if (!this->_isHover) {
		this->_joystickClock.restart();
		return;
	}

	if (this->_joystickState == rtype::sliderutils::joystickState::FOCUS) {
		if (sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::X) < -50 && sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::X) >= (int)rtype::action::joystickAxis::LEFT) {
			this->setPercentage(this->_percentage - 5);
			click(engine, this->_percentage);
		}
		if (sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::X) <= (int)rtype::action::joystickAxis::RIGHT  && sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::X) > 50) {
			this->setPercentage(this->_percentage + 5);
			click(engine, this->_percentage);
		}
	}

	if (milliseconds > 10) {
		if (this->_joystickState == rtype::sliderutils::joystickState::STAY)
			this->_joystickState = rtype::sliderutils::joystickState::CHANGE;
	}

}

void rtype::client::utilities::CustomSlider::setPosition(sf::Vector2f position)
{
	this->_position = position;

	this->_baseRectangle.setPosition(this->_position);
	this->_slider.setPosition(sf::Vector2f(this->_position.x - this->_baseRectangle.getSize().x / 2 + this->_baseRectangle.getSize().x * this->_percentage / 100, this->_position.y));
}

void rtype::client::utilities::CustomSlider::move(sf::Vector2f offset)
{
	this->_position += offset;
	this->_baseRectangle.move(offset);
	this->_slider.move(offset);
}

void rtype::client::utilities::CustomSlider::setPosition(float x, float y)
{
	this->_position = sf::Vector2f(x, y);

	this->_baseRectangle.setPosition(this->_position);
	this->_slider.setPosition(sf::Vector2f(this->_position.x - this->_baseRectangle.getSize().x / 2 + this->_baseRectangle.getSize().x * this->_percentage / 100, this->_position.y));
}


sf::Vector2f rtype::client::utilities::CustomSlider::getPosition(void)
{
	return this->_position;
}

void rtype::client::utilities::CustomSlider::setScale(sf::Vector2f scale)
{
	this->_scale = scale;

	this->_baseRectangle.setSize(sf::Vector2f(200 * this->_scale.x, 10 * this->_scale.y));
	this->_baseRectangle.setOrigin(sf::Vector2f(this->_baseRectangle.getSize().x / 2, this->_baseRectangle.getSize().y / 2));
	this->_baseRectangle.setPosition(this->_position);

	this->_slider.setSize(sf::Vector2f(10 * this->_scale.x, 30 * this->_scale.y));
	this->_slider.setOrigin(sf::Vector2f(this->_slider.getSize().x / 2, this->_slider.getSize().y / 2));
	this->_slider.setPosition(sf::Vector2f(this->_position.x - this->_baseRectangle.getSize().x / 2 + this->_baseRectangle.getSize().x * this->_percentage / 100, this->_position.y));
}

void rtype::client::utilities::CustomSlider::setScale(float scaleX, float scaleY)
{
	this->_scale = sf::Vector2f(scaleX, scaleY);

	this->_baseRectangle.setSize(sf::Vector2f(200 * this->_scale.x, 10 * this->_scale.y));
	this->_baseRectangle.setOrigin(sf::Vector2f(this->_baseRectangle.getSize().x / 2, this->_baseRectangle.getSize().y / 2));
	this->_baseRectangle.setPosition(this->_position);

	this->_slider.setSize(sf::Vector2f(10 * this->_scale.x, 30 * this->_scale.y));
	this->_slider.setOrigin(sf::Vector2f(this->_slider.getSize().x / 2, this->_slider.getSize().y / 2));
	this->_slider.setPosition(sf::Vector2f(this->_position.x - this->_baseRectangle.getSize().x / 2 + this->_baseRectangle.getSize().x * this->_percentage / 100, this->_position.y));
}

sf::Vector2f rtype::client::utilities::CustomSlider::getScale(void)
{
	return this->_scale;
}

void rtype::client::utilities::CustomSlider::setPercentage(float percentage)
{
	if (percentage > 100)
		percentage = 100;
	if (percentage < 0)
		percentage = 0;

	this->_percentage = percentage;

	this->_slider.setPosition(sf::Vector2f(this->_position.x - this->_baseRectangle.getSize().x / 2 + this->_baseRectangle.getSize().x * this->_percentage / 100, this->_position.y));
}

float rtype::client::utilities::CustomSlider::getPercentage(void)
{
	return this->_percentage;
}

int rtype::client::utilities::CustomSlider::getId(void)
{
	return this->_id;
}

void rtype::client::utilities::CustomSlider::setId(int Id)
{
	this->_id = Id;
}

void rtype::client::utilities::CustomSlider::setRightElements(int Id)
{
	this->_rightElementsID = Id;
}

int rtype::client::utilities::CustomSlider::getRightElements(void)
{
	return this->_rightElementsID;
}

void rtype::client::utilities::CustomSlider::setLeftElements(int Id)
{
	this->_leftElementsID = Id;
}

int rtype::client::utilities::CustomSlider::getLeftElements(void)
{
	return this->_leftElementsID;
}

void rtype::client::utilities::CustomSlider::setUpElements(int Id)
{
	this->_upElementsID = Id;
}

int rtype::client::utilities::CustomSlider::getUpElements(void)
{
	return this->_upElementsID;
}

void rtype::client::utilities::CustomSlider::setDownElements(int Id)
{
	this->_downElementsID = Id;
}

int rtype::client::utilities::CustomSlider::getDownElements(void)
{
	return this->_downElementsID;
}