#pragma once

#include "ofMain.h"
#include "VjState.h"

class OutputApp : public ofBaseApp
{
public:
	explicit OutputApp(const std::shared_ptr<VjState>& sharedState);

	void setup() override;
	void update() override;
	void draw() override;
	void keyPressed(int key) override;
	void windowResized(int w, int h) override;

private:
	void drawScene1(float t);
	void drawScene2(float t);
	void drawScene3(float t);

	std::shared_ptr<VjState> state;
	ofEasyCam cam2;
	ofEasyCam cam3;
};