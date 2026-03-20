#include "VjState.h"

void VjState::setup(int outputWidth, int outputHeight)
{
	resizeOutput(outputWidth, outputHeight);
	runningTime = 0.0;
	stepAccumulator = 0.0;
	sceneAccumulator = 0.0;
	currentStep = 0;
	stepAdvanced = false;
}

void VjState::resizeOutput(int outputWidth, int outputHeight)
{
	ofFbo::Settings s;
	s.width = outputWidth;
	s.height = outputHeight;
	s.internalformat = GL_RGBA;
	s.useDepth = true;
	s.useStencil = true;
	s.depthStencilAsTexture = true; // ‚±‚±‚ð false -> true
	s.textureTarget = GL_TEXTURE_2D;
	outputFbo.allocate(s);
}

void VjState::update(double dt)
{
	runningTime += dt;
	stepAdvanced = false;

	if (autoSceneCycle)
	{
		sceneAccumulator += dt;
		if (sceneAccumulator >= autoSceneSeconds)
		{
			sceneAccumulator = 0.0;
			currentScene = static_cast<SceneId>((static_cast<int>(currentScene) + 1) % 3);
		}
	}

	const double stepsPerSecond = (bpm / 60.0) * 4.0;
	const double stepDuration = 1.0 / std::max(stepsPerSecond, 0.001);

	stepAccumulator += dt;
	while (stepAccumulator >= stepDuration)
	{
		stepAccumulator -= stepDuration;
		currentStep = (currentStep + 1) % 16;
		stepAdvanced = true;
		glitchAmount = sequence[static_cast<size_t>(currentStep)];
	}
}

void VjState::tapTempo()
{
	const double now = ofGetElapsedTimef();
	tapTimes.push_back(now);
	while (tapTimes.size() > 8)
	{
		tapTimes.pop_front();
	}

	if (tapTimes.size() >= 2)
	{
		double sum = 0.0;
		int count = 0;
		for (size_t i = 1; i < tapTimes.size(); ++i)
		{
			const double interval = tapTimes[i] - tapTimes[i - 1];
			if (interval > 0.08 && interval < 2.0)
			{
				sum += interval;
				++count;
			}
		}
		if (count > 0)
		{
			const double avg = sum / static_cast<double>(count);
			bpm = static_cast<float>(ofClamp(60.0 / avg, 45.0, 220.0));
		}
	}

	stepAccumulator = 0.0;
	currentStep = 0;
}