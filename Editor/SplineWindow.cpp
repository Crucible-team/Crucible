#include "stdafx.h"
#include "SplineWindow.h"

using namespace wi::ecs;
using namespace wi::scene;

void SplineWindow::Create(EditorComponent* _editor)
{
	editor = _editor;
	wi::gui::Window::Create(ICON_FA_SHIELD " Spline", wi::gui::Window::WindowControls::COLLAPSE | wi::gui::Window::WindowControls::CLOSE);
	SetSize(XMFLOAT2(360, 120));

	closeButton.SetTooltip("Delete SplineComponent");
	OnClose([=](wi::gui::EventArgs args) {

		wi::Archive& archive = editor->AdvanceHistory();
		archive << EditorComponent::HISTORYOP_COMPONENT_DATA;
		editor->RecordEntity(archive, entity);

		editor->GetCurrentScene().splines.Remove(entity);

		editor->RecordEntity(archive, entity);

		editor->optionsWnd.RefreshEntityTree();
		});

	float x = 60;
	float y = 4;
	float hei = 20;
	float step = hei + 2;
	float wid = 220;

	float siz = 140;

	AddPoint.Create("Add Point");
	AddPoint.SetTooltip("Add a new point.");
	AddPoint.SetSize(XMFLOAT2(wid, hei));
	AddPoint.OnClick([=](wi::gui::EventArgs args) {
		wi::scene::Scene& scene = editor->GetCurrentScene();
		SplineComponent* spline = scene.splines.GetComponent(entity);
		if (spline == nullptr)
			return;
		spline->path.emplace(std::to_string(spline->path.size() + 1 ), XMFLOAT3(wi::random::GetRandom(XM_PI), wi::random::GetRandom(XM_PI), wi::random::GetRandom(XM_PI)));
		});
	AddWidget(&AddPoint);

	testENt.Create("Test entity");
	testENt.SetTooltip("use a test entity");
	testENt.SetSize(XMFLOAT2(wid, hei));
	testENt.OnClick([=](wi::gui::EventArgs args) {
		wi::scene::Scene& scene = editor->GetCurrentScene();
		SplineComponent* spline = scene.splines.GetComponent(entity);
		if (spline == nullptr)
			return;
		spline->nexttarget = scene.Entity_CreateCylinder("spline test ent");
		});
	AddWidget(&testENt);

	TtestSlider.Create(0, 1, 0.1f, 100000, "T test: ");
	TtestSlider.SetTooltip("T for spline point.");
	TtestSlider.SetPos(XMFLOAT2(x, y += step));
	TtestSlider.SetSize(XMFLOAT2(siz, hei));
	TtestSlider.OnSlide([&](wi::gui::EventArgs args) {
		editor->GetCurrentScene().splines.GetComponent(entity)->T = args.fValue;
		});
	AddWidget(&TtestSlider);

	


	SetMinimized(true);
	SetVisible(false);

	SetEntity(INVALID_ENTITY);
}

void SplineWindow::SetEntity(Entity entity)
{
	this->entity = entity;

	if (entity != INVALID_ENTITY)
	{
		SetEnabled(true);

		SplineComponent* Spline = editor->GetCurrentScene().splines.GetComponent(entity);
		if (Spline != nullptr)
		{
		}
	}
	else
	{
		SetEnabled(false);
	}
}

void SplineWindow::ResizeLayout()
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

	add(AddPoint);
	add(testENt);
	add(TtestSlider);
}
