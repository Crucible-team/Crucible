#include "stdafx.h"
#include "Editor.h"

#include "sdl2.h"
#include <fstream>

#include "Assets/Icon.c"

using namespace std;

int sdl_loop(Editor &editor)
{
    bool quit = false;
    while (!quit)
    {
        editor.Run();
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            bool textinput_action_delete = false;
            switch(event.type){
                case SDL_QUIT:
                    quit = true;
                    break;
                case SDL_WINDOWEVENT:
                    switch (event.window.event) {
                        case SDL_WINDOWEVENT_CLOSE: // exit editor
                            quit = true;
                            break;
                        case SDL_WINDOWEVENT_RESIZED:
                            // Tells the engine to reload window configuration (size and dpi)
                            editor.SetWindow(editor.window);
                            break;
                        case SDL_WINDOWEVENT_FOCUS_LOST:
                            editor.is_window_active = false;
                            break;
                        case SDL_WINDOWEVENT_FOCUS_GAINED:
                            editor.is_window_active = true;
                            if (wi::shadercompiler::GetRegisteredShaderCount() > 0)
                            {
                                std::thread([] {
                                    wi::backlog::post("[Shader check] Started checking " + std::to_string(wi::shadercompiler::GetRegisteredShaderCount()) + " registered shaders for changes...");
                                    if (wi::shadercompiler::CheckRegisteredShadersOutdated())
                                    {
                                        wi::backlog::post("[Shader check] Changes detected, initiating reload...");
                                        wi::eventhandler::Subscribe_Once(wi::eventhandler::EVENT_THREAD_SAFE_POINT, [](uint64_t userdata) {
                                            wi::renderer::ReloadShaders();
                                            });
                                    }
                                    else
                                    {
                                        wi::backlog::post("[Shader check] All up to date");
                                    }
                                    }).detach();
                            }
							editor.renderComponent.ReloadLanguage();
                            break;
                        default:
                            break;
                    }
                case SDL_KEYDOWN:
                    if(event.key.keysym.scancode == SDL_SCANCODE_BACKSPACE 
                        || event.key.keysym.scancode == SDL_SCANCODE_DELETE
                        || event.key.keysym.scancode == SDL_SCANCODE_KP_BACKSPACE){
                            wi::gui::TextInputField::DeleteFromInput();
                            textinput_action_delete = true;
                        }
                    break;
                case SDL_TEXTINPUT:
                    if(!textinput_action_delete){
                        if(event.text.text[0] >= 21){
                            wi::gui::TextInputField::AddInput(event.text.text[0]);
                        }
                    }
                    break;
                default:
                    break;
            }
            wi::input::sdlinput::ProcessEvent(event);
        }
    }

    return 0;
}

void set_window_icon(SDL_Window *window) {
    // these masks are needed to tell SDL_CreateRGBSurface(From)
    // to assume the data it gets is byte-wise RGB(A) data
    Uint32 rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    int shift = (gimp_image.bytes_per_pixel == 3) ? 8 : 0;
    rmask = 0xff000000 >> shift;
    gmask = 0x00ff0000 >> shift;
    bmask = 0x0000ff00 >> shift;
    amask = 0x000000ff >> shift;
#else // little endian, like x86
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = (gimp_image.bytes_per_pixel == 3) ? 0 : 0xff000000;
#endif
    SDL_Surface* icon = SDL_CreateRGBSurfaceFrom((void*)gimp_image.pixel_data, gimp_image.width,
        gimp_image.height, gimp_image.bytes_per_pixel*8, gimp_image.bytes_per_pixel*gimp_image.width,
        rmask, gmask, bmask, amask);

    SDL_SetWindowIcon(window, icon);
 
    SDL_FreeSurface(icon);
}

int main(int argc, char *argv[])
{
    
    wi::arguments::Parse(argc, argv);
	
	Gameconfig::getInstance().get_game_directories(wi::helper::GetCurrentPath() + "/games");

	Gameconfig::getInstance().currentgame = "default";


	wi::Timer timer;
	if (Gameconfig::getInstance().config.Open("gameconfig.ini"))
	{
		for (auto i = Gameconfig::getInstance().games.begin(); i != Gameconfig::getInstance().games.end(); i++)
		{
			std::string name = i->first;
			std::string name2 = i->second;

			Gameconfig::getInstance().config.GetSection(name.c_str()).Set("gamepath", name2);
			Gameconfig::getInstance().config.Commit();
			
			
			//wi::backlog::post("game: " + name + " , " + name2);
		}
		Gameconfig::getInstance().config.Commit();
		
		
		if (Gameconfig::getInstance().config.GetSection(Gameconfig::getInstance().currentgame.c_str()).Has("shadersourcepath"))
		{
			std::string path = Gameconfig::getInstance().config.GetSection(Gameconfig::getInstance().currentgame.c_str()).GetText("shadersourcepath");
			wi::helper::MakePathRelative(wi::helper::GetCurrentPath(), path);

			wi::renderer::SetShaderSourcePath(wi::helper::GetCurrentPath() + path);
		}
		else
		{
			wi::renderer::SetShaderSourcePath(wi::helper::GetCurrentPath() +"\\Data\\shaders\\");
		}
		
		if (Gameconfig::getInstance().config.GetSection(Gameconfig::getInstance().currentgame.c_str()).Has("shaderpath"))
		{
			std::string path = Gameconfig::getInstance().config.GetSection(Gameconfig::getInstance().currentgame.c_str()).GetText("shaderpath");
			wi::helper::MakePathRelative(wi::helper::GetCurrentPath(),path);
			
			wi::renderer::SetShaderPath(wi::helper::GetCurrentPath() + path);
		}
		else
		{
			wi::renderer::SetShaderPath(wi::helper::GetCurrentPath() + "\\shaders\\");
		}
	

		wi::backlog::post("gameconfig.ini loaded in " + std::to_string(timer.elapsed_milliseconds()) + " milliseconds\n");
	}
	else
	{
		wi::renderer::SetShaderSourcePath(wi::helper::GetCurrentPath() + "/Data/shaders/");
		wi::renderer::SetShaderPath(wi::helper::GetCurrentPath() + "/shaders/");
	}

	Gameconfig::getInstance().config.Commit();

    sdl2::sdlsystem_ptr_t system = sdl2::make_sdlsystem(SDL_INIT_EVERYTHING | SDL_INIT_EVENTS);
    if (!system) {
        throw sdl2::SDLError("Error creating SDL2 system");
    }

	int width = 1920;
	int height = 1080;
	bool fullscreen = false;

	wi::Timer timer;
	if (Editor::getInstance()config.Open("config.ini"))
	{
		if (Editor::getInstance().config.Has("width"))
		{
			width = Editor::getInstance().config.GetInt("width");
			height = Editor::getInstance().config.GetInt("height");
		}
		fullscreen = Editor::getInstance().config.GetBool("fullscreen");
		Editor::getInstance().allow_hdr = Editor::getInstance().config.GetBool("allow_hdr");

		wi::backlog::post("config.ini loaded in " + std::to_string(timer.elapsed_milliseconds()) + " milliseconds\n");
	}

	width = std::max(100, width);
	height = std::max(100, height);

    sdl2::window_ptr_t window = sdl2::make_window(
            "Wicked Engine Editor",
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            width, height,
            SDL_WINDOW_SHOWN | SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    if (!window) {
        throw sdl2::SDLError("Error creating window");
    }

    set_window_icon(window.get());

	if (fullscreen)
	{
		//SDL_SetWindowFullscreen(window.get(), SDL_TRUE);
		//SDL_SetWindowFullscreen(window.get(), SDL_WINDOW_FULLSCREEN);
		SDL_SetWindowFullscreen(window.get(), SDL_WINDOW_FULLSCREEN_DESKTOP);
	}

    Editor::getInstance()SetWindow(window.get());

    int ret = sdl_loop(Editor::getInstance());

    return ret;
}
