#include "ofApp.h"	

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openFrameworks");

	ofBackground(239);
	ofSetColor(148);
	ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_MULTIPLY);

	this->font_size = 80;
	this->font.loadFont("fonts/Kazesawa-Bold.ttf", this->font_size, true, true, true);
}

//--------------------------------------------------------------
void ofApp::update() {

	this->base_location_list.clear();

	this->word = "HOPE";
	auto path_list = font.getStringAsPoints(this->word, true, false);

	for (auto& path : path_list) {

		vector<ofPolyline> outline = path.getOutline();

		for (int outline_index = 0; outline_index < outline.size(); outline_index++) {

			outline[outline_index] = outline[outline_index].getResampledByCount(30);
			auto vertices = outline[outline_index].getVertices();
			for (int vertices_index = 0; vertices_index < vertices.size(); vertices_index++) {

				auto location = glm::vec3(vertices[vertices_index].x - this->font.stringWidth(this->word) * 0.5, vertices[vertices_index].y + this->font.stringHeight(this->word) * 0.25, 0);
				this->base_location_list.push_back(location);
			}
		}
	}

	this->log_list.clear();
	
	int limit = 300;
	for (int i = 0; i < this->base_location_list.size(); i++) {

		vector<glm::vec2> log;
		log.push_back(glm::vec2(this->base_location_list[i]));
		this->log_list.push_back(log);
	}

	int step = 1;
	for (int i = 0; i < this->log_list.size(); i++) {

		int k = 0;
		while (true) {

			auto deg = ofMap(ofNoise(glm::vec3(this->log_list[i].back() * 0.0035, ofGetFrameNum() * 0.003 + k * 0.0005)), 0, 1, -360, 360);
			auto next = this->log_list[i].back() + glm::vec2(step * cos(deg * DEG_TO_RAD), step * sin(deg * DEG_TO_RAD));

			if (next.x < -limit || next.x > limit || next.y < -limit || next.y > limit) {

				break;
			}
			else {

				this->log_list[i].push_back(next);
			}

			k++;
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	ofTranslate(ofGetWidth() * 0.5, ofGetHeight() * 0.5);

	ofSetColor(200);
	ofSetLineWidth(1.2);
	for (int i = 0; i < this->log_list.size(); i++) {

		if (this->log_list[i].size() > 1) {

			ofNoFill();
			ofBeginShape();
			ofVertices(this->log_list[i]);
			ofEndShape();

			ofFill();
			ofDrawCircle(this->log_list[i].back(), 3);
		}
	}

	ofSetColor(0);
	this->font.drawString(this->word, this->font.stringWidth(this->word) * -0.5, this->font.stringHeight(this->word) * 0.25);

}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}