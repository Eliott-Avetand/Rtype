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
//  File:            CustomText.cpp                                                  //
//  Description:     Component: Room infos displayed in MultiplayerRoomMenu          //
//                                                                                   //
// ********************************************************************************* //

#include "SlotInfoComponent.hpp"
#include "MultiplayerWaitingRoom.hpp"

void toggleReadyState(rtype::engine::Engine* engine, rtype::buttonutils::CallbackData &data)
{
    // Void
}

rtype::client::utilities::SlotInfoComponent::SlotInfoComponent(std::string name, sf::Vector2f position)
{
    this->_borderSprite = rtype::client::utilities::StaticSprite(SLOTINFO_BORDER, position);
    this->_playerName = std::make_unique<rtype::client::utilities::CustomText>(name, FONT_PATH, 30, sf::Color::White, position);
    this->_readyCheckbox = std::make_unique<rtype::client::utilities::CustomCheckBox>(sf::Vector2f(position.x - 75 - (this->_borderSprite.getWidth()/2), position.y), 0.35f, toggleReadyState, false);
    if (!this->_playerName || !this->_readyCheckbox)
        throw rtype::settings::Errors("Not enough memory", "MemoryError:");
    this->updatePlayerNamePosition();
}

void rtype::client::utilities::SlotInfoComponent::setPosition(sf::Vector2f position)
{
	this->_borderSprite.setPosition(position);
    this->updatePlayerNamePosition();
}

void rtype::client::utilities::SlotInfoComponent::move(sf::Vector2f offset)
{
	this->_borderSprite.getSprite().move(offset);
	this->_readyCheckbox->move(offset);
    this->updatePlayerNamePosition();
}

void rtype::client::utilities::SlotInfoComponent::setPlayerName(std::string name)
{
    this->_playerName->setText(name);
}

sf::Vector2f rtype::client::utilities::SlotInfoComponent::getPosition(void)
{
	return this->_borderSprite.getPosition();
}

float rtype::client::utilities::SlotInfoComponent::getWidth(void)
{
	return this->_borderSprite.getSprite().getGlobalBounds().width;
}

float rtype::client::utilities::SlotInfoComponent::getHeight(void)
{
	return this->_borderSprite.getSprite().getGlobalBounds().height;
}

void rtype::client::utilities::SlotInfoComponent::draw(rtype::engine::Engine* engine)
{
	this->_borderSprite.draw(engine);
	this->_playerName->draw(engine);
    this->_readyCheckbox->draw(engine);
}

void rtype::client::utilities::SlotInfoComponent::updatePlayerNamePosition(void)
{
    sf::Vector2f pos = this->_borderSprite.getSprite().getPosition();
    sf::FloatRect borderBounds = this->_borderSprite.getSprite().getLocalBounds();
    sf::FloatRect playerNameBounds = this->_playerName->getLocalBounds();

    this->_playerName->setPosition(sf::Vector2f(pos.x - (borderBounds.width/2) + (playerNameBounds.width/2) + 50, pos.y));
}

std::unique_ptr<rtype::client::utilities::CustomCheckBox> &rtype::client::utilities::SlotInfoComponent::getReadyCheckbox(void)
{
    return this->_readyCheckbox;
}

std::string rtype::client::utilities::SlotInfoComponent::getPlayerNameInSlot(void)
{
    return this->_playerName->getText();
}