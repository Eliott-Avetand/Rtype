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
//  File:            GameUI.hpp                                                      //
//  Description:     GameUI Class Header                                             //
//                                                                                   //
// ********************************************************************************* //
#pragma once

#include "Engine.hpp"
#include "Player.hpp"

#include "StaticSprite.hpp"
#include "ShootBar.hpp"
#include "CustomText.hpp"

#define LIFE_TEXTURE "./client/assets/sprites/UI/game/life_sprite.png"

namespace rtype {
	namespace client {
		class GameUI {
			private:
				sf::RectangleShape _background;
				std::unique_ptr<rtype::client::ShootBar> _shootBar;

				std::unique_ptr<rtype::client::utilities::CustomText> _beamText;
				std::unique_ptr<rtype::client::utilities::CustomText> _playerNumberText;
				std::unique_ptr<rtype::client::utilities::CustomText> _scoreText;

				std::vector<rtype::client::utilities::StaticSprite> _lifeSprite;

				int _actualLife;

				void createBackground(rtype::engine::Engine* engine);
				void createTexts(rtype::engine::Engine* engine);
				void createLifeSprite(rtype::engine::Engine* engine);

				void addSpriteLife(rtype::engine::Engine* engine);
				void removeLifeSprite(void);

				void updateScoreText(rtype::engine::Engine* engine, rtype::actor::Actor* player);

			public:
				GameUI();

				void draw(rtype::engine::Engine* engine);
				void update(rtype::engine::Engine* engine, rtype::actor::Actor *player);
		};

	};
};