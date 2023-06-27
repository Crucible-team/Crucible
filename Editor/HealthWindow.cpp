#include "stdafx.h"
#include "HealthWindow.h"

using namespace wi::ecs;
using namespace wi::scene;

void HealthWindow::Create(EditorComponent* _editor)
{
	editor = _editor;
	wi::gui::Window::Create(ICON_FA_HEART " Health", wi::gui::Window::WindowControls::COLLAPSE | wi::gui::Window::WindowControls::CLOSE);
	SetSize(XMFLOAT2(360, 120));

	closeButton.SetTooltip("Delete NameComponent");
	OnClose([=](wi::gui::EventArgs args) {

		wi::Archive& archive = editor->AdvanceHistory();
		archive << EditorComponent::HISTORYOP_COMPONENT_DATA;
		editor->RecordEntity(archive, entity);

		editor->GetCurrentScene().healths.Remove(entity);

		editor->RecordEntity(archive, entity);

		editor->optionsWnd.RefreshEntityTree();
		});

	float x = 60;
	float y = 4;
	float hei = 20;
	float step = hei + 2;

	float wid = 220;

	HealthSlider.Create(0, 100, 100, 1, "Health: ");
	HealthSlider.SetTooltip("Current amount of Health.");
	HealthSlider.SetSize(XMFLOAT2(wid, hei));
	HealthSlider.OnSlide([=](wi::gui::EventArgs args) {
		wi::scene::Scene& scene = editor->GetCurrentScene();
		HealthComponent* Health = scene.healths.GetComponent(entity);
		if (Health == nullptr)
			return;
		Health->SetHealth(args.iValue);
		});
	AddWidget(&HealthSlider);

	MaxHealthSlider.Create(0, 100, 100, 1, "MaxHealth: ");
	MaxHealthSlider.SetTooltip("Max Health this can be.");
	MaxHealthSlider.SetSize(XMFLOAT2(wid, hei));
	MaxHealthSlider.OnSlide([=](wi::gui::EventArgs args) {
		wi::scene::Scene& scene = editor->GetCurrentScene();
		HealthComponent* Health = scene.healths.GetComponent(entity);
		if (Health == nullptr)
			return;
		Health->SetMaxHealth(args.iValue);
		});
	AddWidget(&MaxHealthSlider);

	SetMinimized(true);
	SetVisible(false);

	SetEntity(INVALID_ENTITY);
}

void HealthWindow::SetEntity(Entity entity)
{
	this->entity = entity;

	if (entity != INVALID_ENTITY)
	{
		SetEnabled(true);

		HealthComponent* health = editor->GetCurrentScene().healths.GetComponent(entity);
		if (health != nullptr)
		{
			HealthSlider.SetValue(health->GetHealth());
			MaxHealthSlider.SetValue(health->GetMaxHealth());
		}
	}
	else
	{
		SetEnabled(false);
		HealthSlider.SetValue(0);
		MaxHealthSlider.SetValue(0);
	}
}

void HealthWindow::ResizeLayout()
{
	wi::gui::Window::ResizeLayout();
	const float padding = 4;
	const float width = GetWidgetAreaSize().x;
	float y = padding;
	float jump = 20;

	const float margin_left = 110;
	const float margin_right = 45;

	auto add = [&](wi::gui::Widget& widget) {
		if (!widget.IsVisible())
			return;
		widget.SetPos(XMFLOAT2(margin_left, y));
		widget.SetSize(XMFLOAT2(width - margin_left - margin_right, widget.GetScale().y));
		y += widget.GetSize().y;
		y += padding;
	};

	add(HealthSlider);
	add(MaxHealthSlider);
}
