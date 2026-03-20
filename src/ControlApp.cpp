#include "ControlApp.h"

ControlApp::ControlApp(const std::shared_ptr<VjState>& sharedState)
	: state(sharedState)
{
}

void ControlApp::setup()
{
	ofSetFrameRate(60);

	sceneSelect.set("Scene", static_cast<int>(state->currentScene), 0, 2);

	bpm.set("BPM", state->bpm, 45.0f, 220.0f);
	masterBrightness.set("Master Brightness", state->masterBrightness, 0.1f, 1.5f);
	autoSceneCycle.set("Auto Scene Cycle", state->autoSceneCycle);
	autoSceneSeconds.set("Scene Cycle Seconds", state->autoSceneSeconds, 4.0f, 64.0f);

	effectsEnabled.set("Effects Enabled", state->glitchEnabled);

	scene1Density.set("S1 Density", state->scene1.density, 80.0f, 480.0f);
	scene1Complexity.set("S1 Complexity", state->scene1.complexity, 1.0f, 10.0f);
	scene1Morph.set("S1 Morph", state->scene1.morph, 0.0f, 1.0f);
	scene1Speed.set("S1 Speed", state->scene1.speed, 0.1f, 5.0f);
	scene1Pattern.set("S1 Pattern", state->scene1.pattern, 0, 4);
	scene1BpmPatternSwitch.set("S1 BPM Pattern Switch", state->scene1.bpmPatternSwitch);
	scene1BpmPatternEveryBeats.set("S1 Switch Every Beats", state->scene1.bpmPatternEveryBeats, 1, 8);

	scene2Primitive.set("S2 Primitive", state->scene2.primitive, 0, 4);
	scene2Count.set("S2 Count", state->scene2.count, 2.0f, 12.0f);
	scene2Spacing.set("S2 Spacing", state->scene2.spacing, 60.0f, 240.0f);
	scene2SpinSpeed.set("S2 Spin Speed", state->scene2.spinSpeed, 0.0f, 180.0f);
	scene2Deform.set("S2 Deform", state->scene2.deform, 0.0f, 1.0f);
	scene2CamOrbit.set("S2 Camera Orbit", state->scene2.camOrbit, 0.05f, 1.8f);

	scene3ObjectCount.set("S3 Object Count", state->scene3.objectCount, 12.0f, 220.0f);
	scene3Spread.set("S3 Spread", state->scene3.spread, 200.0f, 2000.0f);
	scene3Pulse.set("S3 Pulse", state->scene3.pulse, 0.0f, 1.0f);
	scene3LineCount.set("S3 Line Count", state->scene3.lineCount, 4.0f, 120.0f);
	scene3CamTravel.set("S3 Camera Travel", state->scene3.camTravel, 0.05f, 1.3f);

	gui.setup("VJ Control");
	gui.add(sceneSelect);
	gui.add(bpm);
	gui.add(masterBrightness);
	gui.add(autoSceneCycle);
	gui.add(autoSceneSeconds);
	gui.add(effectsEnabled);

	gui.add(scene1BpmPatternSwitch);
	gui.add(scene1BpmPatternEveryBeats);
	gui.add(scene1Pattern);
	gui.add(scene1Density);
	gui.add(scene1Complexity);
	gui.add(scene1Morph);
	gui.add(scene1Speed);

	gui.add(scene2Primitive);
	gui.add(scene2Count);
	gui.add(scene2Spacing);
	gui.add(scene2SpinSpeed);
	gui.add(scene2Deform);
	gui.add(scene2CamOrbit);

	gui.add(scene3ObjectCount);
	gui.add(scene3Spread);
	gui.add(scene3Pulse);
	gui.add(scene3LineCount);
	gui.add(scene3CamTravel);
}

void ControlApp::draw()
{
	ofBackground(18);

	applyGuiToState();
	syncStateToGui();

	gui.draw();

	ofSetColor(220);
	ofDrawBitmapString("Tap Tempo: SPACE", 16, 640);
	ofDrawBitmapString("Current Step: " + ofToString(state->currentStep), 16, 660);

	ofDrawBitmapString("Output Preview", 16, 690);
	if (state->outputFbo.isAllocated())
	{
		ofSetColor(255);
		state->outputFbo.draw(16.0f, 700.0f, 480.0f, 270.0f);
	}
}

void ControlApp::keyPressed(int key)
{
	if (key == ' ')
	{
		state->tapTempo();
		bpm = state->bpm;
	}
	if (key == '1')
	{
		state->currentScene = SceneId::Scene1;
		sceneSelect = 0;
	}
	if (key == '2')
	{
		state->currentScene = SceneId::Scene2;
		sceneSelect = 1;
	}
	if (key == '3')
	{
		state->currentScene = SceneId::Scene3;
		sceneSelect = 2;
	}
}

void ControlApp::applyGuiToState()
{
	state->currentScene = static_cast<SceneId>(sceneSelect.get());

	state->bpm = bpm.get();
	state->masterBrightness = masterBrightness.get();
	state->autoSceneCycle = autoSceneCycle.get();
	state->autoSceneSeconds = autoSceneSeconds.get();

	state->glitchEnabled = effectsEnabled.get();

	state->scene1.bpmPatternSwitch = scene1BpmPatternSwitch.get();
	state->scene1.bpmPatternEveryBeats = scene1BpmPatternEveryBeats.get();
	if (!state->scene1.bpmPatternSwitch)
	{
		state->scene1.pattern = scene1Pattern.get();
	}
	state->scene1.density = scene1Density.get();
	state->scene1.complexity = scene1Complexity.get();
	state->scene1.morph = scene1Morph.get();
	state->scene1.speed = scene1Speed.get();

	state->scene2.primitive = scene2Primitive.get();
	state->scene2.count = scene2Count.get();
	state->scene2.spacing = scene2Spacing.get();
	state->scene2.spinSpeed = scene2SpinSpeed.get();
	state->scene2.deform = scene2Deform.get();
	state->scene2.camOrbit = scene2CamOrbit.get();

	state->scene3.objectCount = scene3ObjectCount.get();
	state->scene3.spread = scene3Spread.get();
	state->scene3.pulse = scene3Pulse.get();
	state->scene3.lineCount = scene3LineCount.get();
	state->scene3.camTravel = scene3CamTravel.get();
}

void ControlApp::syncStateToGui()
{
	sceneSelect = static_cast<int>(state->currentScene);
	bpm = state->bpm;
	effectsEnabled = state->glitchEnabled;
	scene1Pattern = state->scene1.pattern;
}