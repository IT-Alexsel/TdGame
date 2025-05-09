#define SDL_MAIN_HANDLED
#include <string> // 字符串
#include <fstream> // 文件操作
#include <sstream> // 字符串操作
#include <iostream> // 输入输出

#include <SDL.h>
#include <SDL_ttf.h> // 字体处理
#include <SDL_image.h> // 图片处理
#include <SDL_mixer.h> // 音频处理
#include <SDL2_gfxPrimitives.h> // 绘制图形

#include <cJSON.h> // json处理

/*
SDL API:https://wiki.libsdl.org/SDL2/CategoryAPI

*/

const char* PhotoPath = "res/128.png";
const char* FontPath = "res/abb.ttf";
const char* MusicPath = "res/aaa.ogg";

void test_json() 
{
    std::ifstream file("test.json");
    if (!file.good())
    {
        std::cout << "文件打开失败" << std::endl;
        return;
    };
    std::stringstream buffer; // 字符串流
    buffer << file.rdbuf(); // 将文件内容读入字符串流
    std::string json = buffer.str(); // 将字符串流转换为字符串
    std::cout << json << std::endl;
    file.close();

    cJSON* root = cJSON_Parse(json.c_str());
    if (root == nullptr)
        return;
    cJSON* name = cJSON_GetObjectItem(root, "name");
    cJSON* age = cJSON_GetObjectItem(root, "age");
    cJSON* pets = cJSON_GetObjectItem(root, "pets");
    std::cout << name->string << ":" << name->valuestring << std::endl;
    std::cout << age->string << ":" << age->valueint << std::endl;
    // 解析JSON数组
    std::cout << pets->string <<":" << std::endl;
    cJSON* json_item = nullptr;
    cJSON_ArrayForEach(json_item, pets) {
        std::cout << ":" << json_item->valuestring << std::endl;
    };

};

void test_csv() {
    std::ifstream file("test.csv");
    if (!file.good())
    {
        std::cout << "文件打开失败" << std::endl;
        return;
    };
    std::string str_line;
    while (std::getline(file, str_line)) 
    {   
        std::string str_grid;
        std::cout << str_line << std::endl;

        std::stringstream str_stream(str_line);
        while (std::getline(str_stream, str_grid, ',')) 
        {
            std::cout << str_grid << ' ' ;
        };
        std::cout << std::endl;

    }
    file.close();
}


int main() {
    test_csv();
    std::cout <<"========" << std::endl;
    test_json();
    
    // 初始化SDL
    SDL_Init(SDL_INIT_EVERYTHING);
    Mix_Init(MIX_INIT_OGG);
    TTF_Init();
    // 音频设置
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    // 窗口设置
    SDL_Window* windows = SDL_CreateWindow(u8"飞天小黑妖", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_SHOWN);
    // 渲染设置
    SDL_Renderer* renderer = SDL_CreateRenderer(windows, -1, SDL_RENDERER_ACCELERATED);

    // 图片处理:
    SDL_Surface* surface_img = IMG_Load(PhotoPath); // 加载图片: 文件中的数据加载到内存中
    if (surface_img == nullptr) {
        std::cout << "图片资源加载失败" << SDL_GetError() << std::endl;
        return 1;
    }
    else
    {
        std::cout << "图片资源加载成功" << std::endl;
    }
    SDL_Texture* texture_img = SDL_CreateTextureFromSurface(renderer, surface_img); // 创建纹理：内存中的数据上传到GPU变成纹理数据
    
    // 字体处理:
    TTF_Font* font = TTF_OpenFont(FontPath, 24); // 加载字体: 文件中的数据加载到内存中
    if (font == nullptr) {
        std::cout << "字体资源加载失败" << SDL_GetError() << std::endl;
        return 1;
    }
    else
    {
        std::cout << "字体资源加载成功" << std::endl;
    }
    SDL_Color color = {255, 255, 255, 255}; // 字体颜色（白色）
    SDL_Surface* surface_font = TTF_RenderUTF8_Blended(font, u8"飞天小黑妖准备开始制作游戏!!!", color); // 字体渲染
    SDL_Texture* texture_font = SDL_CreateTextureFromSurface(renderer, surface_font); // 创建纹理

    // 自定义FPS显示字体
    SDL_Surface* surface_font_fps = TTF_RenderUTF8_Solid(font, u8"FPS:", color); // 字体渲染
    SDL_Texture* texture_font2 = SDL_CreateTextureFromSurface(renderer, surface_font_fps); // 创建纹理

    // 音频处理:
    Mix_Music* music = Mix_LoadMUS(MusicPath); // 加载音频
    if (music == nullptr) {

        std::cout << "音频资源加载失败"<< SDL_GetError() << std::endl;
    }
    else {
        std::cout << "音频资源加载成功" << std::endl;
    }
    //Mix_PlayMusic(music, -1); // 循环播放音频
    Mix_FadeInMusic(music, -1, 1500); // 淡入播放音频

    // 事件
    SDL_Event event;
    // 记录鼠标位置
    SDL_Point mouse_pos = {0, 0};
    // 窗口大小: 用来存放图片和文本
    SDL_Rect rect_img = { 0, 0, surface_img->w, surface_img->h }, 
        rect_text = { 0, 0, surface_font->w, surface_font->h }, 
        rect_text_fps = { 0, 0, 100, 30};

    bool is_quite = false;

    int fps = 120; // 帧率
    Uint64 last_count = SDL_GetPerformanceCounter(); // 获取当前计数(高性能下计数器的计数)
    std::cout << "获取当前计数:" << last_count << std::endl;
    Uint64 counter_freq = SDL_GetPerformanceFrequency(); // 获取计数频率(每秒高性能下计数器的计数)
    std::cout << "获取计数频率:" << counter_freq << std::endl;
    Uint64 current_counter; // 当前帧计数
    
    // 游戏主循环
    while (!is_quite) {
        while (SDL_PollEvent(&event)) { // 检测事件
            if (event.type == SDL_QUIT) { // 退出事件
                is_quite = true;
            }
            else if (event.type == SDL_MOUSEMOTION) { // 鼠标移动事件
                mouse_pos.x = event.motion.x;
                mouse_pos.y = event.motion.y;
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN) { // 鼠标点击事件

            };
        };
        // 限制帧率: (当前帧计数-上一帧计数)/秒计数频率
        current_counter = SDL_GetPerformanceCounter();
        //std::cout << "Update当前计数:" << current_counter << std::endl;
        double delta_time = (double)(current_counter - last_count) / counter_freq;
        //std::cout << "计算后的时间差:" << delta_time << std::endl;
        last_count = current_counter;
        if (delta_time * 1000 < 1000.0 / fps) {
            //std::cout << "睡眠计算:" << delta_time * 1000 << 1000.0 / fps<<"实际睡眠时间:" << (1000.0 / fps - delta_time * 1000) << std::endl;
            SDL_Delay((Uint32)(1000.0 / fps - delta_time * 1000)); // 延迟
        };
        // 处理数据
        rect_img.x = mouse_pos.x;
        rect_img.y = mouse_pos.y;
        rect_text.x = mouse_pos.x-20;
        rect_text.y = mouse_pos.y-20;
        //std::string fps = std::string("FPS:") + std::to_string((int)(1.0/delta_time));
        //std::cout << fps << std::endl;
        //surface_font_fps = TTF_RenderUTF8_Solid(font, fps.c_str(), color);
        //texture_font2 = SDL_CreateTextureFromSurface(renderer, surface_font_fps); // 创建纹理

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // 设置渲染颜色（黑）
        // 清空上一帧整个窗口的绘图
        SDL_RenderClear(renderer);
        // 渲染绘图
        SDL_RenderCopy(renderer, texture_img, nullptr, &rect_img); // 绘制图片纹理
        filledCircleRGBA(renderer, mouse_pos.x, mouse_pos.y, 20, 255, 0, 0, 255); // 绘制圆形
        SDL_RenderCopy(renderer, texture_font, nullptr, &rect_text); // 绘制文字纹理
        SDL_RenderCopy(renderer, texture_font2, nullptr, &rect_text_fps); // 绘制文字纹理2
        SDL_RenderPresent(renderer); // 渲染
    };

    return 0;
}