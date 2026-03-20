#include "ofMain.h"
#include "OutputApp.h"
#include "ControlApp.h"
#include "VjState.h"

//========================================================================
int main( ){

	//Use ofGLFWWindowSettings for more options like multi-monitor fullscreen
	ofGLFWWindowSettings outputSettings;
	outputSettings.setGLVersion(4,1); // Ensure we have a modern OpenGL context for shaders
	outputSettings.setSize(1280, 720);
	outputSettings.setPosition(glm::vec2(20, 20));
	outputSettings.windowMode = OF_WINDOW;
	outputSettings.title = "VJ Output";
	auto outputWindow = ofCreateWindow(outputSettings);

	ofGLFWWindowSettings uiSettings;
	uiSettings.setGLVersion(4, 1); // Match OpenGL version for shared context
	uiSettings.setSize(900, 760);
	uiSettings.setPosition(glm::vec2(1320, 20));
	uiSettings.windowMode = OF_WINDOW;
	uiSettings.title = "VJ Control";
	uiSettings.shareContextWith = outputWindow;
	auto uiWindow = ofCreateWindow(uiSettings);

	auto sharedState = std::make_shared<VjState>();
	auto outputApp = std::make_shared<OutputApp>(sharedState);
	auto controlApp = std::make_shared<ControlApp>(sharedState);

	ofRunApp(outputWindow, outputApp);
	ofRunApp(uiWindow, controlApp);
	ofRunMainLoop();

}
