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
//  File:            ShootBar.hpp                                                    //
//  Description:     ShootBar Header                                                 //
//                                                                                   //
// ********************************************************************************* //
#pragma once

#include "Engine.hpp"
#include "Actor.hpp"
#include "StaticSprite.hpp"

#define SHOOT_BAR_TEXTURE "./client/assets/sprites/UI/game/shoot_bar.png"

namespace rtype {
	namespace client {
		class ShootBar {
			private:
				rtype::client::utilities::StaticSprite _shootBarSprite;
				sf::RectangleShape _loadingShootRect;

				sf::Vector2f _position;
			public:
				ShootBar(sf::Vector2f position = sf::Vector2f(0, 0));

				void draw(rtype::engine::Engine* engine);
				void update(rtype::engine::Engine* engine, rtype::actor::Actor* player);

				void setPosition(const sf::Vector2f position);
				sf::Vector2f getPosition(void) const;

				float getWidth(void);
				float getWidth(const float percentage);

				float getHeight(void);
				float getHeight(const float percentage);
		};
	};
};
