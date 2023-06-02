#include "stdafx.h"
#include "IOWindow.h"
#include "Editor.h"

using namespace wi::ecs;
using namespace wi::scene;


void IOWindow::Create(EditorComponent* _editor)
{
	editor = _editor;
	wi::gui::Window::Create(ICON_IO " IO", wi::gui::Window::WindowControls::COLLAPSE | wi::gui::Window::WindowControls::CLOSE);
	SetSize(XMFLOAT2(360, 60));

	closeButton.SetTooltip("Delete IOComponent");
	OnClose([=](wi::gui::EventArgs args) {

		wi::Archive& archive = editor->AdvanceHistory();
		archive << EditorComponent::HISTORYOP_COMPONENT_DATA;
		editor->RecordEntity(archive, entity);

		editor->GetCurrentScene().IOs.Remove(entity);

		editor->RecordEntity(archive, entity);

		editor->optionsWnd.RefreshEntityTree();
		});

	float x = 60;
	float y = 0;
	float step = 25;
	float siz = 250;
	float hei = 20;

	nameInput.Create("");
	nameInput.SetDescription("Input: ");
	nameInput.SetPos(XMFLOAT2(x, y));
	nameInput.SetSize(XMFLOAT2(siz, hei));
	nameInput.OnInputAccepted([=](wi::gui::EventArgs args) {
		IOComponent* name = editor->GetCurrentScene().IOs.GetComponent(entity);
		if (name == nullptr)
		{
			name = &editor->GetCurrentScene().IOs.Create(entity);
		}
		//name->InputName = args.sValue;

		editor->optionsWnd.RefreshEntityTree();
		});
	AddWidget(&nameInput);

	SetMinimized(true);
	SetVisible(false);

	SetEntity(INVALID_ENTITY);
}

void IOWindow::SetEntity(Entity entity)
{
	this->entity = entity;

	if (entity != INVALID_ENTITY)
	{
		SetEnabled(true);

		IOComponent* name = editor->GetCurrentScene().IOs.GetComponent(entity);
		if (name != nullptr)
		{
			//nameInput.SetValue(name->InputName);
		}
	}
	else
	{
		SetEnabled(false);
		nameInput.SetValue("Select entity to modify inputname...");
	}
}

void IOWindow::ResizeLayout()
{
	wi::gui::Window::ResizeLayout();

	nameInput.SetPos(XMFLOAT2(60, 4));
	nameInput.SetSize(XMFLOAT2(GetSize().x - 65, nameInput.GetSize().y));
}
