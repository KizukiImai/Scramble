#include "OutputApp.h"

OutputApp::OutputApp(const std::shared_ptr<VjState>& sharedState)
	: state(sharedState)
{
}

void OutputApp::setup()
{
	ofSetFrameRate(60);
	ofEnableDepthTest();
	state->setup(ofGetWidth(), ofGetHeight());
}

void OutputApp::update()
{
	state->update(ofGetLastFrameTime());
}

void OutputApp::draw()
{
	const float t = ofGetElapsedTimef();

	state->outputFbo.begin();
	ofClear(0, 0, 0, 255);
	ofBackground(0);

	switch (state->currentScene)
	{
	case SceneId::Scene1:
		drawScene1(t);
		break;
	case SceneId::Scene2:
		drawScene2(t);
		break;
	case SceneId::Scene3:
		drawScene3(t);
		break;
	}

	state->outputFbo.end();

	ofSetColor(255);
	state->outputFbo.draw(0, 0, static_cast<float>(ofGetWidth()), static_cast<float>(ofGetHeight()));
}

void OutputApp::keyPressed(int key)
{
	if (key == ' ')
	{
		state->tapTempo();
	}
	if (key == '1') state->currentScene = SceneId::Scene1;
	if (key == '2') state->currentScene = SceneId::Scene2;
	if (key == '3') state->currentScene = SceneId::Scene3;
}

void OutputApp::windowResized(int w, int h)
{
	state->resizeOutput(w, h);
}

void OutputApp::drawScene1(float t)
{
	ofPushStyle();
	ofDisableDepthTest();
	ofNoFill();
	ofSetLineWidth(1.6f);

	const auto& p = state->scene1;
	const float cx = ofGetWidth() * 0.5f;
	const float cy = ofGetHeight() * 0.5f;
	const float d = p.density;
	const float c = p.complexity;
	const float m = p.morph;
	const float spd = p.speed;

	ofSetColor(255 * state->masterBrightness, 210, 255);

	for (int layer = 0; layer < 6; ++layer)
	{
		const float r = d * (0.35f + layer * 0.14f);
		ofPolyline line;
		const int segments = 260;

		for (int i = 0; i <= segments; ++i)
		{
			const float a = ofMap(i, 0, segments, 0.0f, TWO_PI);
			float x = 0.0f;
			float y = 0.0f;

			switch (p.pattern)
			{
			case 0: // Lissajous
				x = sinf(a * (2.0f + c) + t * spd) * r;
				y = cosf(a * (3.0f + c * 0.7f) - t * spd * 1.2f) * r;
				break;
			case 1: // Rose
			{
				const float rr = r * cosf(a * (c + layer * 0.3f) + t * spd);
				x = cosf(a) * rr;
				y = sinf(a) * rr;
				break;
			}
			case 2: // Spiro-like
			{
				const float k = 0.35f + m * 0.6f;
				x = r * ((1.0f - k) * cosf(a + t * spd) + k * cosf(((1.0f - k) / k) * a));
				y = r * ((1.0f - k) * sinf(a + t * spd) - k * sinf(((1.0f - k) / k) * a));
				break;
			}
			case 3: // Polygon morph
			{
				const float n = 3.0f + c * 2.0f;
				const float poly = cosf(PI / n) / cosf(fmodf(a, TWO_PI / n) - PI / n);
				const float rr = ofLerp(r * poly * 0.4f, r, m);
				x = cosf(a + t * spd * 0.3f) * rr;
				y = sinf(a + t * spd * 0.3f) * rr;
				break;
			}
			default: // Wave tunnel
				x = cosf(a) * r + sinf(a * (4.0f + c) + t * spd) * r * 0.25f * m;
				y = sinf(a) * r + cosf(a * (5.0f + c) - t * spd) * r * 0.25f * (1.0f - m);
				break;
			}

			line.addVertex(cx + x, cy + y);
		}

		line.close();
		line.draw();
	}

	ofPopStyle();
}

void OutputApp::drawScene2(float t)
{
	ofPushStyle();
	ofEnableDepthTest();
	ofNoFill();
	ofSetLineWidth(1.2f);

	const auto& p = state->scene2;
	const float orbitR = 680.0f;
	const float camA = t * p.camOrbit;
	const glm::vec3 camPos(cosf(camA) * orbitR, sinf(camA * 0.55f) * 280.0f, sinf(camA) * orbitR);

	cam2.setPosition(camPos);
	cam2.lookAt(glm::vec3(0, 0, 0));
	cam2.begin();

	ofSetColor(120, 230, 255, 220);
	const int n = static_cast<int>(p.count);
	const float spacing = p.spacing;

	for (int z = -n; z <= n; ++z)
	{
		for (int x = -n; x <= n; ++x)
		{
			ofPushMatrix();
			const float px = x * spacing;
			const float pz = z * spacing;
			const float py = sinf(t * 1.4f + (x + z) * 0.35f) * (90.0f * p.deform);
			ofTranslate(px, py, pz);

			const float rot = t * p.spinSpeed + (x - z) * 7.0f;
			ofRotateDeg(rot, 1, 1, 0);

			const float size = 35.0f + 28.0f * (0.5f + 0.5f * sinf(t * 1.1f + (x * 0.3f + z * 0.21f)));
			switch (p.primitive)
			{
			case 0:
				ofDrawBox(size);
				break;
			case 1:
				ofDrawSphere(size * 0.65f);
				break;
			case 2:
				ofDrawCone(size * 0.6f, size * 1.2f);
				break;
			case 3:
				ofDrawCylinder(size * 0.5f, size * 1.2f);
				break;
			default:
				ofDrawIcoSphere(size * 0.7f);
				break;
			}
			ofPopMatrix();
		}
	}

	// 追加: 空間アクセント用の別立体
	ofFill();
	const int accentCount = std::max(24, n * 6);
	const float accentRadius = spacing * (n + 1.5f);

	for (int i = 0; i < accentCount; ++i)
	{
		const float fi = static_cast<float>(i);
		const float a = fi * 0.41f + t * (0.25f + p.camOrbit * 0.4f);

		const float r = accentRadius + 120.0f * sinf(fi * 0.7f + t * 0.8f);
		const glm::vec3 pos(
			cosf(a) * r,
			sinf(a * 1.9f + fi * 0.23f) * 260.0f,
			sinf(a) * r
		);

		ofPushMatrix();
		ofTranslate(pos);
		ofRotateDeg(t * (18.0f + p.spinSpeed * 0.4f) + fi * 12.0f, 1, 1, 0);

		const float s = 14.0f + 22.0f * (0.5f + 0.5f * sinf(t * 2.0f + fi * 0.37f));
		switch (i % 4)
		{
		case 0:
			ofSetColor(255, 120, 150, 140);
			ofDrawIcoSphere(s * 0.75f);
			break;
		case 1:
			ofSetColor(120, 220, 255, 130);
			ofDrawCone(s * 0.6f, s * 1.7f);
			break;
		case 2:
			ofSetColor(255, 220, 120, 120);
			ofDrawCylinder(s * 0.55f, s * 1.8f);
			break;
		default:
			ofSetColor(170, 255, 180, 120);
			ofDrawBox(s);
			break;
		}
		ofPopMatrix();
	}

	ofNoFill();
	cam2.end();
	ofPopStyle();
}

void OutputApp::drawScene3(float t)
{
	ofPushStyle();
	ofEnableDepthTest();
	ofFill();

	const auto& p = state->scene3;
	const float orbitR = 900.0f;
	const float camA = t * p.camTravel;
	const glm::vec3 camPos(cosf(camA) * orbitR, 220.0f + sinf(camA * 0.7f) * 260.0f, sinf(camA) * orbitR);

	cam3.setPosition(camPos);
	cam3.lookAt(glm::vec3(0, 0, 0));
	cam3.begin();

	ofSetColor(255 * state->masterBrightness);

	const int count = static_cast<int>(p.objectCount);
	const float spread = p.spread;
	for (int i = 0; i < count; ++i)
	{
		const float fi = static_cast<float>(i);
		const float a = fi * 0.32f + t * 0.28f;
		const float r = spread * (0.2f + 0.8f * (fi / std::max(1.0f, p.objectCount)));
		glm::vec3 pos(cosf(a * 1.3f) * r, sinf(a * 0.9f) * (spread * 0.35f), sinf(a * 1.7f) * r);

		ofPushMatrix();
		ofTranslate(pos);
		ofRotateDeg(t * 40.0f + fi * 7.0f, 1, 1, 0);

		const float pulse = 12.0f + 32.0f * (0.5f + 0.5f * sinf(t * 2.2f + fi * 0.21f)) * (0.2f + p.pulse);
		if ((i % 3) == 0)
		{
			ofSetColor(255, 120, 140, 220);
			ofDrawBox(pulse);
		}
		else
		{
			ofSetColor(120, 200, 255, 220);
			ofDrawSphere(pulse * 0.55f);
		}
		ofPopMatrix();
	}

	ofSetColor(255, 255, 255, 180);
	ofSetLineWidth(1.0f);
	const int lines = static_cast<int>(p.lineCount);
	for (int i = 0; i < lines; ++i)
	{
		const float a = (TWO_PI / std::max(lines, 1)) * i + t * 0.2f;
		const glm::vec3 p1(cosf(a) * spread, -220.0f, sinf(a) * spread);
		const glm::vec3 p2(cosf(a + PI * 0.35f) * spread, 220.0f, sinf(a + PI * 0.35f) * spread);
		ofDrawLine(p1, p2);
	}

	cam3.end();
	ofPopStyle();
}