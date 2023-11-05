#include "stdafx.h"
#include "exec.h"
#include "wiRenderer.h"
#include <SDL2/SDL.h>
#include "Assets/Icon.c"

int sdl_loop(GameApp &gameapp)
{
    SDL_Event event;

    bool quit = false;
    while (!quit)
    {
        SDL_PumpEvents();
        gameapp.Run();

        while( SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_QUIT:
                    quit = true;
                    break;
                case SDL_WINDOWEVENT:
                    switch (event.window.event)
                    {
                    case SDL_WINDOWEVENT_CLOSE:
                        quit = true;
                        break;
                    case SDL_WINDOWEVENT_RESIZED:
                        gameapp.SetWindow(gameapp.window);
                        break;
                    default:
                        break;
                    }
                default:
                    break;
            }
        }
    }

    return 0;

}

void GameApp::Initialize()
{

	if (config.Has("font"))
	{
		// Replace default font from config:
		wi::font::AddFontStyle(config.GetText("font"));
	}

	Application::Initialize();

	// With this mode, file data for resources will be kept around. This allows serializing embedded resource data inside scenes
	wi::resourcemanager::SetMode(wi::resourcemanager::Mode::ALLOW_RETAIN_FILEDATA);

    infoDisplay.active = true;
    infoDisplay.watermark = true;
    infoDisplay.resolution = true;
    infoDisplay.fpsinfo = true;
	infoDisplay.heap_allocation_counter = true;
	infoDisplay.pipeline_count = true;
	infoDisplay.vram_usage = true;

	wi::backlog::setFontColor(wi::Color(255, 0, 0, 255));

	wi::renderer::SetOcclusionCullingEnabled(true);

	loader.Load();

	renderComponent.main = this;

	loader.addLoadingComponent(&renderComponent, this, 0.2f);

	ActivatePath(&loader);

}

void GameLoadingScreen::Load()
{
	font = wi::SpriteFont("Loading...", wi::font::Params(0, 0, 36, wi::font::WIFALIGN_LEFT, wi::font::WIFALIGN_CENTER));
	font.anim.typewriter.time = 2;
	font.anim.typewriter.looped = true;
	font.anim.typewriter.character_start = 7;
	AddFont(&font);

	sprite.anim.opa = 1;
	sprite.anim.repeatable = true;
	sprite.params.siz = XMFLOAT2(128, 128);
	sprite.params.pivot = XMFLOAT2(0.5f, 1.0f);
	sprite.params.quality = wi::image::QUALITY_LINEAR;
	sprite.params.blendFlag = wi::enums::BLENDMODE_ALPHA;
	AddSprite(&sprite);

	wi::gui::CheckBox::SetCheckTextGlobal(ICON_CHECK);

	LoadingScreen::Load();
}
void GameLoadingScreen::Update(float dt)
{
	font.params.posX = GetLogicalWidth()*0.5f - font.TextWidth() * 0.5f;
	font.params.posY = GetLogicalHeight()*0.5f;
	sprite.params.pos = XMFLOAT3(GetLogicalWidth()*0.5f, GetLogicalHeight()*0.5f - font.TextHeight(), 0);
	sprite.textureResource.SetTexture(*wi::texturehelper::getLogo()); // use embedded asset

	LoadingScreen::Update(dt);
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
    #ifdef Crucible_SHADER_DIR
    wi::renderer::SetShaderSourcePath(Crucible_SHADER_DIR);
    #endif

int width = 1920;
int height = 1080;
bool fullscreen = false;
wi::Timer timer;

	if (GameApp::getInstance().config.Open("config.ini"))
	{
		if (GameApp::getInstance().config.Has("width"))
		{
			width = GameApp::getInstance().config.GetInt("width");
			height = GameApp::getInstance().config.GetInt("height");
		}
		fullscreen = GameApp::getInstance().config.GetBool("fullscreen");
		GameApp::getInstance().allow_hdr = GameApp::getInstance().config.GetBool("allow_hdr");

		wi::backlog::post("config.ini loaded in " + std::to_string(timer.elapsed_milliseconds()) + " milliseconds\n");
	}

	width = std::max(100, width);
	height = std::max(100, height);

    sdl2::sdlsystem_ptr_t system = sdl2::make_sdlsystem(SDL_INIT_EVERYTHING | SDL_INIT_EVENTS);
    sdl2::window_ptr_t window = sdl2::make_window(
            "Welder",
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            width, height,
            SDL_WINDOW_SHOWN | SDL_WINDOW_VULKAN | SDL_WINDOW_ALLOW_HIGHDPI);
	if (!window) {
    	throw sdl2::SDLError("Error creating window");
    }

    SDL_Event event;

	set_window_icon(window.get());

	if (fullscreen)
	{
		//SDL_SetWindowFullscreen(window.get(), SDL_TRUE);
		//SDL_SetWindowFullscreen(window.get(), SDL_WINDOW_FULLSCREEN);
		SDL_SetWindowFullscreen(window.get(), SDL_WINDOW_FULLSCREEN_DESKTOP);
	}

	GameApp::getInstance().SetWindow(window.get());

    int ret = sdl_loop(GameApp::getInstance());

    SDL_Quit();

    return ret;
}
