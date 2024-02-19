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
//  File:            CustomSlider.hpp                                                //
//  Description:     CustomSlider Class Header                                       //
//                                                                                   //
// ********************************************************************************* //
#pragma once

#include <SFML/Graphics.hpp>

#include "Errors.hpp"
#include "Engine.hpp"
#include "InputHandler.hpp"

namespace rtype {
	namespace sliderutils {
		struct CallbackData {
			float percentage;
		};

		enum joystickState {
			ARRIVED,
			STAY,
			FOCUS,
			CHANGE,
		};

		typedef void (*callback_t)(rtype::engine::Engine* engine, struct CallbackData& data);
	};
}

/**
 * @brief This is a method which display all options texts
 * @see This method is automatically called when scene is render
 *
 * @param engine    A reference of the engine given by the engine himself
 * @param data		All datas needed in a slider
 */
void basicCallback(rtype::engine::Engine* engine, rtype::sliderutils::CallbackData& data);

namespace rtype {
	namespace client {
		namespace utilities {
			class CustomSlider {
				public:
					enum sliderState {
						NORMAL,
						CLICK,
					};

					CustomSlider(sf::Vector2f position = sf::Vector2f(0, 0), sf::Vector2f scale = sf::Vector2f(1, 1), float percentage = 100, rtype::sliderutils::callback_t callback = &basicCallback, int id = -1);

					/**
					 * @brief This is a method which display slider
					 * @see This method is automatically called when scene is render
					 *
					 * @param engine    A reference of the engine given by the engine himself
					 * @param data		All datas needed in a slider
					 */
					void draw(rtype::engine::Engine* engine);

					/**
					 * @brief This is a method which update the event of slider
					 * @see This method is automatically called when scene is update
					 *
					 * @param event		All events triggered in the game
					 * @param engine    A reference of the engine given by the engine himself
					 */
					int eventUpdate(sf::Event event, rtype::engine::Engine* engine);

					/**
					 * @brief Handle events related to the component by the joystick
					 *
					 * @param event sf::Event contained in GameEngine, used to detect what event is fired by user
					 * @param engine GameEngine pointer that will be used to draw in the window
					 */
					int eventUpdateJoystick(sf::Event event, rtype::engine::Engine* engine);

					/**
					 * @brief Handle update
					 *
					 * @param engine GameEngine pointer that will be used to draw in the window
					 */
					void update(rtype::engine::Engine* engine);

					/**
					 * @brief Set position of the slider
					 * @see This method is called wherever you want
					 *
					 * @param position	Position of the slider
					 */
					void setPosition(sf::Vector2f position);

					/**
					 * @brief Move the slider in the window
					 *
					 * @param offset Offset to move the slider
					 */
					void move(sf::Vector2f offset);

					/**
					 * @brief Set position of the slider
					 * @see This method is called wherever you want
					 *
					 * @param positionX	Position X of the slider
					 * @param positionY	Position Y of the slider
					 */
					void setPosition(float x, float y);

					/**
					 * @brief Get position of the slider
					 * @see This method is called wherever you want
					 *
					 * @return position	Position of the slider
					 */
					sf::Vector2f getPosition(void);

					/**
					 * @brief Set position of the slider
					 * @see This method is called wherever you want
					 *
					 * @param scale		Position of the slider
					 */
					void setScale(sf::Vector2f scale);
					
					/**
					 * @brief Set position of the slider
					 * @see This method is called wherever you want
					 *
					 * @param scaleX		Position X of the slider
					 * @param scaleY		Position Y of the slider
					 */
					void setScale(float scaleX, float scaleY);
					
					/**
					 * @brief Get scale of the slider
					 * @see This method is called wherever you want
					 *
					 * @return scale	Position of the slider
					 */
					sf::Vector2f getScale(void);


					/**
					 * @brief Set percentage of the slider
					 * @see This method is called wherever you want
					 *
					 * @param percentage	Percentage of the slider
					 */
					void setPercentage(float percentage);

					/**
					 * @brief Get percentage of the slider
					 * @see This method is called wherever you want
					 *
					 * @return percentage	Percentage of the slider
					 */
					float getPercentage(void);

					/**
					 * @brief Set ID of the button
					 * @see This method is called wherever you want
					 *
					 * @param ID	ID of the slider
					 */
					void setId(int Id);

					/**
					 * @brief Get ID of the button
					 * @see This method is called wherever you want
					 *
					 * @return ID	ID of the slider
					 */
					int getId(void);

					/**
					 * @brief Get ID of the right element
					 * @see This method is called wherever you want
					 *
					 * @return ID	ID of the right element
					 */
					int getRightElements(void);

					/**
					 * @brief Get ID of the left element
					 * @see This method is called wherever you want
					 *
					 * @return ID	ID of the left element
					 */
					int getLeftElements(void);

					/**
					 * @brief Get ID of the up element
					 * @see This method is called wherever you want
					 *
					 * @return ID	ID of the up element
					 */
					int getUpElements(void);

					/**
					 * @brief Get ID of the down element
					 * @see This method is called wherever you want
					 *
					 * @return ID	ID of the down element
					 */
					int getDownElements(void);


					/**
					 * @brief Set ID of the right element
					 * @see This method is called wherever you want
					 *
					 * @param ID	ID of the right element
					 */
					void setRightElements(int Id);

					/**
					 * @brief Set ID of the left element
					 * @see This method is called wherever you want
					 *
					 * @param ID	ID of the left element
					 */
					void setLeftElements(int Id);

					/**
					 * @brief Set ID of the up element
					 * @see This method is called wherever you want
					 *
					 * @param ID	ID of the up element
					 */
					void setUpElements(int Id);

					/**
					 * @brief Set ID of the down element
					 * @see This method is called wherever you want
					 *
					 * @param ID	ID of the down element
					 */
					void setDownElements(int Id);

				private:
					sf::RectangleShape _baseRectangle;
					sf::RectangleShape _slider;

					float _percentage;

					rtype::sliderutils::joystickState _joystickState;
					sf::Clock _joystickClock;
					
					int _id;
					int _rightElementsID;
					int _leftElementsID;
					int _upElementsID;
					int _downElementsID;

					sf::FloatRect _bounds;
					sf::Vector2f _position;
					sf::Vector2f _scale;
					rtype::client::utilities::CustomSlider::sliderState _state;

					bool _isHover;

					rtype::sliderutils::callback_t _callback;

					/**
					 * @brief Get if slider is hover or not
					 * @see This method is called wherever you want
					 *
					 * @return bool		True if is hover, false otherwise.
					 */
					bool isHover(sf::Vector2f mousePosition);
					
					/**
					 * @brief click on the slider
					 * @see This method is called wherever you want
					 */
					void click(rtype::engine::Engine* engine, sf::Vector2f mousePosition);
					
					/**
					 * @brief click on the slider
					 * @see This method is called wherever you want
					 */
					void click(rtype::engine::Engine* engine, float percentage);

					/**
					 * @brief Release focus of slider
					 * @see This method is called wherever you want
					 */
					void released(void);

					/**
					 * @brief Define the behavior when the button is hovered
					 *
					 * @param isSound Boolean that represent if the hover sound should be played or not
					 * @param engine GameEngine pointer that will be used to draw in the window
					 */
					void hover(bool isSound, rtype::engine::Engine* engine);

			};
		};
	};
};
