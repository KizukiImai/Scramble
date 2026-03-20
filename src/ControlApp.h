#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "VjState.h"

class ControlApp : public ofBaseApp
{
public:
	explicit ControlApp(const std::shared_ptr<VjState>& sharedState);

	void setup() override;
	void draw() override;
	void keyPressed(int key) override;

private:
	void applyGuiToState();
	void syncStateToGui();

	std::shared_ptr<VjState> state;

	ofxPanel gui;

	ofParameter<int> sceneSelect;

	ofParameter<float> bpm;
	ofParameter<float> masterBrightness;
	ofParameter<bool> autoSceneCycle;
	ofParameter<float> autoSceneSeconds;

	ofParameter<bool> effectsEnabled;

	ofParameter<float> scene1Density;
	ofParameter<float> scene1Complexity;
	ofParameter<float> scene1Morph;
	ofParameter<float> scene1Speed;
	ofParameter<int> scene1Pattern;
	ofParameter<bool> scene1BpmPatternSwitch;
	ofParameter<int> scene1BpmPatternEveryBeats;

	ofParameter<int> scene2Primitive;
	ofParameter<float> scene2Count;
	ofParameter<float> scene2Spacing;
	ofParameter<float> scene2SpinSpeed;
	ofParameter<float> scene2Deform;
	ofParameter<float> scene2CamOrbit;

	ofParameter<float> scene3ObjectCount;
	ofParameter<float> scene3Spread;
	ofParameter<float> scene3Pulse;
	ofParameter<float> scene3LineCount;
	ofParameter<float> scene3CamTravel;
};