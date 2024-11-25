#include <iostream>
#include "GL/gl3w.h"
#include <string>
#include "Loader.h"
// #include "ColorCube.h"
#include "Cow.h"
#include "Viewer.h"

class MyGlWindow {
public:
	MyGlWindow(int w, int h);
	void initialize();
	void draw();
	Viewer* m_viewer;
	Viewer* getViewer() const { return m_viewer; };
	void setSize(int width, int height) {
		m_width = width;
		m_height = height;
	};
	void setAspect(float aspect) {
		m_aspect = aspect;
		m_viewer->setAspectRatio(aspect);
	};
private:
	ShaderProgram* m_shaderProgram;
	Cow* m_cube;
	
	int m_width;
	int m_height;
	float m_aspect;
	void setupBuffer();
};