#pragma once
class EditorComponent;

class SplineWindow : public wi::gui::Window
{
public:
	void Create(EditorComponent* editor);

	EditorComponent* editor = nullptr;
	wi::ecs::Entity entity;

	wi::gui::Button AddPoint;
	wi::gui::Button testENt;

	wi::gui::Slider TtestSlider;

	void SetEntity(wi::ecs::Entity entity);

	void ResizeLayout() override;
};

