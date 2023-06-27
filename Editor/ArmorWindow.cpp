#include "stdafx.h"
#include "ArmorWindow.h"

using namespace wi::ecs;
using namespace wi::scene;

void ArmorWindow::Create(EditorComponent* _editor)
{
	editor = _editor;
	wi::gui::Window::Create(ICON_FA_SHIELD " Armor", wi::gui::Window::WindowControls::COLLAPSE | wi::gui::Window::WindowControls::CLOSE);
	SetSize(XMFLOAT2(360, 120));

	closeButton.SetTooltip("Delete ArmorComponent");
	OnClose([=](wi::gui::EventArgs args) {

		wi::Archive& archive = editor->AdvanceHistory();
		archive << EditorComponent::HISTORYOP_COMPONENT_DATA;
		editor->RecordEntity(archive, entity);

		editor->GetCurrentScene().armors.Remove(entity);

		editor->RecordEntity(archive, entity);

		editor->optionsWnd.RefreshEntityTree();
		});

	float x = 60;
	float y = 4;
	float hei = 20;
	float step = hei + 2;
	float wid = 220;

	ArmorSlider.Create(0, 100, 100, 1, "Armor: ");
	ArmorSlider.SetTooltip("Current amount of Armor.");
	ArmorSlider.SetSize(XMFLOAT2(wid, hei));
	ArmorSlider.OnSlide([=](wi::gui::EventArgs args) {
		wi::scene::Scene& scene = editor->GetCurrentScene();
		ArmorComponent* armor = scene.armors.GetComponent(entity);
		if (armor == nullptr)
			return;
		armor->SetArmor(args.iValue);
		});
	AddWidget(&ArmorSlider);

	MaxArmorSlider.Create(0, 100, 100, 1, "MaxArmor: ");
	MaxArmorSlider.SetTooltip("Max Armor this can be.");
	MaxArmorSlider.SetSize(XMFLOAT2(wid, hei));
	MaxArmorSlider.OnSlide([=](wi::gui::EventArgs args) {
		wi::scene::Scene& scene = editor->GetCurrentScene();
		ArmorComponent* armor = scene.armors.GetComponent(entity);
		if (armor == nullptr)
			return;
		armor->SetMaxArmor(args.iValue);
		});
	AddWidget(&MaxArmorSlider);

	SetMinimized(true);
	SetVisible(false);

	SetEntity(INVALID_ENTITY);
}

void ArmorWindow::SetEntity(Entity entity)
{
	this->entity = entity;

	if (entity != INVALID_ENTITY)
	{
		SetEnabled(true);

		ArmorComponent* armor = editor->GetCurrentScene().armors.GetComponent(entity);
		if (armor != nullptr)
		{
			ArmorSlider.SetValue(armor->GetArmor());
			MaxArmorSlider.SetValue(armor->GetMaxArmor());
		}
	}
	else
	{
		SetEnabled(false);
		ArmorSlider.SetValue(0);
		MaxArmorSlider.SetValue(0);
	}
}

void ArmorWindow::ResizeLayout()
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

	add(ArmorSlider);
	add(MaxArmorSlider);
}
