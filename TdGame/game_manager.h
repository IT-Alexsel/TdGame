#pragma once
#include "manager.h"

#include <SDL.h>
#include <SDL_ttf.h> // 字体处理
#include <SDL_image.h> // 图片处理
#include <SDL_mixer.h> // 音频处理
#include <SDL2_gfxPrimitives.h> // 绘制图形


class GameManager : public SingleMgr<GameManager>
{
	friend class SingleMgr<GameManager>; // 允许SingleMgr访问私有成员
public:
	int Run(int argc, char** argv) // 游戏主循环
	{	
		Uint64 last_count = SDL_GetPerformanceCounter(); // 获取当前计数(高性能下计数器的计数)
		Uint64 counter_freq = SDL_GetPerformanceFrequency(); // 获取计数频率(每秒高性能下计数器的计数)
		Uint64 current_counter; // 当前帧计数

		while (isRunning)
		{
			while (SDL_PollEvent(&event))
			{
				on_input();
			};

			// 限制帧率: (当前帧计数-上一帧计数)/秒计数频率
			current_counter = SDL_GetPerformanceCounter();
			double delta_time = (double)(current_counter - last_count) / counter_freq;
			last_count = current_counter;
			if (delta_time * 1000 < 1000.0 / fps) {
				SDL_Delay((Uint32)(1000.0 / fps - delta_time * 1000)); // 延迟
			};

			// 更新
			on_update();

			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // 设置渲染器颜色
			SDL_RenderClear(renderer); // 清空屏幕
			// 绘制画面
            on_render();
            SDL_RenderPresent(renderer); // 更新屏幕
		};
		return 0;
	};
protected:
	GameManager() {
		std::cout << "GameManager Created" << std::endl;
		InitSDL();
	};
	~GameManager() {
        std::cout << "GameManager Destroyed" << std::endl;
		SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
		TTF_Quit();
        Mix_Quit();
        IMG_Quit();
        SDL_Quit();
	};
private:
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;
	bool isRunning = true;
	SDL_Event event;
    int fps = 60;
	// -------------初始化--------------
	void InitSDL()
	{
		Init_Assert(!SDL_Init(SDL_INIT_EVERYTHING), u8"SDL_Init Error"); // 初始化SDL
        Init_Assert(IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG), u8"IMG_Init Error"); // 初始化IMG
        Init_Assert(Mix_Init(MIX_INIT_OGG | MIX_INIT_MP3), u8"MIX_Init Error"); // 初始化MIX
		Init_Assert(!TTF_Init(), u8"TTF_Init Error"); // 初始化TTF
        Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048); // 初始化音频
		// 设置SDL提示，显示输入法
		SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1"); 
		// 初始化窗口
		Init_Window();
		// 初始化渲染器
        Init_Renderer();
	};
	void Init_Window()
	{
		window = SDL_CreateWindow(u8"TdGame", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 700, SDL_WINDOW_SHOWN);
		Init_Assert(window, u8"SDL_CreateWindow Error");
	};
	void Init_Renderer()
	{
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);// 创建渲染器(硬件加速、垂直同步、渲染目标为纹理)
        Init_Assert(renderer, u8"SDL_CreateRenderer Error");
	};
	void Init_Assert(bool flag, const char* error)
	{
		if (flag)
		{
			return;
		};
		std::cout << flag << "-----" << error << ":" << SDL_GetError() << std::endl;
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, u8"错误", error, window);
		exit(-1);
	};
    // -------------逻辑处理--------------
	void on_input()
	{
		switch (event.type)
		{
			case SDL_QUIT: // 退出事件
				isRunning = false;
                break;

			//case SDL_KEYDOWN: // 按键按下事件


		default:
			break;
		}
	};
	void on_update()
	{
	};
	void on_render()
	{
	};

};

