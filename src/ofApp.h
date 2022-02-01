#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp{

	public:

		void setup();
		void update();
		void draw();

		ofMesh charMesh;
		ofMesh background;
		ofMesh cloudMesh;
		ofMesh sunMesh;

		ofShader alphaTestShader;
		ofShader backgroundShader;
		ofShader cloudShader;
		ofShader sunShader;
		ofShader spriteSheetShader;

		ofImage alienImg;
		ofImage forest;
		ofImage cloudImg;
		ofImage sunImg;
		ofImage walkAnim;

		bool walkRight;
		bool walkLeft;

		glm::vec3 charPos;

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		
		
};
