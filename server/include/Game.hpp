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
//  File:            Game.hpp                                                        //
//  Description:     Handle the game instance in the server                          //
//                                                                                   //
//                                                                                   //
// ********************************************************************************* //
#pragma once

#include "Monster.hpp"
#include "Player.hpp"
#include "BossDobkeratops.hpp"


class Room;

class Game {
	private:
		Room *_room;

		std::vector<std::unique_ptr<rtype::actor::Actor>> _players;
		std::vector<std::unique_ptr<rtype::actor::Actor>> _enemies;
		std::vector<std::unique_ptr<rtype::actor::Breed>> _breeds;
		std::vector<std::unique_ptr<rtype::actor::Asteroid>> _asteroids;

		std::unique_ptr<rtype::actor::BossDobkeratops> _boss;
		bool _isBoss;
		
		unsigned short _levelID;

		bool _resetVariable;

		int _breedNumber;

		size_t _playerNumber;

		LevelState _levelStatus;

		sf::Clock _asteroidsClock;
		float _asteroidsTime;

		sf::Clock _timer;

		void createEnemies();
		void initBoss(void);
	public:
		Game(unsigned short levelID, size_t playersCount, Room *room);
		~Game();

		void update();
		void checkGameStatus(void);
		bool checkNextGameLevel(void);

		LevelState getGameStatus(void);

		EnemyData getEnemyDatas(int index);
		AsteroidsDatas getAsteroidDatas(int index);
		dobkeratopsMissileDatas getBossMissileDatas(int index);
		dobkeratopsMissileDatas getBossHeartMissileDatas(std::string heartID, int index);
		std::pair<float, float> getBossTailsPosition(int index);
		std::vector<std::unique_ptr<rtype::actor::Actor>>& getPlayers(void);
		std::vector<std::unique_ptr<rtype::actor::Actor>>& getEnemies(void);
		std::vector<std::unique_ptr<rtype::actor::Breed>>& getBreeds(void);
		std::vector<std::unique_ptr<rtype::actor::Asteroid>>& getAsteroids(void);
		std::unique_ptr<rtype::actor::BossDobkeratops> &getBoss(void);
};
