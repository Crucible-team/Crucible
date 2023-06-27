#pragma once
class EditorComponent;

class ArmorWindow : public wi::gui::Window
{
public:
	void Create(EditorComponent* editor);

	EditorComponent* editor = nullptr;
	wi::ecs::Entity entity;
	void SetEntity(wi::ecs::Entity entity);

	wi::gui::Slider ArmorSlider;
	wi::gui::Slider MaxArmorSlider;

	void ResizeLayout() override;
};

