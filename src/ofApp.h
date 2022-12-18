#pragma once

#include "ofMain.h"
#include "list.h"
#ifdef _WIN32
#include <Windows.h>
#include "ofxWinMenu.h" // Menu addon
#else
#include <unistd.h>
#endif
#include "world_cup.h"
#include <cstdlib>
#define HEIGHT 20
#define WIDTH 20
#define LINELENGTH 25
#define LEFT_IMAGE	1
#define RIGHT_IMAGE 0

#define LEFT 0
#define UP 1
#define DOWN 2
#define RIGHT 3
#define NOGROUP -1
#define TOSS (rand()%2)

struct RankElem {
	string name_;
	int point_;
	struct list_elem elem;
	RankElem(string name, int point) : name_(name), point_(point) {}
};




typedef struct Cell {
	int y, x;
	bool direction[4];
	int group;
	bool visited;
	Cell() : x(-1), y(-1), direction{ false,false,false,false },group(NOGROUP),visited(false) {
		//memcpy(direction, false, sizeof direction);
	}
}Cell;

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
		NOT_START, ROUND_OF_16, QUATER_FINAL, SEMI_FINAL, FINAL // 라운드를 enumeration으로 저장하여 보다 가독성 좋게 하였다.
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


	ofxWinMenu* menu; // Menu object
	HWND hWnd; // Application window
	HWND hWndForeground; // current foreground window
	void appMenuFunction(string title, bool bChecked);
	
	struct list_t rank_list;


	void ReadRankFile();
	void SyncRankFIle();
	void AppendWinner();
	void ShowRank();
	bool see_rank;
	int dx[4] = { -1,0,0,1 };
	int dy[4] = { 0,-1,1,0 };
	Cell me;
	Cell dest;
	Cell maze[HEIGHT][WIDTH];
	bool play_maze;
	void InitMaze();
	void DrawMaze();
	void DrawCell(Cell& cell, float y, float x);
	void GiveGroupNumber(int row);
	void DigDownward(int row);
	bool Check(int ny, int nx);
	int group_number;

	void PrintMaze(void);
	void InitDest(void);
	bool BFS(int y, int x);
};

