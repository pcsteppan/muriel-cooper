#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	font.load("HelveticaBold-Regular.ttf", 200);
	ofEnableDepthTest();

	ofRectangle r = font.getStringBoundingBox("verziehen", 0, 0);

	fbo.allocate(r.getWidth(), r.getHeight());
	fbo.begin();
	ofClear(255, 255, 255, 255);
	ofSetColor(0);
	font.drawString("verziehen", -r.x, -r.y);
	fbo.end();
}

//--------------------------------------------------------------
void ofApp::update() {

}

//--------------------------------------------------------------
void ofApp::draw() {

	ofSetColor(255);

	ofBackground(20,20,20);

	//cam.enableOrtho();
	cam.begin();
	ofScale(1, 1, 1);
	//fbo.draw(-fbo.getWidth()/2, -fbo.getHeight()/2);

	ofMesh m;
	m.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);

	vector<glm::vec2> edges = vector<glm::vec2>();

	edges.push_back(glm::vec2(-1.0f, 0.0f));
	edges.push_back(glm::vec2((((float)mouseX / (float)ofGetWidth()) - 0.5f) * 2.0f, 0.0f));

	//cout << edges[1] << endl;

	vector<glm::vec2> bent_paper = bend(
		edges,
		1.0f,
		10
	);

	float px = 0;
	float py = 0;
	float totalLength = 0;

	for (unsigned int i = 0; i < bent_paper.size(); i++) {
		float pct = ofMap(i, 0, bent_paper.size(), 0, 1);

		//float theta = ofMap(ofNoise(((float)i + (ofGetElapsedTimef() * 60.0f)) / mouseX), 0, 1, -PI * 0.6, PI * 0.6);

		//px += cos(theta);
		//py += sin(theta);
		//float x = ofMap(pct, 0, 1, -fbo.getWidth() / 2, fbo.getWidth() / 2);
		//float z = sin(pct * 5) * 300;
		//float x = ofMap(pct, 0, 1, -fbo.getWidth() / 2, fbo.getWidth() / 2);
		//float z = py;
		if (i > 0) {
			totalLength += glm::fastDistance(bent_paper[i - 1], bent_paper[i]);
		}

		m.addVertex(ofPoint(bent_paper[i].x*fbo.getWidth()/2, -fbo.getHeight() / 2, bent_paper[i].y*fbo.getWidth() / 2));
		m.addVertex(ofPoint(bent_paper[i].x*fbo.getWidth()/2, fbo.getHeight() / 2, bent_paper[i].y*fbo.getWidth() / 2));


		m.addTexCoord(fbo.getTexture().getCoordFromPercent(pct, 1));
		m.addTexCoord(fbo.getTexture().getCoordFromPercent(pct, 0));
	}
	cout << totalLength << endl;

	ofSetColor(255);
	fbo.getTexture().bind();
	//fbo.begin();
	m.draw();
	//fbo.end();
	fbo.getTexture().unbind();

	//fbo.draw(0, 0);
	// uncomment for debug
	ofSetColor(200,200,200,50);
	m.drawWireframe();

	cam.end();
}

vector<glm::vec2> ofApp::bend(vector<glm::vec2> pVecs, float radius, int iterations) {
	//if (debug) { drawCirclesAtVectors(pVecs); }
	//drawPolyLine(pVecs);
	iterations -= 1;
	if (iterations == 0) {
		return pVecs;
	}
	vector<glm::vec2> newVecs = vector<glm::vec2>();
	for (unsigned int i = 1; i < pVecs.size(); i++) {
		newVecs.push_back(pVecs[i - 1]);
		float distance = glm::distance(pVecs[i - 1], pVecs[i]);
		float diameter = 2 * radius;
		glm::vec2 midpoint = glm::mix(pVecs[i - 1], pVecs[i], 0.5f);
		glm::vec2 relativeMidpoint = midpoint - pVecs[i - 1];
		float wiggle = (glm::length(relativeMidpoint) - radius);
		float wiggleRoom = ((abs(wiggle) / diameter) + 1) * 4;
		//cout << wiggle << endl;
		glm::vec2 chordAltitude = glm::normalize(relativeMidpoint) * wiggle * squashedNoise(ofGetElapsedTimef() / 6.2f + i * 10, wiggleRoom);
		glm::vec2 sinAtChordAltitude =
			glm::rotate(
				glm::normalize(relativeMidpoint),
				glm::half_pi<float>()
			)
			* sin(
				acos(
				(glm::length(relativeMidpoint) + glm::length(chordAltitude))
					/ radius
				)
			)
			* radius
			* squashedNoise(ofGetElapsedTimef() / 3 + i * 5, wiggleRoom);

		glm::vec2 newVec = midpoint + chordAltitude + sinAtChordAltitude;
		newVecs.push_back(newVec);
	}
	newVecs.push_back(pVecs[pVecs.size() - 1]);
	return bend(newVecs, radius / 2, iterations);
}

float ofApp::squashedNoise(float t, float wiggleRoom) {
	float x = (ofNoise(t) * 2 - 1) * max(wiggleRoom, 1.0f);
	float sigmoid = 1.0f / (pow(glm::e<float>(), -x) + 1.0f);
	sigmoid -= 0.5f;
	sigmoid *= 2;
	return sigmoid;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}
