#pragma once

#include "ofMain.h"
#include <array>
#include <deque>

enum class SceneId
{
	Scene1 = 0,
	Scene2 = 1,
	Scene3 = 2
};

struct Scene1Params
{
	int pattern = 0;
	float density = 220.0f;
	float complexity = 4.0f;
	float morph = 0.5f;
	float speed = 1.0f;

	// BPM˜A“®‚Å pattern ‚ğ©“®Ø‘Ö
	bool bpmPatternSwitch = false;
	int bpmPatternEveryBeats = 1; // 1=–ˆ”, 2=2”‚²‚Æ...
};

struct Scene2Params
{
	int primitive = 0;
	float count = 7.0f;
	float spacing = 140.0f;
	float spinSpeed = 40.0f;
	float deform = 0.35f;
	float camOrbit = 0.35f;
};

struct Scene3Params
{
	float objectCount = 70.0f;
	float spread = 900.0f;
	float pulse = 0.45f;
	float lineCount = 36.0f;
	float camTravel = 0.25f;
};

class VjState
{
public:
	void setup(int outputWidth, int outputHeight);
	void resizeOutput(int outputWidth, int outputHeight);

	void update(double dt);
	void tapTempo();

	SceneId currentScene = SceneId::Scene1;
	Scene1Params scene1;
	Scene2Params scene2;
	Scene3Params scene3;

	float masterBrightness = 1.0f;
	bool autoSceneCycle = false;
	float autoSceneSeconds = 16.0f;

	float bpm = 120.0f;
	int currentStep = 0;
	bool stepAdvanced = false;
	std::array<float, 16> sequence = { 1.0f, 0.2f, 0.7f, 0.2f, 0.9f, 0.2f, 0.6f, 0.2f, 1.0f, 0.3f, 0.6f, 0.3f, 0.8f, 0.2f, 0.7f, 0.2f };

	bool glitchEnabled = true;
	float glitchAmount = 0.0f;
	ofFbo outputFbo;

private:
	double runningTime = 0.0;
	double stepAccumulator = 0.0;
	double sceneAccumulator = 0.0;
	std::deque<double> tapTimes;

	int scene1PatternStepCounter = 0;
};