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
//  File:            ACustomSprite.hpp                                               //
//  Description:     ACustomSprite Class Header                                      //
//                                                                                   //
// ********************************************************************************* //
#pragma once

#include "Engine.hpp"
#include <utility>
#include <memory>
#include <SFML/Graphics.hpp>

namespace rtype {
	namespace client {
		namespace utilities {
			class ACustomSprite {
			public:
				virtual void setPosition(sf::Vector2f position) = 0;
				virtual void setOpacity(int opacity) = 0;

				virtual void setScale(float scaleX, float scaleY) = 0;

				virtual sf::Sprite& getSprite(void) = 0;
				virtual sf::Vector2f getPosition(void) = 0;

				virtual float getWidth(void) = 0;
				virtual float getWidth(float percentage) = 0;


				virtual float getHeight(void) = 0;
				virtual float getHeight(float percentage) = 0;

				virtual int getOpacity(void) = 0;

				virtual sf::Color getColor(void) = 0;
				virtual void setColor(sf::Color color) = 0;

				virtual void draw(rtype::engine::Engine* engine) = 0;
			};
		};
	};
};