
#ifndef __GAME_STATE_H_INCLUDED__
#define __GAME_STATE_H_INCLUDED__

#include "actor.h"

class Game;

class GameState {
private:
	Game& game;

	vector<Actor> actors;

public:
	GameState(Game& g) : game(g) {};

	vector<Actor>& get_actors();

};

#endif