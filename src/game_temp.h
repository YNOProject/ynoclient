/////////////////////////////////////////////////////////////////////////////
// This file is part of EasyRPG Player.
//
// EasyRPG Player is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// EasyRPG Player is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with EasyRPG Player. If not, see <http://www.gnu.org/licenses/>.
/////////////////////////////////////////////////////////////////////////////

#ifndef __game_temp__
#define __game_temp__

#include <string>
#include "game_battler.h"

class Game_Temp {

public:
	Game_Temp();
	~Game_Temp();

	bool menu_calling;
	bool menu_beep;

	Game_Battler* forcing_battler;

	bool battle_calling;
	bool shop_calling;
	bool name_calling;
	bool save_calling;
	bool gameover;

	int common_event_id;

};
#endif // __game_temp__
