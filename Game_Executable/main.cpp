#include "stdafx.h"
#include "exec.h"
#include "wiRenderer.h"
#include <SDL2/SDL.h>
#include "Assets/Icon.c"

using namespace wi::graphics;
using namespace wi::primitive;
using namespace wi::scene;
using namespace wi::ecs;

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

void GameComponent::ResizeBuffers()
{
	//optionsWnd.graphicsWnd.UpdateSwapChainFormats(&main->swapChain);

	init(main->canvas);
	RenderPath2D::ResizeBuffers();

	GraphicsDevice* device = wi::graphics::GetDevice();

	renderPath->init(*this);
	renderPath->ResizeBuffers();

	if(renderPath->GetDepthStencil() != nullptr)
	{
		bool success = false;

		XMUINT2 internalResolution = GetInternalResolution();

		TextureDesc desc;
		desc.width = internalResolution.x;
		desc.height = internalResolution.y;

		desc.format = Format::R8_UNORM;
		desc.bind_flags = BindFlag::RENDER_TARGET | BindFlag::SHADER_RESOURCE;
		if (renderPath->getMSAASampleCount() > 1)
		{
			desc.sample_count = renderPath->getMSAASampleCount();
			success = device->CreateTexture(&desc, nullptr, &rt_selectionOutline_MSAA);
			assert(success);
			desc.sample_count = 1;
		}
		success = device->CreateTexture(&desc, nullptr, &rt_selectionOutline[0]);
		assert(success);
		success = device->CreateTexture(&desc, nullptr, &rt_selectionOutline[1]);
		assert(success);
	}

	{
		TextureDesc desc;
		desc.width = renderPath->GetRenderResult().GetDesc().width;
		desc.height = renderPath->GetRenderResult().GetDesc().height;
		desc.format = Format::R8_UNORM;
		desc.bind_flags = BindFlag::RENDER_TARGET | BindFlag::SHADER_RESOURCE;
		desc.swizzle.r = ComponentSwizzle::R;
		desc.swizzle.g = ComponentSwizzle::R;
		desc.swizzle.b = ComponentSwizzle::R;
		desc.swizzle.a = ComponentSwizzle::R;
		device->CreateTexture(&desc, nullptr, &rt_dummyOutline);
		device->SetName(&rt_dummyOutline, "rt_dummyOutline");
	}

	{
		TextureDesc desc;
		desc.width = renderPath->GetRenderResult().GetDesc().width;
		desc.height = renderPath->GetRenderResult().GetDesc().height;
		desc.format = Format::D32_FLOAT;
		desc.bind_flags = BindFlag::DEPTH_STENCIL;
		desc.layout = ResourceState::DEPTHSTENCIL;
		desc.misc_flags = ResourceMiscFlag::TRANSIENT_ATTACHMENT;
		device->CreateTexture(&desc, nullptr, &editor_depthbuffer);
		device->SetName(&editor_depthbuffer, "editor_depthbuffer");
	}
}

void GameComponent::ResizeLayout()
{
	RenderPath2D::ResizeLayout();

	// GUI elements scaling:

	float screenW = GetLogicalWidth();
	float screenH = GetLogicalHeight();

	//optionsWnd.SetPos(XMFLOAT2(0, screenH - optionsWnd.GetScale().y));
	//optionsWnd.scale_local = wi::math::Clamp(optionsWnd.scale_local, XMFLOAT3(1, 1, 1), XMFLOAT3(screenW, screenH, 1));

	//componentsWnd.SetPos(XMFLOAT2(screenW - componentsWnd.GetScale().x, screenH - componentsWnd.GetScale().y));
	//componentsWnd.scale_local = wi::math::Clamp(componentsWnd.scale_local, XMFLOAT3(1, 1, 1), XMFLOAT3(screenW, screenH, 1));

	//aboutWindow.SetSize(XMFLOAT2(screenW / 2.0f, screenH / 1.5f));
	//aboutWindow.SetPos(XMFLOAT2(screenW / 2.0f - aboutWindow.scale.x / 2.0f, screenH / 2.0f - aboutWindow.scale.y / 2.0f));

}

void GameComponent::Load()
{

RenderPath2D::Load();
}

void GameComponent::Start()
{
	// Font icon is from #include "FontAwesomeV6.h"
	//	We will not directly use this font style, but let the font renderer fall back on it
	//	when an icon character is not found in the default font.
	//	This is added on main thread, not inside Load(), to avoid conflict with font system intialization
	wi::font::AddFontStyle("FontAwesomeV6", font_awesome_v6, font_awesome_v6_size);

	// Add other fonts that were loaded from fonts directory as fallback fonts:
	for (auto& x : font_datas)
	{
		wi::font::AddFontStyle(x.name, x.filedata.data(), x.filedata.size());
	}

	RenderPath2D::Start();
}

void GameComponent::PreUpdate()
{
	RenderPath2D::PreUpdate();

	renderPath->PreUpdate();
}

void GameComponent::FixedUpdate()
{
	RenderPath2D::FixedUpdate();

	renderPath->FixedUpdate();
}

void GameComponent::Update(float dt)
{
	wi::profiler::range_id profrange = wi::profiler::BeginRangeCPU("Editor Update");

	if (wi::input::Press(wi::input::KEYBOARD_BUTTON_F2))
	{
		std::string filename = wi::helper::screenshot(main->swapChain);
		if (filename.empty())
		{
			PostSaveText("Error! Screenshot was not successful!");
		}
		else
		{
			PostSaveText("Screenshot saved: ", filename);
		}
	}

	Scene& scene = GetCurrentScene();
	EditorScene& editorscene = GetCurrentEditorScene();
	CameraComponent& camera = editorscene.camera;

	translator.scene = &scene;

	if (scene.forces.Contains(grass_interaction_entity))
	{
		scene.Entity_Remove(grass_interaction_entity);
	}

	optionsWnd.Update(dt);
	componentsWnd.Update(dt);

	// Pulsating selection color update:
	outlineTimer += dt;

	CheckBonePickingEnabled();
	UpdateTopMenuAnimation();

	save_text_alpha = std::max(0.0f, save_text_alpha - std::min(dt, 0.033f)); // after saving, dt can become huge

	bool clear_selected = false;
	if (wi::input::Press(wi::input::KEYBOARD_BUTTON_ESCAPE))
	{
		if (!GetGUI().IsVisible())
		{
			// Exit cinema mode:
			if (renderPath != nullptr)
			{
				renderPath->GetGUI().SetVisible(true);
			}
			GetGUI().SetVisible(true);

			//If we are in fps toggle mode disable it if we hit escape to get out of cinema mode
			if (fpscamToggle)
			{
				fpscamToggle = false;
			}

		}
		else
		{
			clear_selected = true;
		}
	}

	translator.interactable = false;

	// Camera control:
	if (!wi::backlog::isActive() && !GetGUI().HasFocus())
	{
		deleting = wi::input::Press(wi::input::KEYBOARD_BUTTON_DELETE);
		translator.interactable = true;
		XMFLOAT4 currentMouse = wi::input::GetPointer();
		static XMFLOAT4 originalMouse = XMFLOAT4(0, 0, 0, 0);
		static bool camControlStart = true;
		if (camControlStart)
		{
			originalMouse = wi::input::GetPointer();
		}

		float xDif = 0, yDif = 0;

		if ((wi::input::Down(wi::input::MOUSE_BUTTON_MIDDLE) && !optionsWnd.cameraWnd.fpsCheckBox.GetCheck()))
		{
			camControlStart = false;
#if 0
			// Mouse delta from previous frame:
			xDif = currentMouse.x - originalMouse.x;
			yDif = currentMouse.y - originalMouse.y;
#else
			// Mouse delta from hardware read:
			xDif = wi::input::GetMouseState().delta_position.x;
			yDif = wi::input::GetMouseState().delta_position.y;
#endif
			xDif = 0.1f * xDif * (1.0f / 60.0f);
			yDif = 0.1f * yDif * (1.0f / 60.0f);
			wi::input::SetPointer(originalMouse);
			wi::input::HidePointer(true);
		}
		else
		{
			camControlStart = true;
			wi::input::HidePointer(false);
		}

		if ((wi::input::Down(wi::input::MOUSE_BUTTON_MIDDLE) && optionsWnd.cameraWnd.isFlycamAltCheckBox.GetCheck()))
		{
			camControlStart = false;
#if 0
			// Mouse delta from previous frame:
			xDif = currentMouse.x - originalMouse.x;
			yDif = currentMouse.y - originalMouse.y;
#else
			// Mouse delta from hardware read:
			xDif = wi::input::GetMouseState().delta_position.x;
			yDif = wi::input::GetMouseState().delta_position.y;
#endif
			xDif = 0.1f * xDif * (1.0f / 60.0f);
			yDif = 0.1f * yDif * (1.0f / 60.0f);
			wi::input::SetPointer(originalMouse);
			wi::input::HidePointer(true);
		}
		else
		{
			camControlStart = true;
			wi::input::HidePointer(false);
		}

		if ((!wi::input::Down(wi::input::KEYBOARD_BUTTON_LCONTROL) && wi::input::Press((wi::input::BUTTON)'Z') && optionsWnd.cameraWnd.fpsCheckBox.GetCheck() && !optionsWnd.cameraWnd.isFlycamAltCheckBox.GetCheck()))
		{
			fpscamToggle = !fpscamToggle;
		}

		if (fpscamToggle)
		{
			camControlStart = false;
#if 0
			// Mouse delta from previous frame:
			xDif = currentMouse.x - originalMouse.x;
			yDif = currentMouse.y - originalMouse.y;
#else
			// Mouse delta from hardware read:
			xDif = wi::input::GetMouseState().delta_position.x;
			yDif = wi::input::GetMouseState().delta_position.y;
#endif
			xDif = 0.1f * xDif * (1.0f / 60.0f);
			yDif = 0.1f * yDif * (1.0f / 60.0f);
			wi::input::SetPointer(originalMouse);
			wi::input::HidePointer(true);
		}
		else if ((!wi::input::Down(wi::input::MOUSE_BUTTON_MIDDLE)) && !fpscamToggle)
		{
			camControlStart = true;
			wi::input::HidePointer(false);
		}




		const float buttonrotSpeed = 2.0f * dt;
		if (wi::input::Down(wi::input::KEYBOARD_BUTTON_LEFT) || wi::input::Down(wi::input::KEYBOARD_BUTTON_NUMPAD4))
		{
			xDif -= buttonrotSpeed;
		}
		if (wi::input::Down(wi::input::KEYBOARD_BUTTON_RIGHT) || wi::input::Down(wi::input::KEYBOARD_BUTTON_NUMPAD6))
		{
			xDif += buttonrotSpeed;
		}
		if (wi::input::Down(wi::input::KEYBOARD_BUTTON_UP) || wi::input::Down(wi::input::KEYBOARD_BUTTON_NUMPAD8))
		{
			yDif -= buttonrotSpeed;
		}
		if (wi::input::Down(wi::input::KEYBOARD_BUTTON_DOWN) || wi::input::Down(wi::input::KEYBOARD_BUTTON_NUMPAD2))
		{
			yDif += buttonrotSpeed;
		}

		const XMFLOAT4 leftStick = wi::input::GetAnalog(wi::input::GAMEPAD_ANALOG_THUMBSTICK_L, 0);
		const XMFLOAT4 rightStick = wi::input::GetAnalog(wi::input::GAMEPAD_ANALOG_THUMBSTICK_R, 0);
		const XMFLOAT4 rightTrigger = wi::input::GetAnalog(wi::input::GAMEPAD_ANALOG_TRIGGER_R, 0);

		const float jostickrotspeed = 0.05f;
		xDif += rightStick.x * jostickrotspeed;
		yDif += rightStick.y * jostickrotspeed;

		xDif *= optionsWnd.cameraWnd.rotationspeedSlider.GetValue();
		yDif *= optionsWnd.cameraWnd.rotationspeedSlider.GetValue();


		if (optionsWnd.cameraWnd.fpsCheckBox.GetCheck())
		{
			// FPS Camera
			const float clampedDT = std::min(dt, 0.1f); // if dt > 100 millisec, don't allow the camera to jump too far...

			const float speed = ((wi::input::Down(wi::input::KEYBOARD_BUTTON_LSHIFT) ? 10.0f : 1.0f) + rightTrigger.x * 10.0f) * optionsWnd.cameraWnd.movespeedSlider.GetValue() * clampedDT;
			XMVECTOR move = XMLoadFloat3(&editorscene.cam_move);
			XMVECTOR moveNew = XMVectorSet(0, 0, 0, 0);

			if (!wi::input::Down(wi::input::KEYBOARD_BUTTON_LCONTROL))
			{
				// Only move camera if control not pressed
				if (wi::input::Down((wi::input::BUTTON)'A') || wi::input::Down(wi::input::GAMEPAD_BUTTON_LEFT)) { moveNew += XMVectorSet(-1, 0, 0, 0); }
				if (wi::input::Down((wi::input::BUTTON)'D') || wi::input::Down(wi::input::GAMEPAD_BUTTON_RIGHT)) { moveNew += XMVectorSet(1, 0, 0, 0); }
				if (wi::input::Down((wi::input::BUTTON)'W') || wi::input::Down(wi::input::GAMEPAD_BUTTON_UP)) { moveNew += XMVectorSet(0, 0, 1, 0); }
				if (wi::input::Down((wi::input::BUTTON)'S') || wi::input::Down(wi::input::GAMEPAD_BUTTON_DOWN)) { moveNew += XMVectorSet(0, 0, -1, 0); }
				if (wi::input::Down((wi::input::BUTTON)'E') || wi::input::Down(wi::input::GAMEPAD_BUTTON_2)) { moveNew += XMVectorSet(0, 1, 0, 0); }
				if (wi::input::Down((wi::input::BUTTON)'Q') || wi::input::Down(wi::input::GAMEPAD_BUTTON_1)) { moveNew += XMVectorSet(0, -1, 0, 0); }

				moveNew = XMVector3Normalize(moveNew);
			}
			moveNew += XMVectorSet(leftStick.x, 0, leftStick.y, 0);
			moveNew *= speed;

			move = XMVectorLerp(move, moveNew, optionsWnd.cameraWnd.accelerationSlider.GetValue() * clampedDT / 0.0166f); // smooth the movement a bit
			float moveLength = XMVectorGetX(XMVector3Length(move));

			if (moveLength < 0.0001f)
			{
				move = XMVectorSet(0, 0, 0, 0);
			}

			if (abs(xDif) + abs(yDif) > 0 || moveLength > 0.0001f)
			{
				XMMATRIX camRot = XMMatrixRotationQuaternion(XMLoadFloat4(&editorscene.camera_transform.rotation_local));
				XMVECTOR move_rot = XMVector3TransformNormal(move, camRot);
				XMFLOAT3 _move;
				XMStoreFloat3(&_move, move_rot);
				editorscene.camera_transform.Translate(_move);
				editorscene.camera_transform.RotateRollPitchYaw(XMFLOAT3(yDif, xDif, 0));
				camera.SetDirty();
			}

			editorscene.camera_transform.UpdateTransform();
			XMStoreFloat3(&editorscene.cam_move, move);
		}
		else
		{
			// Orbital Camera

			if (wi::input::Down(wi::input::KEYBOARD_BUTTON_LSHIFT))
			{
				XMVECTOR V = XMVectorAdd(camera.GetRight() * xDif, camera.GetUp() * yDif) * 10;
				XMFLOAT3 vec;
				XMStoreFloat3(&vec, V);
				editorscene.camera_target.Translate(vec);
			}
			else if (wi::input::Down(wi::input::KEYBOARD_BUTTON_LCONTROL) || currentMouse.z != 0.0f)
			{
				editorscene.camera_transform.Translate(XMFLOAT3(0, 0, yDif * 4 + currentMouse.z));
				editorscene.camera_transform.translation_local.z = std::min(0.0f, editorscene.camera_transform.translation_local.z);
				camera.SetDirty();
			}
			else if (abs(xDif) + abs(yDif) > 0)
			{
				editorscene.camera_target.RotateRollPitchYaw(XMFLOAT3(yDif * 2, xDif * 2, 0));
				camera.SetDirty();
			}

			editorscene.camera_target.UpdateTransform();
			editorscene.camera_transform.UpdateTransform_Parented(editorscene.camera_target);
		}

		if (!translator.selected.empty() && wi::input::Down((wi::input::BUTTON)'F'))
		{
			FocusCameraOnSelected();
		}

		inspector_mode = wi::input::Down((wi::input::BUTTON)'I');

		// Begin picking:
		Ray pickRay = wi::renderer::GetPickRay((long)currentMouse.x, (long)currentMouse.y, *this, camera);
		{
			hovered = wi::scene::PickResult();

			if (has_flag(optionsWnd.filter, OptionsWindow::Filter::Light))
			{
				for (size_t i = 0; i < scene.lights.GetCount(); ++i)
				{
					Entity entity = scene.lights.GetEntity(i);
					const LightComponent& light = scene.lights[i];

					XMVECTOR disV = XMVector3LinePointDistance(XMLoadFloat3(&pickRay.origin), XMLoadFloat3(&pickRay.origin) + XMLoadFloat3(&pickRay.direction), XMLoadFloat3(&light.position));
					float dis = XMVectorGetX(disV);
					if (dis > 0.01f && dis < wi::math::Distance(light.position, pickRay.origin) * 0.05f && dis < hovered.distance)
					{
						hovered = wi::scene::PickResult();
						hovered.entity = entity;
						hovered.distance = dis;
					}
					else
					{
						if (light.GetType() == LightComponent::DIRECTIONAL || light.GetType() == LightComponent::SPOT)
						{
							// Light direction visualizer picking:
							const float dist = wi::math::Distance(light.position, camera.Eye);
							float siz = 0.02f * dist;
							const XMMATRIX M =
								XMMatrixScaling(siz, siz, siz) *
								XMMatrixRotationZ(-XM_PIDIV2) *
								XMMatrixRotationQuaternion(XMLoadFloat4(&light.rotation)) *
								XMMatrixTranslation(light.position.x, light.position.y, light.position.z)
								;

							const float origin_size = 0.4f * siz;
							const float axis_length = 18;

							XMFLOAT3 base;
							XMFLOAT3 tip;
							XMStoreFloat3(&base, XMVector3Transform(XMVectorSet(0, 0, 0, 1), M));
							XMStoreFloat3(&tip, XMVector3Transform(XMVectorSet(axis_length, 0, 0, 1), M));
							Capsule capsule = Capsule(base, tip, origin_size);
							if (pickRay.intersects(capsule, dis))
							{
								if (dis < hovered.distance)
								{
									hovered = wi::scene::PickResult();
									hovered.entity = entity;
									hovered.distance = dis;
								}
							}
						}
					}
				}
			}
			if (has_flag(optionsWnd.filter, OptionsWindow::Filter::Decal))
			{
				for (size_t i = 0; i < scene.decals.GetCount(); ++i)
				{
					Entity entity = scene.decals.GetEntity(i);
					if (!scene.transforms.Contains(entity))
						continue;
					const TransformComponent& transform = *scene.transforms.GetComponent(entity);

					XMVECTOR disV = XMVector3LinePointDistance(XMLoadFloat3(&pickRay.origin), XMLoadFloat3(&pickRay.origin) + XMLoadFloat3(&pickRay.direction), transform.GetPositionV());
					float dis = XMVectorGetX(disV);
					if (dis > 0.01f && dis < wi::math::Distance(transform.GetPosition(), pickRay.origin) * 0.05f && dis < hovered.distance)
					{
						hovered = wi::scene::PickResult();
						hovered.entity = entity;
						hovered.distance = dis;
					}
				}
			}
			if (has_flag(optionsWnd.filter, OptionsWindow::Filter::Force))
			{
				for (size_t i = 0; i < scene.forces.GetCount(); ++i)
				{
					Entity entity = scene.forces.GetEntity(i);
					if (!scene.transforms.Contains(entity))
						continue;
					const TransformComponent& transform = *scene.transforms.GetComponent(entity);

					XMVECTOR disV = XMVector3LinePointDistance(XMLoadFloat3(&pickRay.origin), XMLoadFloat3(&pickRay.origin) + XMLoadFloat3(&pickRay.direction), transform.GetPositionV());
					float dis = XMVectorGetX(disV);
					if (dis > 0.01f && dis < wi::math::Distance(transform.GetPosition(), pickRay.origin) * 0.05f && dis < hovered.distance)
					{
						hovered = wi::scene::PickResult();
						hovered.entity = entity;
						hovered.distance = dis;
					}
				}
			}
			if (has_flag(optionsWnd.filter, OptionsWindow::Filter::Emitter))
			{
				for (size_t i = 0; i < scene.emitters.GetCount(); ++i)
				{
					Entity entity = scene.emitters.GetEntity(i);
					if (!scene.transforms.Contains(entity))
						continue;
					const TransformComponent& transform = *scene.transforms.GetComponent(entity);

					XMVECTOR disV = XMVector3LinePointDistance(XMLoadFloat3(&pickRay.origin), XMLoadFloat3(&pickRay.origin) + XMLoadFloat3(&pickRay.direction), transform.GetPositionV());
					float dis = XMVectorGetX(disV);
					if (dis > 0.01f && dis < wi::math::Distance(transform.GetPosition(), pickRay.origin) * 0.05f && dis < hovered.distance)
					{
						hovered = wi::scene::PickResult();
						hovered.entity = entity;
						hovered.distance = dis;
					}
				}
			}
			if (has_flag(optionsWnd.filter, OptionsWindow::Filter::Hairparticle))
			{
				for (size_t i = 0; i < scene.hairs.GetCount(); ++i)
				{
					Entity entity = scene.hairs.GetEntity(i);
					if (!scene.transforms.Contains(entity))
						continue;
					const TransformComponent& transform = *scene.transforms.GetComponent(entity);

					XMVECTOR disV = XMVector3LinePointDistance(XMLoadFloat3(&pickRay.origin), XMLoadFloat3(&pickRay.origin) + XMLoadFloat3(&pickRay.direction), transform.GetPositionV());
					float dis = XMVectorGetX(disV);
					if (dis > 0.01f && dis < wi::math::Distance(transform.GetPosition(), pickRay.origin) * 0.05f && dis < hovered.distance)
					{
						hovered = wi::scene::PickResult();
						hovered.entity = entity;
						hovered.distance = dis;
					}
				}
			}
			if (has_flag(optionsWnd.filter, OptionsWindow::Filter::EnvironmentProbe))
			{
				for (size_t i = 0; i < scene.probes.GetCount(); ++i)
				{
					Entity entity = scene.probes.GetEntity(i);
					if (!scene.transforms.Contains(entity))
						continue;
					const TransformComponent& transform = *scene.transforms.GetComponent(entity);

					if (Sphere(transform.GetPosition(), 1).intersects(pickRay))
					{
						float dis = wi::math::Distance(transform.GetPosition(), pickRay.origin);
						if (dis < hovered.distance)
						{
							hovered = wi::scene::PickResult();
							hovered.entity = entity;
							hovered.distance = dis;
						}
					}
				}
			}
			if (has_flag(optionsWnd.filter, OptionsWindow::Filter::Camera))
			{
				for (size_t i = 0; i < scene.cameras.GetCount(); ++i)
				{
					Entity entity = scene.cameras.GetEntity(i);
					if (!scene.transforms.Contains(entity))
						continue;
					const TransformComponent& transform = *scene.transforms.GetComponent(entity);

					XMVECTOR disV = XMVector3LinePointDistance(XMLoadFloat3(&pickRay.origin), XMLoadFloat3(&pickRay.origin) + XMLoadFloat3(&pickRay.direction), transform.GetPositionV());
					float dis = XMVectorGetX(disV);
					if (dis > 0.01f && dis < wi::math::Distance(transform.GetPosition(), pickRay.origin) * 0.05f && dis < hovered.distance)
					{
						hovered = wi::scene::PickResult();
						hovered.entity = entity;
						hovered.distance = dis;
					}
				}
			}
			if (has_flag(optionsWnd.filter, OptionsWindow::Filter::Armature))
			{
				for (size_t i = 0; i < scene.armatures.GetCount(); ++i)
				{
					Entity entity = scene.armatures.GetEntity(i);
					if (!scene.transforms.Contains(entity))
						continue;
					const TransformComponent& transform = *scene.transforms.GetComponent(entity);

					XMVECTOR disV = XMVector3LinePointDistance(XMLoadFloat3(&pickRay.origin), XMLoadFloat3(&pickRay.origin) + XMLoadFloat3(&pickRay.direction), transform.GetPositionV());
					float dis = XMVectorGetX(disV);
					if (dis > 0.01f && dis < wi::math::Distance(transform.GetPosition(), pickRay.origin) * 0.05f && dis < hovered.distance)
					{
						hovered = wi::scene::PickResult();
						hovered.entity = entity;
						hovered.distance = dis;
					}
				}
			}
			if (has_flag(optionsWnd.filter, OptionsWindow::Filter::Sound))
			{
				for (size_t i = 0; i < scene.sounds.GetCount(); ++i)
				{
					Entity entity = scene.sounds.GetEntity(i);
					if (!scene.transforms.Contains(entity))
						continue;
					const TransformComponent& transform = *scene.transforms.GetComponent(entity);

					XMVECTOR disV = XMVector3LinePointDistance(XMLoadFloat3(&pickRay.origin), XMLoadFloat3(&pickRay.origin) + XMLoadFloat3(&pickRay.direction), transform.GetPositionV());
					float dis = XMVectorGetX(disV);
					if (dis > 0.01f && dis < wi::math::Distance(transform.GetPosition(), pickRay.origin) * 0.05f && dis < hovered.distance)
					{
						hovered = wi::scene::PickResult();
						hovered.entity = entity;
						hovered.distance = dis;
					}
				}
			}
			if (has_flag(optionsWnd.filter, OptionsWindow::Filter::Video))
			{
				for (size_t i = 0; i < scene.videos.GetCount(); ++i)
				{
					Entity entity = scene.videos.GetEntity(i);
					if (!scene.transforms.Contains(entity))
						continue;
					const TransformComponent& transform = *scene.transforms.GetComponent(entity);

					XMVECTOR disV = XMVector3LinePointDistance(XMLoadFloat3(&pickRay.origin), XMLoadFloat3(&pickRay.origin) + XMLoadFloat3(&pickRay.direction), transform.GetPositionV());
					float dis = XMVectorGetX(disV);
					if (dis > 0.01f && dis < wi::math::Distance(transform.GetPosition(), pickRay.origin) * 0.05f && dis < hovered.distance)
					{
						hovered = wi::scene::PickResult();
						hovered.entity = entity;
						hovered.distance = dis;
					}
				}
			}
			if (bone_picking)
			{
				for (size_t i = 0; i < scene.armatures.GetCount(); ++i)
				{
					const ArmatureComponent& armature = scene.armatures[i];
					for (Entity entity : armature.boneCollection)
					{
						if (!scene.transforms.Contains(entity))
							continue;
						const TransformComponent& transform = *scene.transforms.GetComponent(entity);
						XMVECTOR a = transform.GetPositionV();
						XMVECTOR b = a + XMVectorSet(0, 0.1f, 0, 0);
						// Search for child to connect bone tip:
						bool child_found = false;
						for (size_t h = 0; (h < scene.humanoids.GetCount()) && !child_found; ++h)
						{
							const HumanoidComponent& humanoid = scene.humanoids[h];
							int bodypart = 0;
							for (Entity child : humanoid.bones)
							{
								const HierarchyComponent* hierarchy = scene.hierarchy.GetComponent(child);
								if (hierarchy != nullptr && hierarchy->parentID == entity && scene.transforms.Contains(child))
								{
									if (bodypart == int(HumanoidComponent::HumanoidBone::Hips))
									{
										// skip root-hip connection
										child_found = true;
										break;
									}
									const TransformComponent& child_transform = *scene.transforms.GetComponent(child);
									b = child_transform.GetPositionV();
									child_found = true;
									break;
								}
								bodypart++;
							}
						}
						if (!child_found)
						{
							for (Entity child : armature.boneCollection)
							{
								const HierarchyComponent* hierarchy = scene.hierarchy.GetComponent(child);
								if (hierarchy != nullptr && hierarchy->parentID == entity && scene.transforms.Contains(child))
								{
									const TransformComponent& child_transform = *scene.transforms.GetComponent(child);
									b = child_transform.GetPositionV();
									child_found = true;
									break;
								}
							}
						}
						if (!child_found)
						{
							// No child, try to guess bone tip compared to parent (if it has parent):
							const HierarchyComponent* hierarchy = scene.hierarchy.GetComponent(entity);
							if (hierarchy != nullptr && scene.transforms.Contains(hierarchy->parentID))
							{
								const TransformComponent& parent_transform = *scene.transforms.GetComponent(hierarchy->parentID);
								XMVECTOR ab = a - parent_transform.GetPositionV();
								b = a + ab;
							}
						}
						XMVECTOR ab = XMVector3Normalize(b - a);

						wi::primitive::Capsule capsule;
						capsule.radius = wi::math::Distance(a, b) * 0.1f;
						a -= ab * capsule.radius;
						b += ab * capsule.radius;
						XMStoreFloat3(&capsule.base, a);
						XMStoreFloat3(&capsule.tip, b);

						float dis = -1;
						if (pickRay.intersects(capsule, dis) && dis < hovered.distance)
						{
							hovered = wi::scene::PickResult();
							hovered.entity = entity;
							hovered.distance = dis;
						}
					}
				}
			}

			if (hovered.entity == INVALID_ENTITY)
			{
				// Object picking only when mouse button down, because it can be slow with high polycount
				if (
					wi::input::Down(wi::input::MOUSE_BUTTON_LEFT) ||
					wi::input::Down(wi::input::MOUSE_BUTTON_RIGHT) ||
					optionsWnd.paintToolWnd.GetMode() != PaintToolWindow::MODE_DISABLED ||
					inspector_mode
					)
				{
					hovered = wi::scene::Pick(pickRay, wi::enums::FILTER_OBJECT_ALL, ~0u, scene);
				}
			}
		}

		if (hovered.entity != INVALID_ENTITY &&
			wi::input::Down((wi::input::BUTTON)'C') &&
			wi::input::Down(wi::input::MOUSE_BUTTON_LEFT))
		{
			camera.focal_length = hovered.distance;
			camera.SetDirty();
			optionsWnd.cameraWnd.focalLengthSlider.SetValue(camera.focal_length);
		}

		// Interactions only when paint tool is disabled:
		if (optionsWnd.paintToolWnd.GetMode() == PaintToolWindow::MODE_DISABLED)
		{
			// Interact:
			if (wi::input::Down((wi::input::BUTTON)'P'))
			{
				if (wi::input::Press(wi::input::MOUSE_BUTTON_LEFT))
				{
					// Physics impulse tester:
					wi::physics::RayIntersectionResult result = wi::physics::Intersects(scene, pickRay);
					if (result.IsValid())
					{
						XMFLOAT3 impulse;
						XMStoreFloat3(&impulse, XMVector3Normalize(XMLoadFloat3(&pickRay.direction)) * 20);
						if (result.humanoid_ragdoll_entity != INVALID_ENTITY)
						{
							// Ragdoll:
							HumanoidComponent* humanoid = scene.humanoids.GetComponent(result.humanoid_ragdoll_entity);
							if (humanoid != nullptr)
							{
								humanoid->SetRagdollPhysicsEnabled(true);
								wi::physics::ApplyImpulse(*humanoid, result.humanoid_bone, impulse);
							}
						}
						else
						{
							// Rigidbody:
							RigidBodyPhysicsComponent* rigidbody = scene.rigidbodies.GetComponent(result.entity);
							if (rigidbody != nullptr)
							{
								wi::physics::ApplyImpulse(*rigidbody, impulse);
							}
						}
					}
				}
			}
			else
			{
				// Physics pick dragger:
				if (wi::input::Down(wi::input::MOUSE_BUTTON_LEFT))
				{
					wi::physics::PickDrag(scene, pickRay, physicsDragOp);
				}
				else
				{
					physicsDragOp = {};
				}
			}

			// Other:
			if (hovered.entity != INVALID_ENTITY && wi::input::Down(wi::input::MOUSE_BUTTON_LEFT))
			{
				if (dummy_enabled && translator.selected.empty())
				{
					dummy_pos = hovered.position;
				}
				else
				{
					const ObjectComponent* object = scene.objects.GetComponent(hovered.entity);
					if (object != nullptr)
					{
						if (translator.selected.empty() && object->GetFilterMask() & wi::enums::FILTER_WATER)
						{
							// if water, then put a water ripple onto it:
							scene.PutWaterRipple("../Content/models/ripple.png", hovered.position);
						}
						else if (componentsWnd.decalWnd.IsEnabled() && componentsWnd.decalWnd.placementCheckBox.GetCheck() && wi::input::Press(wi::input::MOUSE_BUTTON_LEFT))
						{
							// if not water, put a decal on it:
							Entity entity = scene.Entity_CreateDecal("editorDecal", "");
							// material and decal parameters will be copied from selected:
							if (scene.decals.Contains(componentsWnd.decalWnd.entity))
							{
								*scene.decals.GetComponent(entity) = *scene.decals.GetComponent(componentsWnd.decalWnd.entity);
							}
							if (scene.materials.Contains(componentsWnd.decalWnd.entity))
							{
								*scene.materials.GetComponent(entity) = *scene.materials.GetComponent(componentsWnd.decalWnd.entity);
							}
							// place it on picked surface:
							TransformComponent& transform = *scene.transforms.GetComponent(entity);
							transform.MatrixTransform(hovered.orientation);
							transform.RotateRollPitchYaw(XMFLOAT3(XM_PIDIV2, 0, 0));
							scene.Component_Attach(entity, hovered.entity);

							wi::Archive& archive = AdvanceHistory();
							archive << EditorComponent::HISTORYOP_ADD;
							RecordSelection(archive);
							RecordSelection(archive);
							RecordEntity(archive, entity);

							optionsWnd.RefreshEntityTree();
						}
						else if (translator.selected.empty())
						{
							// Check for interactive grass (hair particle that is child of hovered object:
							for (size_t i = 0; i < scene.hairs.GetCount(); ++i)
							{
								Entity entity = scene.hairs.GetEntity(i);
								HierarchyComponent* hier = scene.hierarchy.GetComponent(entity);
								if (hier != nullptr && hier->parentID == hovered.entity)
								{
									XMVECTOR P = XMLoadFloat3(&hovered.position);
									P += XMLoadFloat3(&hovered.normal) * 2;
									if (grass_interaction_entity == INVALID_ENTITY)
									{
										grass_interaction_entity = CreateEntity();
									}
									ForceFieldComponent& force = scene.forces.Create(grass_interaction_entity);
									TransformComponent& transform = scene.transforms.Create(grass_interaction_entity);
									force.type = ForceFieldComponent::Type::Point;
									force.gravity = -80;
									force.range = 3;
									transform.Translate(P);
									break;
								}
							}
						}
					}
				}

			}
		}

		// Select...
		if (wi::input::Press(wi::input::MOUSE_BUTTON_RIGHT) || selectAll || clear_selected)
		{

			wi::Archive& archive = AdvanceHistory();
			archive << HISTORYOP_SELECTION;
			// record PREVIOUS selection state...
			RecordSelection(archive);

			if (selectAll)
			{
				// Add everything to selection:
				selectAll = false;
				ClearSelected();

				selectAllStorage.clear();
				scene.FindAllEntities(selectAllStorage);
				for (auto& entity : selectAllStorage)
				{
					AddSelected(entity);
				}
			}
			else if (hovered.entity != INVALID_ENTITY)
			{
				// Add the hovered item to the selection:

				if (!translator.selected.empty() && wi::input::Down(wi::input::KEYBOARD_BUTTON_LSHIFT))
				{
					// Union selection:
					wi::vector<wi::scene::PickResult> saved = translator.selected;
					translator.selected.clear();
					for (const wi::scene::PickResult& picked : saved)
					{
						AddSelected(picked);
					}
					AddSelected(hovered);
				}
				else
				{
					// Replace selection:
					translator.selected.clear();
					AddSelected(hovered);
				}
			}
			else
			{
				clear_selected = true;
			}

			if (clear_selected)
			{
				ClearSelected();
			}


			// record NEW selection state...
			RecordSelection(archive);

			optionsWnd.RefreshEntityTree();
		}

	}

	main->infoDisplay.colorgrading_helper = false;

	// Control operations...
	if (!GetGUI().IsTyping() && wi::input::Down(wi::input::KEYBOARD_BUTTON_LCONTROL) || wi::input::Down(wi::input::KEYBOARD_BUTTON_RCONTROL))
	{
		// Color Grading helper
		if (wi::input::Down((wi::input::BUTTON)'G'))
		{
			main->infoDisplay.colorgrading_helper = true;
		}
		// Toggle wireframe mode
		if (wi::input::Press((wi::input::BUTTON)'W'))
		{
			wi::renderer::SetWireRender(!wi::renderer::IsWireRender());
			optionsWnd.generalWnd.wireFrameCheckBox.SetCheck(wi::renderer::IsWireRender());
		}
		// Enable transform tool
		if (wi::input::Press((wi::input::BUTTON)'T'))
		{
			translator.SetEnabled(!translator.IsEnabled());
		}
		// Save
		if (wi::input::Press((wi::input::BUTTON)'S'))
		{
			if (wi::input::Down(wi::input::KEYBOARD_BUTTON_LSHIFT) || wi::input::Down(wi::input::KEYBOARD_BUTTON_RSHIFT) || GetCurrentEditorScene().path.empty())
			{
				SaveAs();
			}
			else
			{
				Save(GetCurrentEditorScene().path);
			}
		}
		// Select All
		if (wi::input::Press((wi::input::BUTTON)'A'))
		{
			selectAll = true;
		}
		// Copy/Cut
		if (wi::input::Press((wi::input::BUTTON)'C') || wi::input::Press((wi::input::BUTTON)'X'))
		{
			auto& prevSel = translator.selectedEntitiesNonRecursive;

			EntitySerializer seri;
			clipboard.SetReadModeAndResetPos(false);
			clipboard << prevSel.size();
			for (auto& x : prevSel)
			{
				scene.Entity_Serialize(clipboard, seri, x);
			}

			if (wi::input::Press((wi::input::BUTTON)'X'))
			{
				deleting = true;
			}
		}
		// Paste
		if (wi::input::Press((wi::input::BUTTON)'V'))
		{
			wi::Archive& archive = AdvanceHistory();
			archive << HISTORYOP_ADD;
			RecordSelection(archive);

			ClearSelected();

			EntitySerializer seri;
			clipboard.SetReadModeAndResetPos(true);
			size_t count;
			clipboard >> count;
			wi::vector<Entity> addedEntities;
			for (size_t i = 0; i < count; ++i)
			{
				wi::scene::PickResult picked;
				picked.entity = scene.Entity_Serialize(clipboard, seri, INVALID_ENTITY, Scene::EntitySerializeFlags::RECURSIVE);
				AddSelected(picked);
				addedEntities.push_back(picked.entity);
			}

			RecordSelection(archive);
			RecordEntity(archive, addedEntities);

			optionsWnd.RefreshEntityTree();
		}
		// Duplicate Instances
		if (wi::input::Press((wi::input::BUTTON)'D'))
		{
			wi::Archive& archive = AdvanceHistory();
			archive << HISTORYOP_ADD;
			RecordSelection(archive);

			auto& prevSel = translator.selectedEntitiesNonRecursive;
			wi::vector<Entity> addedEntities;
			for (auto& x : prevSel)
			{
				wi::scene::PickResult picked;
				picked.entity = scene.Entity_Duplicate(x);
				addedEntities.push_back(picked.entity);
			}

			ClearSelected();

			for (auto& x : addedEntities)
			{
				AddSelected(x);
			}

			RecordSelection(archive);
			RecordEntity(archive, addedEntities);

			optionsWnd.RefreshEntityTree();
		}
		// Put Instances
		if (clipboard.IsOpen() && hovered.subsetIndex >= 0 && wi::input::Down(wi::input::KEYBOARD_BUTTON_LSHIFT) && wi::input::Press(wi::input::MOUSE_BUTTON_LEFT))
		{
			wi::vector<Entity> addedEntities;
			EntitySerializer seri;
			clipboard.SetReadModeAndResetPos(true);
			size_t count;
			clipboard >> count;
			for (size_t i = 0; i < count; ++i)
			{
				Entity entity = scene.Entity_Serialize(clipboard, seri, INVALID_ENTITY, Scene::EntitySerializeFlags::RECURSIVE | Scene::EntitySerializeFlags::KEEP_INTERNAL_ENTITY_REFERENCES);
				const HierarchyComponent* hier = scene.hierarchy.GetComponent(entity);
				if (hier != nullptr)
				{
					scene.Component_Detach(entity);
				}
				TransformComponent* transform = scene.transforms.GetComponent(entity);
				if (transform != nullptr)
				{
					transform->translation_local = {};
#if 0
					// orient around surface normal:
					transform->MatrixTransform(hovered.orientation);
#else
					// orient in random vertical rotation only:
					transform->RotateRollPitchYaw(XMFLOAT3(0, wi::random::GetRandom(XM_PI), 0));
					transform->Translate(hovered.position);
#endif
					transform->UpdateTransform();
				}
				if (hier != nullptr)
				{
					scene.Component_Attach(entity, hier->parentID);
				}
				addedEntities.push_back(entity);
			}

			wi::Archive& archive = AdvanceHistory();
			archive << HISTORYOP_ADD;
			// because selection didn't change here, we record same selection state twice, it's not a bug:
			RecordSelection(archive);
			RecordSelection(archive);
			RecordEntity(archive, addedEntities);

			optionsWnd.RefreshEntityTree();
		}
		// Undo
		if (wi::input::Down(wi::input::KEYBOARD_BUTTON_LCONTROL) && wi::input::Press((wi::input::BUTTON)'Z') &&
			!wi::input::Down(wi::input::KEYBOARD_BUTTON_LSHIFT) &&
			!wi::input::Down(wi::input::KEYBOARD_BUTTON_RSHIFT))
		{
			ConsumeHistoryOperation(true);

			optionsWnd.RefreshEntityTree();
		}
		// Redo
		if (wi::input::Down(wi::input::KEYBOARD_BUTTON_LCONTROL) && wi::input::Press((wi::input::BUTTON)'Y') )
		{
			ConsumeHistoryOperation(false);

			optionsWnd.RefreshEntityTree();
		}
	}

	if (!wi::backlog::isActive() && !GetGUI().IsTyping())
	{
		if (wi::input::Press(wi::input::BUTTON('1')))
		{
			translator.isTranslator = !translator.isTranslator;
			translator.isScalator = false;
			translator.isRotator = false;
		}
		else if (wi::input::Press(wi::input::BUTTON('2')))
		{
			translator.isRotator = !translator.isRotator;
			translator.isScalator = false;
			translator.isTranslator = false;
		}
		else if (wi::input::Press(wi::input::BUTTON('3')))
		{
			translator.isScalator = !translator.isScalator;
			translator.isTranslator = false;
			translator.isRotator = false;
		}
	}

	// Delete
	if (deleting)
	{
		deleting = false;
		wi::Archive& archive = AdvanceHistory();
		archive << HISTORYOP_DELETE;
		RecordSelection(archive);

		archive << translator.selectedEntitiesNonRecursive;
		EntitySerializer seri;
		for (auto& x : translator.selectedEntitiesNonRecursive)
		{
			scene.Entity_Serialize(archive, seri, x);
		}
		for (auto& x : translator.selectedEntitiesNonRecursive)
		{
			scene.Entity_Remove(x);
		}

		ClearSelected();

		optionsWnd.RefreshEntityTree();
	}

	// Update window data bindings...
	if (translator.selected.empty())
	{
		optionsWnd.cameraWnd.SetEntity(INVALID_ENTITY);
		componentsWnd.objectWnd.SetEntity(INVALID_ENTITY);
		componentsWnd.emitterWnd.SetEntity(INVALID_ENTITY);
		componentsWnd.hairWnd.SetEntity(INVALID_ENTITY);
		componentsWnd.meshWnd.SetEntity(INVALID_ENTITY, -1);
		componentsWnd.materialWnd.SetEntity(INVALID_ENTITY);
		componentsWnd.soundWnd.SetEntity(INVALID_ENTITY);
		componentsWnd.lightWnd.SetEntity(INVALID_ENTITY);
		componentsWnd.videoWnd.SetEntity(INVALID_ENTITY);
		componentsWnd.decalWnd.SetEntity(INVALID_ENTITY);
		componentsWnd.envProbeWnd.SetEntity(INVALID_ENTITY);
		componentsWnd.forceFieldWnd.SetEntity(INVALID_ENTITY);
		componentsWnd.springWnd.SetEntity(INVALID_ENTITY);
		componentsWnd.ikWnd.SetEntity(INVALID_ENTITY);
		componentsWnd.transformWnd.SetEntity(INVALID_ENTITY);
		componentsWnd.layerWnd.SetEntity(INVALID_ENTITY);
		componentsWnd.nameWnd.SetEntity(INVALID_ENTITY);
		componentsWnd.weatherWnd.SetEntity(INVALID_ENTITY);
		componentsWnd.animWnd.SetEntity(INVALID_ENTITY);
		componentsWnd.scriptWnd.SetEntity(INVALID_ENTITY);
		componentsWnd.rigidWnd.SetEntity(INVALID_ENTITY);
		componentsWnd.softWnd.SetEntity(INVALID_ENTITY);
		componentsWnd.colliderWnd.SetEntity(INVALID_ENTITY);
		componentsWnd.hierarchyWnd.SetEntity(INVALID_ENTITY);
		componentsWnd.cameraComponentWnd.SetEntity(INVALID_ENTITY);
		componentsWnd.expressionWnd.SetEntity(INVALID_ENTITY);
		componentsWnd.armatureWnd.SetEntity(INVALID_ENTITY);
		componentsWnd.humanoidWnd.SetEntity(INVALID_ENTITY);
		componentsWnd.terrainWnd.SetEntity(INVALID_ENTITY);

		componentsWnd.ioWnd.SetEntity(INVALID_ENTITY);
		componentsWnd.responseWnd.SetEntity(INVALID_ENTITY);

		componentsWnd.healthWnd.SetEntity(INVALID_ENTITY);
		componentsWnd.armorWnd.SetEntity(INVALID_ENTITY);
		componentsWnd.splineWnd.SetEntity(INVALID_ENTITY);
		componentsWnd.spriteWnd.SetEntity(INVALID_ENTITY);
		componentsWnd.fontWnd.SetEntity(INVALID_ENTITY);
	}
	else
	{
		const wi::scene::PickResult& picked = translator.selected.back();

		assert(picked.entity != INVALID_ENTITY);
		optionsWnd.cameraWnd.SetEntity(picked.entity);
		componentsWnd.emitterWnd.SetEntity(picked.entity);
		componentsWnd.hairWnd.SetEntity(picked.entity);
		componentsWnd.lightWnd.SetEntity(picked.entity);
		componentsWnd.soundWnd.SetEntity(picked.entity);
		componentsWnd.videoWnd.SetEntity(picked.entity);
		componentsWnd.decalWnd.SetEntity(picked.entity);
		componentsWnd.envProbeWnd.SetEntity(picked.entity);
		componentsWnd.forceFieldWnd.SetEntity(picked.entity);
		componentsWnd.springWnd.SetEntity(picked.entity);
		componentsWnd.ikWnd.SetEntity(picked.entity);
		componentsWnd.transformWnd.SetEntity(picked.entity);
		componentsWnd.layerWnd.SetEntity(picked.entity);
		componentsWnd.nameWnd.SetEntity(picked.entity);
		componentsWnd.weatherWnd.SetEntity(picked.entity);
		componentsWnd.animWnd.SetEntity(picked.entity);
		componentsWnd.scriptWnd.SetEntity(picked.entity);
		componentsWnd.rigidWnd.SetEntity(picked.entity);
		componentsWnd.colliderWnd.SetEntity(picked.entity);
		componentsWnd.hierarchyWnd.SetEntity(picked.entity);
		componentsWnd.cameraComponentWnd.SetEntity(picked.entity);
		componentsWnd.expressionWnd.SetEntity(picked.entity);
		componentsWnd.armatureWnd.SetEntity(picked.entity);
		componentsWnd.humanoidWnd.SetEntity(picked.entity);
		componentsWnd.terrainWnd.SetEntity(picked.entity);
		componentsWnd.spriteWnd.SetEntity(picked.entity);
		componentsWnd.fontWnd.SetEntity(picked.entity);

		componentsWnd.ioWnd.SetEntity(picked.entity);
		componentsWnd.responseWnd.SetEntity(picked.entity);

		componentsWnd.healthWnd.SetEntity(picked.entity);
		componentsWnd.armorWnd.SetEntity(picked.entity);
		componentsWnd.splineWnd.SetEntity(picked.entity);

		if (picked.subsetIndex >= 0)
		{
			const ObjectComponent* object = scene.objects.GetComponent(picked.entity);
			if (object != nullptr) // maybe it was deleted...
			{
				componentsWnd.objectWnd.SetEntity(picked.entity);
				componentsWnd.meshWnd.SetEntity(object->meshID, picked.subsetIndex);
				componentsWnd.softWnd.SetEntity(object->meshID);

				const MeshComponent* mesh = scene.meshes.GetComponent(object->meshID);
				if (mesh != nullptr && (int)mesh->subsets.size() > picked.subsetIndex)
				{
					componentsWnd.materialWnd.SetEntity(mesh->subsets[picked.subsetIndex].materialID);
				}
			}
		}
		else
		{
			bool found_object = false;
			bool found_mesh = false;
			bool found_soft = false;
			bool found_material = false;
			for (auto& x : translator.selected)
			{
				if (!found_object && scene.objects.Contains(x.entity))
				{
					componentsWnd.objectWnd.SetEntity(x.entity);
					found_object = true;
				}
				if (!found_mesh && scene.meshes.Contains(x.entity))
				{
					componentsWnd.meshWnd.SetEntity(x.entity, 0);
					found_mesh = true;
				}
				if (!found_soft && scene.softbodies.Contains(x.entity))
				{
					componentsWnd.softWnd.SetEntity(x.entity);
					found_soft = true;
				}
				if (!found_material && scene.materials.Contains(x.entity))
				{
					componentsWnd.materialWnd.SetEntity(x.entity);
					found_material = true;
				}
			}

			if (!found_object)
			{
				componentsWnd.objectWnd.SetEntity(INVALID_ENTITY);
			}
			if (!found_mesh)
			{
				componentsWnd.meshWnd.SetEntity(INVALID_ENTITY, -1);
			}
			if (!found_soft)
			{
				componentsWnd.softWnd.SetEntity(INVALID_ENTITY);
			}
			if (!found_material)
			{
				componentsWnd.materialWnd.SetEntity(INVALID_ENTITY);
			}
		}

	}

	// Clear highlight state:
	for (size_t i = 0; i < scene.materials.GetCount(); ++i)
	{
		scene.materials[i].SetUserStencilRef(EDITORSTENCILREF_CLEAR);
	}
	for (size_t i = 0; i < scene.objects.GetCount(); ++i)
	{
		scene.objects[i].SetUserStencilRef(EDITORSTENCILREF_CLEAR);
	}
	for (auto& x : translator.selected)
	{
		ObjectComponent* object = scene.objects.GetComponent(x.entity);
		if (object != nullptr) // maybe it was deleted...
		{
			object->SetUserStencilRef(EDITORSTENCILREF_HIGHLIGHT_OBJECT);
			if (x.subsetIndex >= 0)
			{
				const MeshComponent* mesh = scene.meshes.GetComponent(object->meshID);
				if (mesh != nullptr && (int)mesh->subsets.size() > x.subsetIndex)
				{
					MaterialComponent* material = scene.materials.GetComponent(mesh->subsets[x.subsetIndex].materialID);
					if (material != nullptr)
					{
						material->SetUserStencilRef(EDITORSTENCILREF_HIGHLIGHT_MATERIAL);
					}
				}
			}
		}
	}

	if (translator.IsDragEnded())
	{
		EntitySerializer seri;
		wi::Archive& archive = AdvanceHistory();
		archive << HISTORYOP_TRANSLATOR;
		archive << translator.isTranslator;
		archive << translator.isRotator;
		archive << translator.isScalator;
		translator.transform_start.Serialize(archive, seri);
		translator.transform.Serialize(archive, seri);
		archive << translator.matrices_start;
		archive << translator.matrices_current;
	}

	componentsWnd.emitterWnd.UpdateData();
	componentsWnd.hairWnd.UpdateData();

	// Follow camera proxy:
	if (optionsWnd.cameraWnd.followCheckBox.IsEnabled() && optionsWnd.cameraWnd.followCheckBox.GetCheck())
	{
		TransformComponent* proxy = scene.transforms.GetComponent(optionsWnd.cameraWnd.entity);
		if (proxy != nullptr)
		{
			editorscene.camera_transform.Lerp(editorscene.camera_transform, *proxy, 1.0f - optionsWnd.cameraWnd.followSlider.GetValue());
			editorscene.camera_transform.UpdateTransform();
		}

		CameraComponent* proxy_camera = scene.cameras.GetComponent(optionsWnd.cameraWnd.entity);
		if (proxy_camera != nullptr)
		{
			editorscene.camera.Lerp(editorscene.camera, *proxy_camera, 1.0f - optionsWnd.cameraWnd.followSlider.GetValue());
		}
	}

	camera.TransformCamera(editorscene.camera_transform);
	camera.UpdateCamera();

	wi::RenderPath3D_PathTracing* pathtracer = dynamic_cast<wi::RenderPath3D_PathTracing*>(renderPath.get());
	if (pathtracer != nullptr)
	{
		pathtracer->setTargetSampleCount((int)optionsWnd.graphicsWnd.pathTraceTargetSlider.GetValue());

		std::string ss;
		ss += "Sample count: " + std::to_string(pathtracer->getCurrentSampleCount()) + "\n";
		ss += "Trace progress: " + std::to_string(int(pathtracer->getProgress() * 100)) + "%\n";
		if (pathtracer->isDenoiserAvailable())
		{
			if (pathtracer->getDenoiserProgress() > 0)
			{
				ss += "Denoiser progress: " + std::to_string(int(pathtracer->getDenoiserProgress() * 100)) + "%\n";
			}
		}
		else
		{
			ss += "Denoiser not available!\nTo find out how to enable the denoiser, visit the documentation.";
		}
		optionsWnd.graphicsWnd.pathTraceStatisticsLabel.SetText(ss);
	}

	wi::profiler::EndRange(profrange);

	RenderPath2D::Update(dt);

	if (optionsWnd.paintToolWnd.GetMode() == PaintToolWindow::MODE::MODE_DISABLED)
	{
		translator.Update(camera, *this);
	}

	renderPath->colorspace = colorspace;
	renderPath->Update(dt);


	bool force_collider_visualizer = false;
	for (auto& x : translator.selected)
	{
		if (scene.colliders.Contains(x.entity))
		{
			force_collider_visualizer = true;
		}
	}
	if (force_collider_visualizer)
	{
		wi::renderer::SetToDrawDebugColliders(true);
	}
	else
	{
		wi::renderer::SetToDrawDebugColliders(optionsWnd.generalWnd.colliderVisCheckBox.GetCheck());
	}
}

void GameComponent::PostUpdate()
{
	RenderPath2D::PostUpdate();

	renderPath->PostUpdate();
}

void GameComponent::Render() const
{
	const Scene& scene = GetCurrentScene();

	// Hovered item boxes:
	if (GetGUI().IsVisible())
	{
		if (hovered.entity != INVALID_ENTITY)
		{
			const ObjectComponent* object = scene.objects.GetComponent(hovered.entity);
			if (object != nullptr)
			{
				const AABB& aabb = scene.aabb_objects[scene.objects.GetIndex(hovered.entity)];

				XMFLOAT4X4 hoverBox;
				XMStoreFloat4x4(&hoverBox, aabb.getAsBoxMatrix());
				wi::renderer::DrawBox(hoverBox, XMFLOAT4(0.5f, 0.5f, 0.5f, 0.5f));
			}

			const LightComponent* light = scene.lights.GetComponent(hovered.entity);
			if (light != nullptr)
			{
				const AABB& aabb = scene.aabb_lights[scene.lights.GetIndex(hovered.entity)];

				XMFLOAT4X4 hoverBox;
				XMStoreFloat4x4(&hoverBox, aabb.getAsBoxMatrix());
				wi::renderer::DrawBox(hoverBox, XMFLOAT4(0.5f, 0.5f, 0, 0.5f));
			}

			const DecalComponent* decal = scene.decals.GetComponent(hovered.entity);
			if (decal != nullptr)
			{
				wi::renderer::DrawBox(decal->world, XMFLOAT4(0.5f, 0, 0.5f, 0.5f));
			}

			const EnvironmentProbeComponent* probe = scene.probes.GetComponent(hovered.entity);
			if (probe != nullptr)
			{
				const AABB& aabb = scene.aabb_probes[scene.probes.GetIndex(hovered.entity)];

				XMFLOAT4X4 hoverBox;
				XMStoreFloat4x4(&hoverBox, aabb.getAsBoxMatrix());
				wi::renderer::DrawBox(hoverBox, XMFLOAT4(0.5f, 0.5f, 0.5f, 0.5f));
			}

			const wi::HairParticleSystem* hair = scene.hairs.GetComponent(hovered.entity);
			if (hair != nullptr)
			{
				XMFLOAT4X4 hoverBox;
				XMStoreFloat4x4(&hoverBox, hair->aabb.getAsBoxMatrix());
				wi::renderer::DrawBox(hoverBox, XMFLOAT4(0, 0.5f, 0, 0.5f));
			}
		}

		// Spring visualizer:
		if (componentsWnd.springWnd.debugCheckBox.GetCheck())
		{
			for (size_t i = 0; i < scene.springs.GetCount(); ++i)
			{
				const SpringComponent& spring = scene.springs[i];
				wi::primitive::Sphere sphere;
				sphere.center = spring.currentTail;
				sphere.radius = spring.hitRadius;
				wi::renderer::DrawSphere(sphere, XMFLOAT4(1, 1, 0, 1));
			}
		}
	}

	// Selected items box:
	if (GetGUI().IsVisible() && !translator.selected.empty())
	{
		AABB selectedAABB = AABB(
			XMFLOAT3(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max()),
			XMFLOAT3(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest()));
		for (auto& picked : translator.selected)
		{
			if (picked.entity != INVALID_ENTITY)
			{
				const ObjectComponent* object = scene.objects.GetComponent(picked.entity);
				if (object != nullptr)
				{
					const AABB& aabb = scene.aabb_objects[scene.objects.GetIndex(picked.entity)];
					selectedAABB = AABB::Merge(selectedAABB, aabb);
				}

				const LightComponent* light = scene.lights.GetComponent(picked.entity);
				if (light != nullptr)
				{
					const AABB& aabb = scene.aabb_lights[scene.lights.GetIndex(picked.entity)];
					selectedAABB = AABB::Merge(selectedAABB, aabb);
				}

				const DecalComponent* decal = scene.decals.GetComponent(picked.entity);
				if (decal != nullptr)
				{
					const AABB& aabb = scene.aabb_decals[scene.decals.GetIndex(picked.entity)];
					selectedAABB = AABB::Merge(selectedAABB, aabb);

					// also display decal OBB:
					XMFLOAT4X4 selectionBox;
					selectionBox = decal->world;
					wi::renderer::DrawBox(selectionBox, XMFLOAT4(1, 0, 1, 1));
				}

				const EnvironmentProbeComponent* probe = scene.probes.GetComponent(picked.entity);
				if (probe != nullptr)
				{
					const AABB& aabb = scene.aabb_probes[scene.probes.GetIndex(picked.entity)];
					selectedAABB = AABB::Merge(selectedAABB, aabb);
				}

				const wi::HairParticleSystem* hair = scene.hairs.GetComponent(picked.entity);
				if (hair != nullptr)
				{
					selectedAABB = AABB::Merge(selectedAABB, hair->aabb);
				}

			}
		}

		XMFLOAT4X4 selectionBox;
		XMStoreFloat4x4(&selectionBox, selectedAABB.getAsBoxMatrix());
		wi::renderer::DrawBox(selectionBox, XMFLOAT4(1, 1, 1, 1));
	}

	renderPath->Render();

	// Editor custom render:
	if (GetGUI().IsVisible())
	{
		GraphicsDevice* device = wi::graphics::GetDevice();
		CommandList cmd = device->BeginCommandList();
		device->EventBegin("Editor", cmd);

		// Selection outline:
		if (renderPath->GetDepthStencil() != nullptr && !translator.selected.empty())
		{
			device->EventBegin("Selection Outline Mask", cmd);

			Viewport vp;
			vp.width = (float)rt_selectionOutline[0].GetDesc().width;
			vp.height = (float)rt_selectionOutline[0].GetDesc().height;
			device->BindViewports(1, &vp, cmd);

			wi::image::Params fx;
			fx.enableFullScreen();
			fx.stencilComp = wi::image::STENCILMODE::STENCILMODE_EQUAL;

			// We will specify the stencil ref in user-space, don't care about engine stencil refs here:
			//	Otherwise would need to take into account engine ref and draw multiple permutations of stencil refs.
			fx.stencilRefMode = wi::image::STENCILREFMODE_USER;

			// Materials outline:
			{
				if (renderPath->getMSAASampleCount() > 1)
				{
					RenderPassImage rp[] = {
						RenderPassImage::RenderTarget(&rt_selectionOutline_MSAA, RenderPassImage::LoadOp::CLEAR, RenderPassImage::StoreOp::DONTCARE),
						RenderPassImage::Resolve(&rt_selectionOutline[0]),
						RenderPassImage::DepthStencil(
							renderPath->GetDepthStencil(),
							RenderPassImage::LoadOp::LOAD,
							RenderPassImage::StoreOp::STORE
						),
					};
					device->RenderPassBegin(rp, arraysize(rp), cmd);
				}
				else
				{
					RenderPassImage rp[] = {
						RenderPassImage::RenderTarget(&rt_selectionOutline[0], RenderPassImage::LoadOp::CLEAR),
						RenderPassImage::DepthStencil(
							renderPath->GetDepthStencil(),
							RenderPassImage::LoadOp::LOAD,
							RenderPassImage::StoreOp::STORE
						),
					};
					device->RenderPassBegin(rp, arraysize(rp), cmd);
				}

				// Draw solid blocks of selected materials
				fx.stencilRef = EDITORSTENCILREF_HIGHLIGHT_MATERIAL;
				wi::image::Draw(wi::texturehelper::getWhite(), fx, cmd);

				device->RenderPassEnd(cmd);
			}

			// Objects outline:
			{
				if (renderPath->getMSAASampleCount() > 1)
				{
					RenderPassImage rp[] = {
						RenderPassImage::RenderTarget(&rt_selectionOutline_MSAA, RenderPassImage::LoadOp::CLEAR, RenderPassImage::StoreOp::DONTCARE),
						RenderPassImage::Resolve(&rt_selectionOutline[1]),
						RenderPassImage::DepthStencil(
							renderPath->GetDepthStencil(),
							RenderPassImage::LoadOp::LOAD,
							RenderPassImage::StoreOp::STORE
						),
					};
					device->RenderPassBegin(rp, arraysize(rp), cmd);
				}
				else
				{
					RenderPassImage rp[] = {
						RenderPassImage::RenderTarget(&rt_selectionOutline[1], RenderPassImage::LoadOp::CLEAR),
						RenderPassImage::DepthStencil(
							renderPath->GetDepthStencil(),
							RenderPassImage::LoadOp::LOAD,
							RenderPassImage::StoreOp::STORE
						),
					};
					device->RenderPassBegin(rp, arraysize(rp), cmd);
				}

				// Draw solid blocks of selected objects
				fx.stencilRef = EDITORSTENCILREF_HIGHLIGHT_OBJECT;
				wi::image::Draw(wi::texturehelper::getWhite(), fx, cmd);

				device->RenderPassEnd(cmd);
			}

			device->EventEnd(cmd);
		}

		// Reference dummy render:
		if(dummy_enabled)
		{
			device->EventBegin("Reference Dummy", cmd);
			static PipelineState pso;
			if (!pso.IsValid())
			{
				static auto LoadShaders = [] {
					PipelineStateDesc desc;
					desc.vs = wi::renderer::GetShader(wi::enums::VSTYPE_VERTEXCOLOR);
					desc.ps = wi::renderer::GetShader(wi::enums::PSTYPE_VERTEXCOLOR);
					desc.il = wi::renderer::GetInputLayout(wi::enums::ILTYPE_VERTEXCOLOR);
					desc.dss = wi::renderer::GetDepthStencilState(wi::enums::DSSTYPE_DEPTHDISABLED);
					desc.rs = wi::renderer::GetRasterizerState(wi::enums::RSTYPE_DOUBLESIDED);
					desc.bs = wi::renderer::GetBlendState(wi::enums::BSTYPE_TRANSPARENT);
					desc.pt = PrimitiveTopology::TRIANGLELIST;
					wi::graphics::GetDevice()->CreatePipelineState(&desc, &pso);
				};
				static wi::eventhandler::Handle handle = wi::eventhandler::Subscribe(wi::eventhandler::EVENT_RELOAD_SHADERS, [](uint64_t userdata) { LoadShaders(); });
				LoadShaders();
			}
			struct Vertex
			{
				XMFLOAT4 position;
				XMFLOAT4 color;
			};

			const float3* vertices = dummy_female::vertices;
			size_t vertices_size = sizeof(dummy_female::vertices);
			size_t vertices_count = arraysize(dummy_female::vertices);
			const unsigned int* indices = dummy_female::indices;
			size_t indices_size = sizeof(dummy_female::indices);
			size_t indices_count = arraysize(dummy_female::indices);
			if (dummy_male)
			{
				vertices = dummy_male::vertices;
				vertices_size = sizeof(dummy_male::vertices);
				vertices_count = arraysize(dummy_male::vertices);
				indices = dummy_male::indices;
				indices_size = sizeof(dummy_male::indices);
				indices_count = arraysize(dummy_male::indices);
			}

			static GPUBuffer dummyBuffers[2];
			if (!dummyBuffers[dummy_male].IsValid())
			{
				auto fill_data = [&](void* data) {
					Vertex* gpu_vertices = (Vertex*)data;
					for (size_t i = 0; i < vertices_count; ++i)
					{
						Vertex vert = {};
						vert.position.x = vertices[i].x;
						vert.position.y = vertices[i].y;
						vert.position.z = vertices[i].z;
						vert.position.w = 1;
						vert.color = XMFLOAT4(1, 1, 1, 1);
						std::memcpy(gpu_vertices + i, &vert, sizeof(vert));
					}

					uint32_t* gpu_indices = (uint32_t*)(gpu_vertices + vertices_count);
					std::memcpy(gpu_indices, indices, indices_size);
				};

				GPUBufferDesc desc;
				desc.size = indices_count * sizeof(uint32_t) + vertices_count * sizeof(Vertex);
				desc.bind_flags = BindFlag::INDEX_BUFFER | BindFlag::VERTEX_BUFFER;
				device->CreateBuffer2(&desc, fill_data, &dummyBuffers[dummy_male]);
				device->SetName(&dummyBuffers[dummy_male], "dummyBuffer");
			}

			RenderPassImage rp[] = {
				RenderPassImage::RenderTarget(&rt_dummyOutline, RenderPassImage::LoadOp::CLEAR),
			};
			device->RenderPassBegin(rp, arraysize(rp), cmd);

			Viewport vp;
			vp.width = (float)rt_dummyOutline.GetDesc().width;
			vp.height = (float)rt_dummyOutline.GetDesc().height;
			device->BindViewports(1, &vp, cmd);

			device->BindPipelineState(&pso, cmd);

			// remove camera jittering
			CameraComponent cam = *renderPath->camera;
			cam.jitter = XMFLOAT2(0, 0);
			cam.UpdateCamera();
			const XMMATRIX VP = cam.GetViewProjection();

			MiscCB sb;
			XMStoreFloat4x4(&sb.g_xTransform, XMMatrixTranslation(dummy_pos.x, dummy_pos.y, dummy_pos.z) * VP);
			sb.g_xColor = XMFLOAT4(1, 1, 1, 1);
			device->BindDynamicConstantBuffer(sb, CB_GETBINDSLOT(MiscCB), cmd);

			const GPUBuffer* vbs[] = {
				&dummyBuffers[dummy_male],
			};
			const uint32_t strides[] = {
				sizeof(Vertex),
			};
			const uint64_t offsets[] = {
				0,
			};
			device->BindVertexBuffers(vbs, 0, arraysize(vbs), strides, offsets, cmd);
			device->BindIndexBuffer(&dummyBuffers[dummy_male], IndexBufferFormat::UINT32, vertices_count * sizeof(Vertex), cmd);
			device->DrawIndexed((uint32_t)indices_count, 0, 0, cmd);
			device->RenderPassEnd(cmd);
			device->EventEnd(cmd);
		}

		// Full resolution:
		{
			const Texture& render_result = renderPath->GetRenderResult();
			RenderPassImage rp[] = {
				RenderPassImage::RenderTarget(
					&render_result,
					RenderPassImage::LoadOp::CLEAR,
					RenderPassImage::StoreOp::STORE
				),
				RenderPassImage::DepthStencil(
					&editor_depthbuffer,
					RenderPassImage::LoadOp::CLEAR,
					RenderPassImage::StoreOp::DONTCARE
				),
			};
			device->RenderPassBegin(rp, arraysize(rp), cmd);

			Viewport vp;
			vp.width = (float)editor_depthbuffer.GetDesc().width;
			vp.height = (float)editor_depthbuffer.GetDesc().height;
			device->BindViewports(1, &vp, cmd);

			// Selection color:
			float selectionColorIntensity = std::sin(outlineTimer * XM_2PI * 0.8f) * 0.5f + 0.5f;
			XMFLOAT4 glow = wi::math::Lerp(wi::math::Lerp(XMFLOAT4(1, 1, 1, 1), selectionColor, 0.4f), selectionColor, selectionColorIntensity);
			wi::Color selectedEntityColor = wi::Color::fromFloat4(glow);

			// Draw selection outline to the screen:
			if (renderPath->GetDepthStencil() != nullptr && !translator.selected.empty())
			{
				device->EventBegin("Selection Outline Edge", cmd);
				wi::renderer::BindCommonResources(cmd);
				float opacity = wi::math::Lerp(0.4f, 1.0f, selectionColorIntensity);
				XMFLOAT4 col = selectionColor2;
				col.w *= opacity;
				wi::renderer::Postprocess_Outline(rt_selectionOutline[0], cmd, 0.1f, 1, col);
				col = selectionColor;
				col.w *= opacity;
				wi::renderer::Postprocess_Outline(rt_selectionOutline[1], cmd, 0.1f, 1, col);
				device->EventEnd(cmd);
			}

			if (dummy_enabled)
			{
				wi::image::Params fx;
				fx.enableFullScreen();
				fx.blendFlag = wi::enums::BLENDMODE_PREMULTIPLIED;
				fx.color = XMFLOAT4(0, 0, 0, 0.4f);
				wi::image::Draw(&rt_dummyOutline, fx, cmd);
				XMFLOAT4 dummyColorBlinking = dummyColor;
				dummyColorBlinking.w = wi::math::Lerp(0.4f, 1, selectionColorIntensity);
				wi::renderer::Postprocess_Outline(rt_dummyOutline, cmd, 0.1f, 1, dummyColorBlinking);
			}

			const CameraComponent& camera = GetCurrentEditorScene().camera;

			const Scene& scene = GetCurrentScene();

			// remove camera jittering
			CameraComponent cam = *renderPath->camera;
			cam.jitter = XMFLOAT2(0, 0);
			cam.UpdateCamera();
			const XMMATRIX VP = cam.GetViewProjection();

			MiscCB sb;
			XMStoreFloat4x4(&sb.g_xTransform, VP);
			sb.g_xColor = XMFLOAT4(1, 1, 1, 1);
			device->BindDynamicConstantBuffer(sb, CB_GETBINDSLOT(MiscCB), cmd);

			const XMMATRIX R = XMLoadFloat3x3(&cam.rotationMatrix);

			wi::font::Params fp;
			fp.customRotation = &R;
			fp.customProjection = &VP;
			fp.size = 32; // icon font render quality
			const float scaling = 0.0025f;
			fp.h_align = wi::font::WIFALIGN_CENTER;
			fp.v_align = wi::font::WIFALIGN_CENTER;
			fp.shadowColor = backgroundEntityColor;
			fp.shadow_softness = 1;

			if (has_flag(optionsWnd.filter, OptionsWindow::Filter::Light))
			{
				for (size_t i = 0; i < scene.lights.GetCount(); ++i)
				{
					const LightComponent& light = scene.lights[i];
					Entity entity = scene.lights.GetEntity(i);
					if (!scene.transforms.Contains(entity))
						continue;

					const float dist = wi::math::Distance(light.position, camera.Eye);

					fp.position = light.position;
					fp.scaling = scaling * dist;
					fp.color = inactiveEntityColor;

					if (hovered.entity == entity)
					{
						fp.color = hoveredEntityColor;
					}
					for (auto& picked : translator.selected)
					{
						if (picked.entity == entity)
						{
							fp.color = selectedEntityColor;
							break;
						}
					}

					switch (light.GetType())
					{
					case LightComponent::POINT:
						wi::font::Draw(ICON_POINTLIGHT, fp, cmd);
						break;
					case LightComponent::SPOT:
						wi::font::Draw(ICON_SPOTLIGHT, fp, cmd);
						break;
					case LightComponent::DIRECTIONAL:
						wi::font::Draw(ICON_DIRECTIONALLIGHT, fp, cmd);
						break;
					default:
						break;
					}

					if (light.GetType() == LightComponent::DIRECTIONAL || light.GetType() == LightComponent::SPOT)
					{
						// Light direction visualizer:
						device->EventBegin("LightDirectionVisualizer", cmd);
						static PipelineState pso;
						if (!pso.IsValid())
						{
							static auto LoadShaders = [] {
								PipelineStateDesc desc;
								desc.vs = wi::renderer::GetShader(wi::enums::VSTYPE_VERTEXCOLOR);
								desc.ps = wi::renderer::GetShader(wi::enums::PSTYPE_VERTEXCOLOR);
								desc.il = wi::renderer::GetInputLayout(wi::enums::ILTYPE_VERTEXCOLOR);
								desc.dss = wi::renderer::GetDepthStencilState(wi::enums::DSSTYPE_DEFAULT);
								desc.rs = wi::renderer::GetRasterizerState(wi::enums::RSTYPE_DOUBLESIDED);
								desc.bs = wi::renderer::GetBlendState(wi::enums::BSTYPE_TRANSPARENT);
								desc.pt = PrimitiveTopology::TRIANGLELIST;
								wi::graphics::GetDevice()->CreatePipelineState(&desc, &pso);
							};
							static wi::eventhandler::Handle handle = wi::eventhandler::Subscribe(wi::eventhandler::EVENT_RELOAD_SHADERS, [](uint64_t userdata) { LoadShaders(); });
							LoadShaders();
						}
						struct Vertex
						{
							XMFLOAT4 position;
							XMFLOAT4 color;
						};

						device->BindPipelineState(&pso, cmd);

						const uint32_t segmentCount = 6;
						const uint32_t cylinder_triangleCount = segmentCount * 2;
						const uint32_t cone_triangleCount = segmentCount;
						const uint32_t vertexCount = (cylinder_triangleCount + cone_triangleCount) * 3;
						auto mem = device->AllocateGPU(sizeof(Vertex) * vertexCount, cmd);

						float siz = 0.02f * dist;
						const XMMATRIX M =
							XMMatrixScaling(siz, siz, siz) *
							XMMatrixRotationZ(-XM_PIDIV2) *
							XMMatrixRotationQuaternion(XMLoadFloat4(&light.rotation)) *
							XMMatrixTranslation(light.position.x, light.position.y, light.position.z)
							;

						const XMFLOAT4 col = fp.color;
						const XMFLOAT4 col_fade = XMFLOAT4(col.x, col.y, col.z, 0);
						const float origin_size = 0.2f;
						const float cone_length = 0.75f;
						const float axis_length = 18;
						float cylinder_length = axis_length;
						cylinder_length -= cone_length;
						uint8_t* dst = (uint8_t*)mem.data;
						for (uint32_t i = 0; i < segmentCount; ++i)
						{
							const float angle0 = (float)i / (float)segmentCount * XM_2PI;
							const float angle1 = (float)(i + 1) / (float)segmentCount * XM_2PI;
							// cylinder base:
							{
								const float cylinder_radius = 0.075f;
								Vertex verts[] = {
									{XMFLOAT4(0, std::sin(angle0) * cylinder_radius, std::cos(angle0) * cylinder_radius, 1), col_fade},
									{XMFLOAT4(0, std::sin(angle1) * cylinder_radius, std::cos(angle1) * cylinder_radius, 1), col_fade},
									{XMFLOAT4(cylinder_length, std::sin(angle0) * cylinder_radius, std::cos(angle0) * cylinder_radius, 1), col},
									{XMFLOAT4(cylinder_length, std::sin(angle0) * cylinder_radius, std::cos(angle0) * cylinder_radius, 1), col},
									{XMFLOAT4(cylinder_length, std::sin(angle1) * cylinder_radius, std::cos(angle1) * cylinder_radius, 1), col},
									{XMFLOAT4(0, std::sin(angle1) * cylinder_radius, std::cos(angle1) * cylinder_radius, 1), col_fade},
								};
								for (auto& vert : verts)
								{
									XMStoreFloat4(&vert.position, XMVector3Transform(XMLoadFloat4(&vert.position), M));
								}
								std::memcpy(dst, verts, sizeof(verts));
								dst += sizeof(verts);
							}
							// cone cap:
							{
								const float cone_radius = origin_size;
								Vertex verts[] = {
									{XMFLOAT4(axis_length, 0, 0, 1), col},
									{XMFLOAT4(cylinder_length, std::sin(angle0) * cone_radius, std::cos(angle0) * cone_radius, 1), col},
									{XMFLOAT4(cylinder_length, std::sin(angle1) * cone_radius, std::cos(angle1) * cone_radius, 1), col},
								};
								for (auto& vert : verts)
								{
									XMStoreFloat4(&vert.position, XMVector3Transform(XMLoadFloat4(&vert.position), M));
								}
								std::memcpy(dst, verts, sizeof(verts));
								dst += sizeof(verts);
							}
						}
						const GPUBuffer* vbs[] = {
							&mem.buffer,
						};
						const uint32_t strides[] = {
							sizeof(Vertex),
						};
						const uint64_t offsets[] = {
							mem.offset,
						};
						device->BindVertexBuffers(vbs, 0, arraysize(vbs), strides, offsets, cmd);
						device->Draw(vertexCount, 0, cmd);
						device->EventEnd(cmd);
					}
				}
			}

			if (has_flag(optionsWnd.filter, OptionsWindow::Filter::Decal))
			{
				for (size_t i = 0; i < scene.decals.GetCount(); ++i)
				{
					Entity entity = scene.decals.GetEntity(i);
					if (!scene.transforms.Contains(entity))
						continue;
					const TransformComponent& transform = *scene.transforms.GetComponent(entity);

					fp.position = transform.GetPosition();
					fp.scaling = scaling * wi::math::Distance(transform.GetPosition(), camera.Eye);
					fp.color = inactiveEntityColor;

					if (hovered.entity == entity)
					{
						fp.color = hoveredEntityColor;
					}
					for (auto& picked : translator.selected)
					{
						if (picked.entity == entity)
						{
							fp.color = selectedEntityColor;
							break;
						}
					}


					wi::font::Draw(ICON_DECAL, fp, cmd);

				}
			}

			if (has_flag(optionsWnd.filter, OptionsWindow::Filter::Force))
			{
				for (size_t i = 0; i < scene.forces.GetCount(); ++i)
				{
					Entity entity = scene.forces.GetEntity(i);
					if (!scene.transforms.Contains(entity))
						continue;
					const TransformComponent& transform = *scene.transforms.GetComponent(entity);

					fp.position = transform.GetPosition();
					fp.scaling = scaling * wi::math::Distance(transform.GetPosition(), camera.Eye);
					fp.color = inactiveEntityColor;

					if (hovered.entity == entity)
					{
						fp.color = hoveredEntityColor;
					}
					for (auto& picked : translator.selected)
					{
						if (picked.entity == entity)
						{
							fp.color = selectedEntityColor;
							break;
						}
					}


					wi::font::Draw(ICON_FORCE, fp, cmd);
				}
			}

			if (has_flag(optionsWnd.filter, OptionsWindow::Filter::Camera))
			{
				for (size_t i = 0; i < scene.cameras.GetCount(); ++i)
				{
					Entity entity = scene.cameras.GetEntity(i);
					if (!scene.transforms.Contains(entity))
						continue;
					const TransformComponent& transform = *scene.transforms.GetComponent(entity);

					fp.position = transform.GetPosition();
					fp.scaling = scaling * wi::math::Distance(transform.GetPosition(), camera.Eye);
					fp.color = inactiveEntityColor;

					if (hovered.entity == entity)
					{
						fp.color = hoveredEntityColor;
					}
					for (auto& picked : translator.selected)
					{
						if (picked.entity == entity)
						{
							fp.color = selectedEntityColor;
							break;
						}
					}


					wi::font::Draw(ICON_CAMERA, fp, cmd);
				}
			}

			if (has_flag(optionsWnd.filter, OptionsWindow::Filter::Armature))
			{
				for (size_t i = 0; i < scene.armatures.GetCount(); ++i)
				{
					Entity entity = scene.armatures.GetEntity(i);
					if (!scene.transforms.Contains(entity))
						continue;
					const TransformComponent& transform = *scene.transforms.GetComponent(entity);

					fp.position = transform.GetPosition();
					fp.scaling = scaling * wi::math::Distance(transform.GetPosition(), camera.Eye);
					fp.color = inactiveEntityColor;

					if (hovered.entity == entity)
					{
						fp.color = hoveredEntityColor;
					}
					for (auto& picked : translator.selected)
					{
						if (picked.entity == entity)
						{
							fp.color = selectedEntityColor;
							break;
						}
					}


					wi::font::Draw(ICON_ARMATURE, fp, cmd);
				}
			}

			if (has_flag(optionsWnd.filter, OptionsWindow::Filter::Emitter))
			{
				for (size_t i = 0; i < scene.emitters.GetCount(); ++i)
				{
					Entity entity = scene.emitters.GetEntity(i);
					if (!scene.transforms.Contains(entity))
						continue;
					const TransformComponent& transform = *scene.transforms.GetComponent(entity);

					fp.position = transform.GetPosition();
					fp.scaling = scaling * wi::math::Distance(transform.GetPosition(), camera.Eye);
					fp.color = inactiveEntityColor;

					if (hovered.entity == entity)
					{
						fp.color = hoveredEntityColor;
					}
					for (auto& picked : translator.selected)
					{
						if (picked.entity == entity)
						{
							fp.color = selectedEntityColor;
							break;
						}
					}


					wi::font::Draw(ICON_EMITTER, fp, cmd);
				}
			}

			if (has_flag(optionsWnd.filter, OptionsWindow::Filter::Hairparticle))
			{
				for (size_t i = 0; i < scene.hairs.GetCount(); ++i)
				{
					Entity entity = scene.hairs.GetEntity(i);
					if (!scene.transforms.Contains(entity))
						continue;
					const TransformComponent& transform = *scene.transforms.GetComponent(entity);

					fp.position = transform.GetPosition();
					fp.scaling = scaling * wi::math::Distance(transform.GetPosition(), camera.Eye);
					fp.color = inactiveEntityColor;

					if (hovered.entity == entity)
					{
						fp.color = hoveredEntityColor;
					}
					for (auto& picked : translator.selected)
					{
						if (picked.entity == entity)
						{
							fp.color = selectedEntityColor;
							break;
						}
					}


					wi::font::Draw(ICON_HAIR, fp, cmd);
				}
			}

			if (has_flag(optionsWnd.filter, OptionsWindow::Filter::Sound))
			{
				for (size_t i = 0; i < scene.sounds.GetCount(); ++i)
				{
					Entity entity = scene.sounds.GetEntity(i);
					if (!scene.transforms.Contains(entity))
						continue;
					const TransformComponent& transform = *scene.transforms.GetComponent(entity);

					fp.position = transform.GetPosition();
					fp.scaling = scaling * wi::math::Distance(transform.GetPosition(), camera.Eye);
					fp.color = inactiveEntityColor;

					if (hovered.entity == entity)
					{
						fp.color = hoveredEntityColor;
					}
					for (auto& picked : translator.selected)
					{
						if (picked.entity == entity)
						{
							fp.color = selectedEntityColor;
							break;
						}
					}


					wi::font::Draw(ICON_SOUND, fp, cmd);
				}
			}
			if (has_flag(optionsWnd.filter, OptionsWindow::Filter::Video))
			{
				for (size_t i = 0; i < scene.videos.GetCount(); ++i)
				{
					Entity entity = scene.videos.GetEntity(i);
					if (!scene.transforms.Contains(entity))
						continue;
					const TransformComponent& transform = *scene.transforms.GetComponent(entity);

					fp.position = transform.GetPosition();
					fp.scaling = scaling * wi::math::Distance(transform.GetPosition(), camera.Eye);
					fp.color = inactiveEntityColor;

					if (hovered.entity == entity)
					{
						fp.color = hoveredEntityColor;
					}
					for (auto& picked : translator.selected)
					{
						if (picked.entity == entity)
						{
							fp.color = selectedEntityColor;
							break;
						}
					}


					wi::font::Draw(ICON_VIDEO, fp, cmd);
				}
			}

			size_t spline_points_count = 0;
			size_t spline_shape_count = 0;
			for (size_t i = 0; i < scene.splines.GetCount(); ++i)
			{
				const SplineComponent& spline = scene.splines[i];
				spline_points_count += spline.path.size();
			}

			for (size_t i = 0; i < scene.shapes.GetCount(); ++i)
			{
				const ShapeComponent& shape = scene.shapes[i];
				spline_shape_count += shape.mesh2dvtex.size();
			}



			if (spline_points_count > 0)
			{
				wi::renderer::RenderablePoint point;
				wi::renderer::RenderablePoint point2;
				wi::renderer::RenderableLine line;

				for (size_t i = 0; i < scene.splines.GetCount(); ++i)
				{
					const SplineComponent& spline = scene.splines[i];

					for (auto i = spline.path.begin(); i != spline.path.end(); i++)
					{
						point.position = i->second;

						point.size = 0.5f;

						point.color = XMFLOAT4(1,1, 1, 1);

						wi::renderer::DrawPoint(point);

					}


				}

				if (spline_points_count >= 4)
				{
					for (size_t i = 0; i < scene.splines.GetCount(); ++i)
					{
						SplineComponent& spline = scene.splines[i];

						wi::vector<XMFLOAT3> path;

						for (auto i = spline.path.begin(); i != spline.path.end(); i++)
						{
							path.push_back(i->second);
						}

						for (float i = 0; i < spline.path.size() - 3.0f; i+=0.01)
						{
							//XMFLOAT3 start = GetSplinePoint();
							//XMFLOAT3 end = GetSplinePoint();



							//point.position = spline.GetSplinePointCat(path, i);

							//point.size = 0.01f;

							//point.color = XMFLOAT4(1, 1, 1, 1);

							XMFLOAT3 a = spline.GetSplinePointCat(path, i);
							XMFLOAT3 b = spline.GetSplinePointCat(path, std::clamp(i + 0.01f,0.0f,spline.path.size() - 3.0f));

							line.color_start = XMFLOAT4(1, 1, 1, 1);
							line.color_end = XMFLOAT4(0, 0, 1, 1);
							line.start = a;
							line.end = b;
							wi::renderer::DrawLine(line);

							/*line.start = i->second;

							line.end;

							line.color_start = XMFLOAT4(1, 1, 1, 1);
							line.color_end = XMFLOAT4(1, 1, 1, 1);*/

							point2.position = spline.GetSplinePointCat(path, spline.T);
							point2.size = 0.75f;
							point2.color = XMFLOAT4(1, 0, 0, 1);

							//wi::renderer::DrawPoint(point);
							wi::renderer::DrawPoint(point2);

							if (spline.nexttarget != INVALID_ENTITY)
							{

								TransformComponent* entTransform = scene.transforms.GetComponent(spline.nexttarget);


								if (entTransform == nullptr)
									return;
								entTransform->SetDirty();
								entTransform->translation_local = spline.GetSplinePointCat(path, spline.T);
								XMFLOAT4 rot;

								XMStoreFloat4(&rot, spline.GetOrintation(path, spline.T));
								entTransform->rotation_local = rot;


								//entTransform->Rotate(spline.GetOrintation(path, spline.T));
							}

						}


					}
				}
				else
				{
					for (size_t i = 0; i < scene.splines.GetCount(); ++i)
					{
						SplineComponent& spline = scene.splines[i];

						wi::vector<XMFLOAT3> path;

						for (auto i = spline.path.begin(); i != spline.path.end(); i++)
						{
							path.push_back(i->second);
						}

						for (float i = 0; i < spline.path.size(); i += 0.01)
						{
							//XMFLOAT3 start = GetSplinePoint();
							//XMFLOAT3 end = GetSplinePoint();

							if (spline.path.size() > 1)
							{




								point.position = spline.GetSplinePointLinear(path, i);

								point.size = 0.01f;

								point.color = XMFLOAT4(1, 1, 1, 1);

								/*line.start = i->second;

								line.end;

								line.color_start = XMFLOAT4(1, 1, 1, 1);
								line.color_end = XMFLOAT4(1, 1, 1, 1);*/

								wi::renderer::DrawPoint(point);





							}




						}


					}
				}




			}

			if (spline_shape_count > 0)
			{

				for (size_t i = 0; i < scene.splines.GetCount(); ++i)
				{
					ShapeComponent& shape = scene.shapes[i];

					wi::renderer::RenderableLine line;

					for (size_t i = 0; i < shape.lineIndices.size(); i += 2)
					{

						/*if (!scene.transforms.Contains(entity))
							continue;
						const TransformComponent& transform = *scene.transforms.GetComponent(entity);
						XMVECTOR a = transform.GetPositionV();
						XMVECTOR b = a + XMVectorSet(0, 0.1f, 0, 0);

						XMFLOAT3 TT;

						XMStoreFloat3(&TT, T);
						*/

						XMFLOAT3 a = XMFLOAT3(shape.mesh2dvtex[shape.lineIndices[i]].point.x /* + TT.x*/, shape.mesh2dvtex[shape.lineIndices[i]].point.y /*+  TT.y*/, 0);
						XMFLOAT3 b = XMFLOAT3(shape.mesh2dvtex[shape.lineIndices[i + 1]].point.x /* + TT.x */ , shape.mesh2dvtex[shape.lineIndices[i + 1]].point.y /* + TT.y*/, 0);
						line.start = a;
						line.end = b;
						wi::renderer::DrawLine(line);
					}
				}
			}

			if (bone_picking)
			{
				static PipelineState pso;
				if (!pso.IsValid())
				{
					static auto LoadShaders = [] {
						PipelineStateDesc desc;
						desc.vs = wi::renderer::GetShader(wi::enums::VSTYPE_VERTEXCOLOR);
						desc.ps = wi::renderer::GetShader(wi::enums::PSTYPE_VERTEXCOLOR);
						desc.il = wi::renderer::GetInputLayout(wi::enums::ILTYPE_VERTEXCOLOR);
						desc.dss = wi::renderer::GetDepthStencilState(wi::enums::DSSTYPE_DEPTHDISABLED);
						desc.rs = wi::renderer::GetRasterizerState(wi::enums::RSTYPE_DOUBLESIDED);
						desc.bs = wi::renderer::GetBlendState(wi::enums::BSTYPE_TRANSPARENT);
						desc.pt = PrimitiveTopology::TRIANGLELIST;
						wi::graphics::GetDevice()->CreatePipelineState(&desc, &pso);
					};
					static wi::eventhandler::Handle handle = wi::eventhandler::Subscribe(wi::eventhandler::EVENT_RELOAD_SHADERS, [](uint64_t userdata) { LoadShaders(); });
					LoadShaders();
				}



				size_t bone_count = 0;
				for (size_t i = 0; i < scene.armatures.GetCount(); ++i)
				{
					const ArmatureComponent& armature = scene.armatures[i];
					bone_count += armature.boneCollection.size();
				}

				if (bone_count > 0)
				{
					struct Vertex
					{
						XMFLOAT4 position;
						XMFLOAT4 color;
					};
					const size_t segment_count = 18 + 1 + 18 + 1;
					const size_t vb_size = sizeof(Vertex) * (bone_count * (segment_count + 1 + 1));
					const size_t ib_size = sizeof(uint32_t) * bone_count * (segment_count + 1) * 3;
					GraphicsDevice::GPUAllocation mem = device->AllocateGPU(vb_size + ib_size, cmd);
					Vertex* vertices = (Vertex*)mem.data;
					uint32_t* indices = (uint32_t*)((uint8_t*)mem.data + vb_size);
					uint32_t vertex_count = 0;
					uint32_t index_count = 0;

					const XMVECTOR Eye = camera.GetEye();
					const XMVECTOR Unit = XMVectorSet(0, 1, 0, 0);

					for (size_t i = 0; i < scene.armatures.GetCount(); ++i)
					{
						const ArmatureComponent& armature = scene.armatures[i];
						for (Entity entity : armature.boneCollection)
						{
							if (!scene.transforms.Contains(entity))
								continue;
							const TransformComponent& transform = *scene.transforms.GetComponent(entity);
							XMVECTOR a = transform.GetPositionV();
							XMVECTOR b = a + XMVectorSet(0, 0.1f, 0, 0);
							// Search for child to connect bone tip:
							bool child_found = false;
							for (size_t h = 0; (h < scene.humanoids.GetCount()) && !child_found; ++h)
							{
								const HumanoidComponent& humanoid = scene.humanoids[h];
								int bodypart = 0;
								for (Entity child : humanoid.bones)
								{
									const HierarchyComponent* hierarchy = scene.hierarchy.GetComponent(child);
									if (hierarchy != nullptr && hierarchy->parentID == entity && scene.transforms.Contains(child))
									{
										if (bodypart == int(HumanoidComponent::HumanoidBone::Hips))
										{
											// skip root-hip connection
											child_found = true;
											break;
										}
										const TransformComponent& child_transform = *scene.transforms.GetComponent(child);
										b = child_transform.GetPositionV();
										child_found = true;
										break;
									}
									bodypart++;
								}
							}
							if (!child_found)
							{
								for (Entity child : armature.boneCollection)
								{
									const HierarchyComponent* hierarchy = scene.hierarchy.GetComponent(child);
									if (hierarchy != nullptr && hierarchy->parentID == entity && scene.transforms.Contains(child))
									{
										const TransformComponent& child_transform = *scene.transforms.GetComponent(child);
										b = child_transform.GetPositionV();
										child_found = true;
										break;
									}
								}
							}
							if (!child_found)
							{
								// No child, try to guess bone tip compared to parent (if it has parent):
								const HierarchyComponent* hierarchy = scene.hierarchy.GetComponent(entity);
								if (hierarchy != nullptr && scene.transforms.Contains(hierarchy->parentID))
								{
									const TransformComponent& parent_transform = *scene.transforms.GetComponent(hierarchy->parentID);
									XMVECTOR ab = a - parent_transform.GetPositionV();
									b = a + ab;
								}
							}
							XMVECTOR ab = XMVector3Normalize(b - a);

							wi::primitive::Capsule capsule;
							capsule.radius = wi::math::Distance(a, b) * 0.1f;
							a -= ab * capsule.radius;
							b += ab * capsule.radius;
							XMStoreFloat3(&capsule.base, a);
							XMStoreFloat3(&capsule.tip, b);
							XMFLOAT4 color = inactiveEntityColor;

							if (scene.springs.Contains(entity))
							{
								color = wi::Color(255, 70, 165, uint8_t(color.w * 255));
							}
							if (scene.inverse_kinematics.Contains(entity))
							{
								color = wi::Color(49, 190, 103, uint8_t(color.w * 255));
							}

							if (hovered.entity == entity)
							{
								color = hoveredEntityColor;
							}
							for (auto& picked : translator.selected)
							{
								if (picked.entity == entity)
								{
									color = selectedEntityColor;
									break;
								}
							}

							color.w *= optionsWnd.generalWnd.bonePickerOpacitySlider.GetValue();

							XMVECTOR Base = XMLoadFloat3(&capsule.base);
							XMVECTOR Tip = XMLoadFloat3(&capsule.tip);
							XMVECTOR Radius = XMVectorReplicate(capsule.radius);
							XMVECTOR Normal = XMVector3Normalize(Tip - Base);
							XMVECTOR Tangent = XMVector3Normalize(XMVector3Cross(Normal, Base - Eye));
							XMVECTOR Binormal = XMVector3Normalize(XMVector3Cross(Tangent, Normal));
							XMVECTOR LineEndOffset = Normal * Radius;
							XMVECTOR A = Base + LineEndOffset;
							XMVECTOR B = Tip - LineEndOffset;
							XMVECTOR AB = Unit * XMVector3Length(B - A);
							XMMATRIX M = { Tangent,Normal,Binormal,XMVectorSetW(A, 1) };

							uint32_t center_vertex_index = vertex_count;
							Vertex center_vertex;
							XMStoreFloat4(&center_vertex.position, A);
							center_vertex.position.w = 1;
							center_vertex.color = color;
							center_vertex.color.w = 0;
							std::memcpy(vertices + vertex_count, &center_vertex, sizeof(center_vertex));
							vertex_count++;

							for (size_t i = 0; i < segment_count; ++i)
							{
								XMVECTOR segment_pos;
								const float angle0 = XM_PIDIV2 + (float)i / (float)segment_count * XM_2PI;
								if (i < 18)
								{
									segment_pos = XMVectorSet(sinf(angle0) * capsule.radius, cosf(angle0) * capsule.radius, 0, 1);
								}
								else if (i == 18)
								{
									segment_pos = XMVectorSet(sinf(angle0) * capsule.radius, cosf(angle0) * capsule.radius, 0, 1);
								}
								else if (i > 18 && i < 18 + 1 + 18)
								{
									segment_pos = AB + XMVectorSet(sinf(angle0) * capsule.radius * 0.5f, cosf(angle0) * capsule.radius * 0.5f, 0, 1);
								}
								else
								{
									segment_pos = AB + XMVectorSet(sinf(angle0) * capsule.radius * 0.5f, cosf(angle0) * capsule.radius * 0.5f, 0, 1);
								}
								segment_pos = XMVector3Transform(segment_pos, M);

								Vertex vertex;
								XMStoreFloat4(&vertex.position, segment_pos);
								vertex.position.w = 1;
								vertex.color = color;
								//vertex.color.w = 0;
								std::memcpy(vertices + vertex_count, &vertex, sizeof(vertex));
								uint32_t ind[] = { center_vertex_index,vertex_count - 1,vertex_count };
								std::memcpy(indices + index_count, ind, sizeof(ind));
								index_count += arraysize(ind);
								vertex_count++;
							}
							// closing triangle fan:
							uint32_t ind[] = { center_vertex_index,vertex_count - 1,center_vertex_index+1 };
							std::memcpy(indices + index_count, ind, sizeof(ind));
							index_count += arraysize(ind);
						}
					}

					device->EventBegin("Bone capsules", cmd);
					device->BindPipelineState(&pso, cmd);

					const GPUBuffer* vbs[] = {
						&mem.buffer,
					};
					const uint32_t strides[] = {
						sizeof(Vertex)
					};
					const uint64_t offsets[] = {
						mem.offset,
					};
					device->BindVertexBuffers(vbs, 0, arraysize(vbs), strides, offsets, cmd);
					device->BindIndexBuffer(&mem.buffer, IndexBufferFormat::UINT32, mem.offset + vb_size, cmd);

					device->DrawIndexed(index_count, 0, 0, cmd);
					device->EventEnd(cmd);
				}
			}

			if (optionsWnd.generalWnd.nameDebugCheckBox.GetCheck())
			{
				device->EventBegin("Debug Names", cmd);
				struct DebugNameEntitySorter
				{
					size_t name_index;
					float distance;
					XMFLOAT3 position;
				};
				static wi::vector<DebugNameEntitySorter> debugNameEntitiesSorted;
				debugNameEntitiesSorted.clear();
				for (size_t i = 0; i < scene.names.GetCount(); ++i)
				{
					Entity entity = scene.names.GetEntity(i);
					const TransformComponent* transform = scene.transforms.GetComponent(entity);
					if (transform != nullptr)
					{
						auto& x = debugNameEntitiesSorted.emplace_back();
						x.name_index = i;
						x.position = transform->GetPosition();
						const ObjectComponent* object = scene.objects.GetComponent(entity);
						if (object != nullptr)
						{
							x.position = object->center;
						}
						x.distance = wi::math::Distance(x.position, camera.Eye);
					}
				}
				std::sort(debugNameEntitiesSorted.begin(), debugNameEntitiesSorted.end(), [](const DebugNameEntitySorter& a, const DebugNameEntitySorter& b)
					{
						return a.distance > b.distance;
					});
				for (auto& x : debugNameEntitiesSorted)
				{
					Entity entity = scene.names.GetEntity(x.name_index);
					wi::font::Params params;
					params.position = x.position;
					params.size = wi::font::WIFONTSIZE_DEFAULT;
					params.scaling = 1.0f / params.size * x.distance * 0.03f;
					params.color = wi::Color::White();
					for (auto& picked : translator.selected)
					{
						if (picked.entity == entity)
						{
							params.color = selectedEntityColor;
							break;
						}
					}
					params.h_align = wi::font::WIFALIGN_CENTER;
					params.v_align = wi::font::WIFALIGN_CENTER;
					params.softness = 0.1f;
					params.shadowColor = wi::Color::Black();
					params.shadow_softness = 0.5f;
					params.customProjection = &VP;
					params.customRotation = &R;
					wi::font::Draw(scene.names[x.name_index].name, params, cmd);
				}
				device->EventEnd(cmd);
			}

			if (inspector_mode)
			{
				std::string str;
				str += "Entity: " + std::to_string(hovered.entity);
				const NameComponent* name = scene.names.GetComponent(hovered.entity);
				if (name != nullptr)
				{
					str += "\nName: " + name->name;
				}



				const MaterialComponent* material = scene.materials.GetComponent(hovered.entity);


				if (material != nullptr)
				{
					auto type = material->shaderTypeDefines[material->shaderType];
					std::string finalname;
					if (material->shaderType == 0)
					{
						finalname = "PBR";
					}
					else
					{
						for (size_t i = 0; i < type.size(); i++)
						{
							finalname += " " + type[i];
						}

					}

					str += "\nShader: " + finalname;
				}

				if (material != nullptr)
				{
					auto type = material->surfaceTypeDefines[material->surfacetype];

					str += "\nSurface type: " + type[0];
				}

				const ResponseComponent* response = scene.responses.GetComponent(hovered.entity);

				if (response != nullptr)
				{
					auto type = response->_conditions;
					std::string finalname;

					finalname = "CONDS: ";

					if ((type & response->isBurn).any())
					{
						auto type = response->CondsDefines[ResponseComponent::COND_BURN];
						finalname += " " + type[0];
					}
					if ((type & response->isSee_Hate).any())
					{
						auto type = response->CondsDefines[ResponseComponent::COND_SEE_HATE];
						finalname += "," + type[0];
					}
					/*for (size_t i = 0; i < type.size(); i++)
					{
						finalname += " " + std::to_string(type.test(i));

					}*/

					str += "\n" + finalname;
				}


				XMFLOAT4 pointer = wi::input::GetPointer();
				wi::font::Params params;
				params.position = XMFLOAT3(pointer.x - 10, pointer.y, 0);
				params.shadowColor = wi::Color::Black();
				params.h_align = wi::font::WIFALIGN_RIGHT;
				params.v_align = wi::font::WIFALIGN_CENTER;
				wi::font::Draw(str, params, cmd);
			}


			optionsWnd.paintToolWnd.DrawBrush(*this, cmd);
			if (optionsWnd.paintToolWnd.GetMode() == PaintToolWindow::MODE::MODE_DISABLED)
			{
				translator.Draw(GetCurrentEditorScene().camera, cmd);
			}

			device->RenderPassEnd(cmd);
		}

		device->EventEnd(cmd);
	}

	RenderPath2D::Render();

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
