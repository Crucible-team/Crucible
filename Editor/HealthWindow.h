#pragma once
class EditorComponent;

class HealthWindow : public wi::gui::Window
{
public:
	void Create(EditorComponent* editor);

	EditorComponent* editor = nullptr;
	wi::ecs::Entity entity;
	void SetEntity(wi::ecs::Entity entity);

	wi::gui::Slider HealthSlider;
	wi::gui::Slider MaxHealthSlider;

	void ResizeLayout() override;
};

