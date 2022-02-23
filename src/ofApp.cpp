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
	buildMesh(cloudMesh, 0.25, 0.15, glm::vec3(0.0, 0.0, 0.0));
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

	cam.position = glm::vec3(-1, 0, 0);
	glm::mat4 view = glm::mat4();
	glm::mat4 proj = glm::ortho(-1.33f, 1.33f, -1.0f, 1.0f, 0.0f, 10.0f);
	ofDisableBlendMode();
	ofEnableDepthTest();

	static float frame = 0.0;
	frame = (frame > 10) ? 0.0 : frame += 0.2;
	glm::vec2 spriteSize = glm::vec2(0.28, 0.19);
	glm::vec2 spriteFrame = glm::vec2((int)frame % 3, (int)frame / 3);

	spriteSheetShader.begin();
	spriteSheetShader.setUniform2fv("size", glm::value_ptr(spriteSize));
	spriteSheetShader.setUniform2fv("offset", glm::value_ptr(spriteFrame));
	
	glm::mat4 charTransform = transformMat(charPos, 0.0, glm::vec3(1.0, 1.0, 1.0));
	spriteSheetShader.setUniformMatrix4f("model", charTransform);
	spriteSheetShader.setUniformMatrix4f("view", view);
	spriteSheetShader.setUniformMatrix4f("proj", proj);
	if (walkRight) {
	   float speed = 0.4 * ofGetLastFrameTime();
	   charPos += glm::vec3(speed, 0.0, 0.0);
	   charTransform = transformMat(charPos, 0.0, glm::vec3(1.0, 1.0, 1.0));
	   spriteSheetShader.setUniformMatrix4f("model", charTransform);
	   spriteSheetShader.setUniform1i("invertX", false);
	}
	else if (walkLeft) {
		float speed = 0.4 * ofGetLastFrameTime();
		charPos -= glm::vec3(speed, 0.0, 0.0);
		charTransform = transformMat(charPos, 0.0, glm::vec3(1.0, 1.0, 1.0));
		spriteSheetShader.setUniformMatrix4f("model", charTransform);
		spriteSheetShader.setUniform1i("invertX", true);
	}
	spriteSheetShader.setUniformTexture("tex", walkAnim, 0);
	
	charMesh.draw();

	spriteSheetShader.end();

	alphaTestShader.begin();

	
	alphaTestShader.setUniformTexture("tex", forest, 0);
	alphaTestShader.setUniformMatrix4f("model", glm::translate(glm::vec3(0.0, 0.0, -0.5)));
	alphaTestShader.setUniformMatrix4f("view", view);
	alphaTestShader.setUniformMatrix4f("proj", proj);
	background.draw();
	
	alphaTestShader.end();

	cloudShader.begin();


	ofDisableDepthTest();
	// alpha blending
	ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_ALPHA);

	// draw clounds
	cloudShader.setUniformTexture("tex", cloudImg, 0);

	//glm::mat4 transformCloud1 = transformMat(glm::vec3(-0.55, 0.0, 0.0), 0.0f, glm::vec3(1.5, 1, 1));
	
	static float rotation = 0.0f;
	rotation += 0.1f;
	//construct the transform for our un-rotated cloud
	glm::mat4 translationA = glm::translate(glm::vec3(-0.55, 0.0, 0.0));
	glm::mat4 scaleA = glm::scale(glm::vec3(1.5, 1, 1));
	glm::mat4 transformA = translationA * scaleA;

	//apply a rotation to that
	glm::mat4 ourRotation = glm::rotate(rotation, glm::vec3(0.0, 0.0, 1.0));
	glm::mat4 newMatrix = translationA * ourRotation * inverse(translationA);
	glm::mat4 transformCloud1 = newMatrix * transformA;
	cloudShader.setUniformMatrix4f("model", transformCloud1);
	cloudShader.setUniformMatrix4f("view", view);
	cloudShader.setUniformMatrix4f("proj", proj);
	cloudMesh.draw();

	glm::mat4 transformCloud2 = transformMat(glm::vec3(0.4, 0.2, 0.0), 1.0f, glm::vec3(1, 1, 1));
	cloudShader.setUniformMatrix4f("model", transformCloud2);
	cloudShader.setUniformMatrix4f("view", view);
	cloudShader.setUniformMatrix4f("proj", proj);
	cloudMesh.draw();

	// additive blending
	ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_ADD);

	// draw sun
	cloudShader.setUniformTexture("tex", sunImg, 0);
	glm::mat4 transformSun = glm::mat4();
	cloudShader.setUniformMatrix4f("model", glm::translate(glm::vec3(0.0, 0.0, -0.5)));
	cloudShader.setUniformMatrix4f("view", view);
	cloudShader.setUniformMatrix4f("proj", proj);
	sunMesh.draw();

	cloudShader.end();
	
}

glm::mat4 ofApp::transformMat(glm::vec3 trans, float rot, glm::vec3 scale)
{
	using glm::mat4;
	mat4 translation = glm::translate(trans);
	mat4 rotation = glm::rotate(rot, glm::vec3(0.0, 0.0, 1.0));
	mat4 scaler = glm::scale(scale);
	return translation * rotation * scaler;
}

glm::mat4 ofApp::viewMatrix(CameraData cam) {
	return glm::inverse(transformMat(cam.position, cam.rotation, glm::vec3(1, 1, 1)));
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


