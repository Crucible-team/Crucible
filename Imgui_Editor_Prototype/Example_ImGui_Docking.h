#pragma once
#include "WickedEngine.h"


class Example_ImGuiRenderer : public wi::RenderPath2D
{
	wi::gui::Label label;
public:
	void Load() override;
	void Update(float dt) override;
	void ResizeLayout() override;
	void Render() const override;
	void DisplayPerformanceData(bool* p_open);
	void igTextTitle(const char* text);

	wi::RenderPath3D viewport1;

	wi::RenderPath3D viewport2;

	wi::RenderPath3D viewport3;

	wi::RenderPath3D viewport4;

	wi::scene::CameraComponent viewport2Cam;

	wi::scene::CameraComponent viewport3Cam;

	wi::scene::CameraComponent viewport4Cam;

	void PushToEntityTree(wi::ecs::Entity entity);
};

class Example_ImGui : public wi::Application
{
	Example_ImGuiRenderer renderer;
	

public:
	~Example_ImGui() override;
	void Initialize() override;
	void Compose(wi::graphics::CommandList cmd) override;
};

