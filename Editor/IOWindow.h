#pragma once
#include "WickedEngine.h"

class EditorComponent;

class IOWindow : public wi::gui::Window
{
public:
	void Create(EditorComponent* editor);

	EditorComponent* editor = nullptr;
	wi::ecs::Entity entity;
	void SetEntity(wi::ecs::Entity entity);

	wi::gui::TextInputField nameInput;

	void ResizeLayout() override;
};

