#define SDL_MAIN_HANDLED
#include <iostream>
#include "manager.h"
#include "game_manager.h"

#include "map.h"
#include "config_manager.h"
#include "resources_manager.h"


int main(int argc, char** argv) {

	//ConfigManager::GetInstance()->Load_Game_Config("./MapData/config.json");
	
	//return Test_Map_H();
	Test_ResMgr();
	return 0;
	//return GameManager::GetInstance()->Run(argc, argv); // 游戏主循环
};