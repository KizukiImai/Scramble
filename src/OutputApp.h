#pragma once

#include "ofMain.h"
#include "ofxPostProcessing.h"
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

	void setupPostProcessing(int w, int h);
	void updatePostEffects();

	std::shared_ptr<VjState> state;

	ofxPostProcessing post;
	std::shared_ptr<itg::RGBShiftPass> rgbShiftPass;
	std::shared_ptr<itg::BloomPass> bloomPass;
	std::shared_ptr<itg::NoiseWarpPass> noiseWarpPass;
	std::shared_ptr<itg::ZoomBlurPass> zoomBlurPass;
	std::shared_ptr<itg::KaleidoscopePass> kaleidoscopePass;

	ofEasyCam cam2;
	ofEasyCam cam3;
};