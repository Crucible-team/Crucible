#include "stdafx.h"
#include "exec.h"
#include "wiRenderer.h"
#include <SDL2/SDL.h>
#include "Assets/Icon.c"

using namespace wi::graphics;

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
	//wi::profiler::range_id profrange = wi::profiler::BeginRangeCPU("Editor Update");

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

	// Pulsating selection color update:
	outlineTimer += dt;;

	//save_text_alpha = std::max(0.0f, save_text_alpha - std::min(dt, 0.033f)); // after saving, dt can become huge

	bool clear_selected = false;

	main->infoDisplay.colorgrading_helper = false;


	//wi::profiler::EndRange(profrange);

	RenderPath2D::Update(dt);


	renderPath->colorspace = colorspace;
	renderPath->Update(dt);

}

void GameComponent::PostUpdate()
{
	RenderPath2D::PostUpdate();

	renderPath->PostUpdate();
}

void GameComponent::Render() const
{
	const Scene& scene = GetCurrentScene();

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
