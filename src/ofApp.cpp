#include "ofApp.h"



void buildMesh(ofMesh& mesh, float w, float h, glm::vec3 pos)
{
	float verts[] = { -w + pos.x, -h + pos.y, pos.z,
	-w + pos.x, h + pos.y, pos.z,
	w + pos.x, h + pos.y, pos.z,
	w + pos.x, -h + pos.y, pos.z };
	float uvs[] = { 0,0, 0,1, 1,1, 1,0 };
	for (int i = 0; i < 4; ++i) {
		int idx = i * 3;
		int uvIdx = i * 2;
		mesh.addVertex(glm::vec3(verts[idx], verts[idx + 1], verts[idx + 2]));
		mesh.addTexCoord(glm::vec2(uvs[uvIdx], uvs[uvIdx + 1]));
	}
	ofIndexType indices[6] = { 0,1,2,2,3,0 };
	mesh.addIndices(indices, 6);
}

//--------------------------------------------------------------
void ofApp::setup(){

	ofDisableArbTex();
	ofEnableDepthTest();

	walkRight = false;
	walkLeft = false;

	buildMesh(charMesh, 0.1, 0.2, glm::vec3(0.0, -0.25, 0.0));
	buildMesh(background, 1.0, 1.0, glm::vec3(0.0, 0.0, 0.5));
	buildMesh(cloudMesh, 0.25, 0.15, glm::vec3(-0.55, 0.0, 0.0));
	buildMesh(sunMesh, 1.0, 1.0, glm::vec3(0.0, 0.0, 0.4));

	alienImg.load("alien.png");
	forest.load("forest.png");
	cloudImg.load("cloud.png");
	sunImg.load("sun.png");
	walkAnim.load("walk_sheet.png");

	alphaTestShader.load("passthrough.vert", "alphaTest.frag");
	backgroundShader.load("passthrough.vert", "texture.frag");
	cloudShader.load("passthrough.vert", "cloud.frag");
	spriteSheetShader.load("sprite_sheet.vert", "alphaTest.frag");
}

//--------------------------------------------------------------
void ofApp::update(){
	
}

//--------------------------------------------------------------
void ofApp::draw(){

	ofDisableBlendMode();
	ofEnableDepthTest();

	static float frame = 0.0;
	frame = (frame > 10) ? 0.0 : frame += 0.2;
	glm::vec2 spriteSize = glm::vec2(0.28, 0.19);
	glm::vec2 spriteFrame = glm::vec2((int)frame % 3, (int)frame / 3);

	spriteSheetShader.begin();
	spriteSheetShader.setUniform2fv("size", glm::value_ptr(spriteSize));
	spriteSheetShader.setUniform2fv("offset", glm::value_ptr(spriteFrame));
	if (walkRight) {
	   float speed = 0.4 * ofGetLastFrameTime();
	   charPos += glm::vec3(speed, 0.0, 0.0);
	   spriteSheetShader.setUniform3fv("translation", glm::value_ptr(charPos));
	   spriteSheetShader.setUniform1i("invertX", false);
	}
	else if (walkLeft) {
		float speed = 0.4 * ofGetLastFrameTime();
		charPos -= glm::vec3(speed, 0.0, 0.0);
		spriteSheetShader.setUniform3fv("translation", glm::value_ptr(charPos));
		spriteSheetShader.setUniform1i("invertX", true);
	}
	spriteSheetShader.setUniformTexture("tex", walkAnim, 0);
	
	charMesh.draw();

	spriteSheetShader.end();

	alphaTestShader.begin();

	

	alphaTestShader.setUniformTexture("tex", forest, 0);
	background.draw();
	
	alphaTestShader.end();

	cloudShader.begin();


	ofDisableDepthTest();
	// alpha blending
	ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_ALPHA);

	cloudShader.setUniformTexture("tex", cloudImg, 0);
	cloudMesh.draw();

	// additive blending
	ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_ADD);

	cloudShader.setUniformTexture("tex", sunImg, 0);
	sunMesh.draw();

	cloudShader.end();
	
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == ofKey::OF_KEY_RIGHT) {
		walkRight = true;
	}
	if (key == ofKey::OF_KEY_LEFT) {
		walkLeft = true;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	if (key == ofKey::OF_KEY_RIGHT) {
		walkRight = false;
	}
	if (key == ofKey::OF_KEY_LEFT) {
		walkLeft = false;
	}
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}



//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}


