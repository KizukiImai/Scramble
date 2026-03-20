#include "ControlApp.h"

ControlApp::ControlApp(const std::shared_ptr<VjState>& sharedState)
	: state(sharedState)
{
}

void ControlApp::setup()
{
	ofSetFrameRate(60);
	gui.setup();
}

void ControlApp::draw()
{
	ofBackground(18);

	gui.begin();
	ImGui::Begin("VJ Control");

	int scene = static_cast<int>(state->currentScene);
	ImGui::Text("Scene");
	ImGui::RadioButton("Scene 1 (2D Line)", &scene, 0); ImGui::SameLine();
	ImGui::RadioButton("Scene 2 (3D Wire)", &scene, 1); ImGui::SameLine();
	ImGui::RadioButton("Scene 3 (Solid)", &scene, 2);
	state->currentScene = static_cast<SceneId>(scene);

	ImGui::Separator();
	drawSceneControls();

	ImGui::Separator();
	ImGui::SliderFloat("BPM", &state->bpm, 45.0f, 220.0f, "%.1f");
	ImGui::Text("Tap Tempo: press SPACE");
	ImGui::Text("Current Step: %d", state->currentStep);

	drawSequencer();

	ImGui::Separator();
	ImGui::SliderFloat("Master Brightness", &state->masterBrightness, 0.1f, 1.5f);
	ImGui::Checkbox("Auto Scene Cycle", &state->autoSceneCycle);
	ImGui::SliderFloat("Scene Cycle Seconds", &state->autoSceneSeconds, 4.0f, 64.0f, "%.1f");

	ImGui::Separator();
	ImGui::Text("Output Preview");
	if (state->outputFbo.isAllocated())
	{
		const ImVec2 sz(480, 270);
		auto texture = state->outputFbo.getTexture();
		auto textureData = texture.getTextureData();

		const ImTextureID texId = static_cast<ImTextureID>(textureData.textureID);
		ImGui::Image(ImTextureRef(texId), sz, ImVec2(0, 1), ImVec2(1, 0));
	}

	ImGui::End();
	gui.end();
}

void ControlApp::keyPressed(int key)
{
	if (key == ' ')
	{
		state->tapTempo();
	}
	if (key == '1') state->currentScene = SceneId::Scene1;
	if (key == '2') state->currentScene = SceneId::Scene2;
	if (key == '3') state->currentScene = SceneId::Scene3;
}

void ControlApp::drawSceneControls()
{
	if (state->currentScene == SceneId::Scene1)
	{
		ImGui::Text("Scene1 Params");
		ImGui::SliderInt("Pattern", &state->scene1.pattern, 0, 4);
		ImGui::SliderFloat("Density", &state->scene1.density, 80.0f, 480.0f);
		ImGui::SliderFloat("Complexity", &state->scene1.complexity, 1.0f, 10.0f);
		ImGui::SliderFloat("Morph", &state->scene1.morph, 0.0f, 1.0f);
		ImGui::SliderFloat("Speed", &state->scene1.speed, 0.1f, 5.0f);
	}
	else if (state->currentScene == SceneId::Scene2)
	{
		ImGui::Text("Scene2 Params");
		ImGui::SliderInt("Primitive", &state->scene2.primitive, 0, 4);
		ImGui::SliderFloat("Count", &state->scene2.count, 2.0f, 12.0f);
		ImGui::SliderFloat("Spacing", &state->scene2.spacing, 60.0f, 240.0f);
		ImGui::SliderFloat("Spin Speed", &state->scene2.spinSpeed, 0.0f, 180.0f);
		ImGui::SliderFloat("Deform", &state->scene2.deform, 0.0f, 1.0f);
		ImGui::SliderFloat("Camera Orbit", &state->scene2.camOrbit, 0.05f, 1.8f);
	}
	else
	{
		ImGui::Text("Scene3 Params");
		ImGui::SliderFloat("Object Count", &state->scene3.objectCount, 12.0f, 220.0f);
		ImGui::SliderFloat("Spread", &state->scene3.spread, 200.0f, 2000.0f);
		ImGui::SliderFloat("Pulse", &state->scene3.pulse, 0.0f, 1.0f);
		ImGui::SliderFloat("Line Count", &state->scene3.lineCount, 4.0f, 120.0f);
		ImGui::SliderFloat("Camera Travel", &state->scene3.camTravel, 0.05f, 1.3f);
	}
}

void ControlApp::drawSequencer()
{
	ImGui::Text("Step Sequencer (Glitch Strength)");

	if (ImGui::Button(state->glitchEnabled ? "Effects: ON (Click to OFF)" : "Effects: OFF (Click to ON)"))
	{
		state->glitchEnabled = !state->glitchEnabled;
	}

	for (int i = 0; i < 16; ++i)
	{
		ImGui::PushID(i);
		if (i == state->currentStep)
		{
			ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.9f, 0.35f, 0.35f, 1.0f));
		}

		ImGui::VSliderFloat("##step", ImVec2(18, 80), &state->sequence[static_cast<size_t>(i)], 0.0f, 1.0f, "");
		if (i == state->currentStep)
		{
			ImGui::PopStyleColor();
		}
		if (i < 15) ImGui::SameLine();
		ImGui::PopID();
	}
}