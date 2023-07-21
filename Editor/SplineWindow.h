#pragma once
class EditorComponent;

class SplineWindow : public wi::gui::Window
{
public:
	void Create(EditorComponent* editor);

	EditorComponent* editor = nullptr;
	wi::ecs::Entity entity;

	wi::gui::Button AddPoint;

	void SetEntity(wi::ecs::Entity entity);

	void ResizeLayout() override;
};

