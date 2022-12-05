#pragma once

#include "ofMain.h"

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif
#include "world_cup.h"
#include <cstdlib>


class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();


	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	ofTrueTypeFont font;

	enum Round {
		NOT_START, ROUND_OF_16, QUATER_FINAL, SEMI_FINAL, FINAL
	};
	bool game_over;

	void InitWorldCup();
	Professor* match_up[32];
	Professor* left;
	Professor* right;
	Professor* champion;
	/* 0 : 16 , 1 : 8, 2 : 4, 1 : 2 */
	Round stage;
	int pos;
};

