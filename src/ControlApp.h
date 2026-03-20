#pragma once

#include "ofMain.h"
#include "ofxImGui.h"
#include "imgui.h"
#include "VjState.h"

class ControlApp : public ofBaseApp
{
public:
	explicit ControlApp(const std::shared_ptr<VjState>& sharedState);

	void setup() override;
	void draw() override;
	void keyPressed(int key) override;

private:
	void drawSceneControls();
	void drawSequencer();

	std::shared_ptr<VjState> state;
	ofxImGui::Gui gui;
};