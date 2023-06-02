#include "stdafx.h"
#include "ResponseWindow.h"
#include "Editor.h"

void ResponseWindow::Create(EditorComponent* _editor)
{
	editor = _editor;
	wi::gui::Window::Create(ICON_FA_CHART_AREA " Response", wi::gui::Window::WindowControls::COLLAPSE | wi::gui::Window::WindowControls::CLOSE);
	SetSize(XMFLOAT2(520, 80));


	closeButton.SetTooltip("Delete Response");
	OnClose([=](wi::gui::EventArgs args) {

		wi::Archive& archive = editor->AdvanceHistory();
		archive << EditorComponent::HISTORYOP_COMPONENT_DATA;
		editor->RecordEntity(archive, entity);

		editor->GetCurrentScene().responses.Remove(entity);

		editor->RecordEntity(archive, entity);

		editor->optionsWnd.RefreshEntityTree();
		});

	float hei = 20;
	float wid = 100;

	

	fileButton.Create("Open File...");
	fileButton.SetDescription("File: ");
	fileButton.SetSize(XMFLOAT2(wid, hei));
	fileButton.OnClick([=](wi::gui::EventArgs args) {
		wi::scene::Scene& scene = editor->GetCurrentScene();
		wi::scene::ResponseComponent* response = scene.responses.GetComponent(entity);
		if (response == nullptr)
			return;

		if (response->resource.IsValid())
		{
			response->resource = {};
			response->filename = {};
			response->script = {};
			fileButton.SetText("Open File...");
		}
		else
		{
			wi::helper::FileDialogParams params;
			params.type = wi::helper::FileDialogParams::OPEN;
			params.description = "Response File (*.rps)";
			params.extensions = wi::resourcemanager::GetSupportedScriptExtensions();
			wi::helper::FileDialog(params, [=](std::string fileName) {
				wi::eventhandler::Subscribe_Once(wi::eventhandler::EVENT_THREAD_SAFE_POINT, [=](uint64_t userdata) {
					response->CreateFromFile(fileName);
					fileButton.SetText(wi::helper::GetFileNameFromPath(fileName));

					//response->_conditions |= (response->isBurn | response->isSee_Hate); // I am buring and I see hate!
					
					});
				});
		}
	});
	AddWidget(&fileButton);

	

	playonceCheckBox.Create("Once: ");
	playonceCheckBox.SetTooltip("Play the response only one time, and stop immediately.\nUseful for having custom update frequency logic in the response.");
	playonceCheckBox.SetSize(XMFLOAT2(hei, hei));
	playonceCheckBox.OnClick([=](wi::gui::EventArgs args) {
		wi::scene::Scene& scene = editor->GetCurrentScene();
		wi::scene::ResponseComponent* response = scene.responses.GetComponent(entity);
		if (response == nullptr)
			return;

		response->SetPlayOnce(args.bValue);
	});
	AddWidget(&playonceCheckBox);

	playstopButton.Create("");
	playstopButton.SetTooltip("Play / Stop response");
	playstopButton.SetSize(XMFLOAT2(wid, hei));
	playstopButton.OnClick([=](wi::gui::EventArgs args) {
		wi::scene::Scene& scene = editor->GetCurrentScene();
		wi::scene::ResponseComponent* response = scene.responses.GetComponent(entity);
		if (response == nullptr)
			return;

		if (response->IsPlaying())
		{
			response->Stop();
		}
		else
		{
			response->Play();
		}
	});
	AddWidget(&playstopButton);

	SetMinimized(true);
	SetVisible(false);
}

void ResponseWindow::SetEntity(wi::ecs::Entity entity)
{
	if (this->entity == entity)
		return;

	this->entity = entity;

	wi::scene::Scene& scene = editor->GetCurrentScene();
	wi::scene::ResponseComponent* response = scene.responses.GetComponent(entity);

	if (response != nullptr)
	{
		if (response->resource.IsValid())
		{
			fileButton.SetText(wi::helper::GetFileNameFromPath(response->filename));
		}
		else
		{
			fileButton.SetText("Open File...");
		}
		playonceCheckBox.SetCheck(response->IsPlayingOnlyOnce());
	}

	//for debug only
	/*if (response != nullptr)
	{
		response->CreateFromFile("G:/SteamLibrary/steamapps/common/Wicked Engine/Content/scripts/response/testresponse.rps");
	}*/
	

}

void ResponseWindow::Update(const wi::Canvas& canvas, float dt)
{
	wi::scene::Scene& scene = editor->GetCurrentScene();
	wi::scene::ResponseComponent* response = scene.responses.GetComponent(entity);
	if (response != nullptr)
	{
		if (response->IsPlaying())
		{
			playstopButton.SetText(ICON_STOP);
		}
		else
		{
			playstopButton.SetText(ICON_PLAY);
		}
	}

	wi::gui::Window::Update(canvas, dt);
}
void ResponseWindow::ResizeLayout()
{
	wi::gui::Window::ResizeLayout();

	fileButton.SetPos(XMFLOAT2(60, 4));
	fileButton.SetSize(XMFLOAT2(GetSize().x - 65, fileButton.GetSize().y));

	wi::scene::Scene& scene = editor->GetCurrentScene();
	wi::scene::ResponseComponent* response = scene.responses.GetComponent(entity);
	if (response != nullptr && response->resource.IsValid())
	{
		playstopButton.SetVisible(true);
		playstopButton.SetPos(XMFLOAT2(84, fileButton.GetPos().y + fileButton.GetSize().y + 4));
		playstopButton.SetSize(XMFLOAT2(GetSize().x - 90, playstopButton.GetSize().y));

		playonceCheckBox.SetVisible(true);
		playonceCheckBox.SetPos(XMFLOAT2(playstopButton.GetPos().x - playonceCheckBox.GetSize().x - 4, playstopButton.GetPos().y));
	}
	else
	{
		playstopButton.SetVisible(false);
		playonceCheckBox.SetVisible(false);
	}
}
