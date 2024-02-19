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

#include "RoomInfoComponent.hpp"
#include "MultiplayerRoomMenu.hpp"

void openPopup(rtype::engine::Engine *engine, rtype::buttonutils::CallbackData &data)
{
    rtype::client::MultiplayerRoomMenu::getInstance()->getPopup()->open(data.roomName);
}

rtype::client::utilities::RoomInfoComponent::RoomInfoComponent()
{
    // Void
}

rtype::client::utilities::RoomInfoComponent::RoomInfoComponent(std::string roomName, int numberOfPlayer, sf::Vector2f position)
{
    this->_borderSprite = rtype::client::utilities::StaticSprite(ROOMINFO_BORDER, position);
    this->_roomName = std::make_unique<rtype::client::utilities::CustomText>(roomName, FONT_PATH, 30, sf::Color::White, position);
    this->_numberOfPlayer = std::make_unique<rtype::client::utilities::CustomText>(std::to_string(numberOfPlayer) + "/4", FONT_PATH, 20, sf::Color::White, position);
    this->_joinRoomBtn = std::make_unique<rtype::client::utilities::CustomButton>("JOIN ROOM", position, 20.0f, openPopup, 1);
    if (!this->_roomName || !this->_numberOfPlayer || !this->_joinRoomBtn)
        throw rtype::settings::Errors("Cannot allocate more memory", "MemoryError");

    rtype::buttonutils::CallbackData data;
    data.roomName.assign(roomName);
    this->_joinRoomBtn->setAdditionalData(data);
    this->setObjectsPosition();
}

void rtype::client::utilities::RoomInfoComponent::setPosition(sf::Vector2f position)
{
	this->_borderSprite.setPosition(position);
    this->setObjectsPosition();
}

void rtype::client::utilities::RoomInfoComponent::move(sf::Vector2f offset)
{
    this->_borderSprite.getSprite().move(offset);
    this->_joinRoomBtn->getText()->getTextObject().move(offset);
    this->_roomName->getTextObject().move(offset);
    this->_numberOfPlayer->getTextObject().move(offset);
}

sf::Vector2f rtype::client::utilities::RoomInfoComponent::getPosition(void)
{
	return this->_borderSprite.getPosition();
}

float rtype::client::utilities::RoomInfoComponent::getWidth(void)
{
	return this->_borderSprite.getSprite().getGlobalBounds().width;
}

float rtype::client::utilities::RoomInfoComponent::getHeight(void)
{
	return this->_borderSprite.getSprite().getGlobalBounds().height;
}

void rtype::client::utilities::RoomInfoComponent::draw(rtype::engine::Engine* engine)
{
	this->_borderSprite.draw(engine);
	this->_roomName->draw(engine);
	this->_numberOfPlayer->draw(engine);
    this->_joinRoomBtn->draw(engine);
}

std::unique_ptr<rtype::client::utilities::CustomButton> &rtype::client::utilities::RoomInfoComponent::getJoinRoomBtn(void)
{
    return this->_joinRoomBtn;
}

void rtype::client::utilities::RoomInfoComponent::setObjectsPosition(void)
{
    sf::FloatRect bounds = this->_borderSprite.getSprite().getLocalBounds();
    sf::Vector2f pos(this->_borderSprite.getPosition().x - (bounds.width / 2), this->_borderSprite.getPosition().y - (bounds.height / 2));
    sf::Vector2f offset(20, 20);

    sf::FloatRect roomNameBounds = this->_roomName->getLocalBounds();
    this->_roomName->setPosition(sf::Vector2f(pos.x + (roomNameBounds.width/2) + offset.x, pos.y + (roomNameBounds.height/2) + offset.y));

    sf::FloatRect nbPlayerBounds = this->_numberOfPlayer->getLocalBounds();
    this->_numberOfPlayer->setPosition(sf::Vector2f(pos.x + bounds.width - (nbPlayerBounds.width/2) - offset.x, pos.y + (nbPlayerBounds.height/2) + offset.y));

    sf::FloatRect joinRoomBounds = this->_joinRoomBtn->getText()->getLocalBounds();
    this->_joinRoomBtn->setPosition(sf::Vector2f(pos.x + bounds.width - (joinRoomBounds.width/2) - offset.x, pos.y + bounds.height - (joinRoomBounds.height/2) - offset.y));
}
