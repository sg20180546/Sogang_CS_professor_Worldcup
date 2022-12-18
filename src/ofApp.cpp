#include "ofApp.h"
#include <string>
#include <windows.h>
#include <shellapi.h>
int xPos;
//--------------------------------------------------------------
ofImage sogang_logo;
ofImage title;
ofImage be_happy;
bool in_play;
Professor* match_up[32];


void ofApp::setup() {

	list_init(&rank_list);

	ReadRankFile();

    ofBackground(ofColor::white); // 하얀색 화면으로
    sogang_logo.load("image/download.png"); // 서강대학교 로고를 띄운다
    title.load("image/title.png"); // 오픈프레임워크에서는 한글 폰트 띄우는게 안되서 서강대학교 컴퓨터공학과 대학원 ... 제목을 띄운다
	be_happy.load("image/be_happy.png"); // 행복해지는 진학 버튼
	font.load(OF_TTF_SANS, 20);
    ofSetBackgroundAuto(false);
    in_play = false;
    stage = NOT_START; // state : 아직 시작하지 않은 상태다.

	hWnd = WindowFromDC(wglGetCurrentDC());

	menu = new ofxWinMenu(this, hWnd);
	menu->CreateMenuFunction(&ofApp::appMenuFunction);
	HMENU hMenu = menu->CreateWindowMenu();

	HMENU hPopup = menu->AddPopupMenu(hMenu, "File");
	menu->AddPopupItem(hPopup, "심심한데 미로찾기 하기", false, false); // Not checked and not auto-checked
	menu->AddPopupItem(hPopup, "가장 인기있는 연구실", false, false); // Not checked and not auto-checked
//	menu->AddPopupSeparator(hPopup);
//	menu->AddPopupItem(hPopup, "Exit", false, false);
	menu->SetWindowMenu();
}
void ofApp::update() {
}
void ofApp::draw() {
	if (see_rank == true) {
		ShowRank();
		return;
	}
	if (play_maze == true) {
		DrawMaze();
		if (me.x == dest.x&&me.y == dest.y) {
			ofSetColor(0);
			font.drawString("YOU CLEAR!", 200, 200);
		}
		return;
	}


    if (!in_play) { // 첫 화면
        ofClear(255);
        ofSetColor(255);

        title.resize(560, 100);
        title.draw(350, 150);
        sogang_logo.draw(512, 300);
        ofScale(2);
        ofSetColor(248, 53, 76);
        ofDrawRectangle(130, 270, 400, 60); // 빨간색 사각형, 클릭했을때 event는 아래에서 event를 listen 할수 있게 구현할것이다.
        ofSetColor(ofColor::black);

        font.drawString("get start !", 230, 300); // 한글이 안띄어져서 get start ! 로 버튼 위에 폰트를 띄운다.
    }
    else if (in_play && game_over) {
        ofClear(255);

        ofSetColor(ofColor::black);
        font.drawString("Champion : ", 100, 70);
        champion->DrawImage(LEFT_IMAGE); // 좌측에 우승한 교수님/연구실 사진을 띄운다

        ofSetColor(248, 53, 76);
        ofDrawRectangle(700, 150, 400, 60); // 재시작 버튼(직사각형)을 띄운다
        ofSetColor(ofColor::black);
        font.drawString("Restart ? ", 740, 190);

        ofSetColor(76, 53, 248);
        ofDrawRectangle(700, 300, 400, 60); //프로그램 종료 버튼(직사각형)을 띄운다.
        ofSetColor(ofColor::black);
        font.drawString("Good bye", 740, 340);
		ofSetColor(ofColor::white);
		be_happy.resize(400, 80);
		be_happy.draw(700, 450);
    }
    else { // 게임 진행중 상태
        ofClear(255);
        ofSetColor(0);
        ofDrawLine(640, 0, 640, 720); // 좌측/우측 구분 라인
        char buf[64];

        switch (stage)
        {
        case ROUND_OF_16: //  16강, 조별리그까지 만들기에 연구실의 수가 부족하다.
            /* code */
            snprintf(buf, sizeof buf, "ROUND OF 16 %d/%d", (pos - 16) / 2 + 1, 8);
            break;
        case QUATER_FINAL: // 8강
            snprintf(buf, sizeof buf, "QUATER_FINAL %d/%d", (pos - 8) / 2 + 1, 4);
            break;
        case SEMI_FINAL:// 4강
            snprintf(buf, sizeof buf, "QUATER_FINAL %d/%d", (pos - 4) / 2 + 1, 2);
            break;
        case FINAL: // 결승전
            snprintf(buf, sizeof buf, "FINAL");
            break;
        default:
            break;
        }
        font.drawString(string(buf), 560, 30);
        ofSetColor(255);
        match_up[pos]->DrawImage(LEFT_IMAGE); // 왼쪽에 사진 띄우기
        match_up[pos + 1]->DrawImage(RIGHT_IMAGE); // 오른쪽에 사진 띄우기
    }

}
bool ofApp::Check(int ny, int nx) {
	if (ny >= 0 && ny < HEIGHT && nx >= 0 && nx < WIDTH) {
		return true;
	}
	return false;
}

//--------------------------------------------------------------x
void ofApp::keyPressed(int key) {
	int dir = -1;
	
	if (play_maze == true) {
		switch (key)
		{
		case OF_KEY_DOWN:
			dir = DOWN;
			break;
		case OF_KEY_RIGHT:
			dir = RIGHT;
			break;
		case OF_KEY_LEFT:
			dir = LEFT;
			break;
		case OF_KEY_UP:
			dir = UP;
			break;
		default:
			return;
		}
	}
	if (me.x == dest.x&&me.y == dest.y) { // 최종 목적지를 찾은 경우
		ofSetColor(0);
		play_maze = false;
		return;
	}

	int nx = me.x + dx[dir];
	int ny = me.y + dy[dir];

	if (Check(ny,nx)&&maze[me.y][me.x].direction[dir]==true) { // 이동가능한지 체크한뒤 위치 갱신
		me.x = nx;
		me.y = ny;
	}

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) { // 랭크 보기, 미로찾기 모드에서 화면을 누르면 돌아갈 수 있도록
	if (see_rank == true) {
		see_rank = false;
		return;
	}
	if (play_maze == true) {
		play_maze = false;

		return;
	}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
	/*
		첫 화면이다
	*/
    if (x >= 264 && x <= 1050 && y >= 540 && y <= 650 && in_play == false) { // get start! 직사각형의 범위다.

		//ShellExecute(0, 0, L"http://www.google.com", 0, 0, SW_SHOW);
        InitWorldCup(); // InitWorldcup으로 파일을 읽어 사진/텍스트를 메모리로 탑재한다
        in_play = true;

        return;
    }
	/*
		우승자 화면이다.
	*/
    if (in_play == true && game_over == true) {  // restart 직사각형의 범위다. 누르면 첫화면으로 돌아간다.
        if (y >= 150 && y <= 210 && x >= 702 && x <= 1098) {
			for (int i = 16; i < 32; i++) {
				delete match_up[i];  // 다시 시작하므로 메모리 할당 해제를 index 16부터 해준다. 어차피 1~15는 16~31의 값중 하나이다.
			}
            in_play = false;
            game_over = false;
        }

        if (y >= 300 && y <= 360 && x >= 702 && x <= 1098) { // GOODBYE 직사각형 범위다. 누르면 프로그램이 종료한다.
            // goodbye
            OF_EXIT_APP(0);
        }
		
		if (y > 450 && y <= 530 && x >= 700 && x <= 1100) {
			ShellExecute(0, 0, L"https://cs.sogang.ac.kr/cs/cs04_1.html", 0, 0, SW_SHOW); // 진학하기 버튼
			return;
		}

        return;
    }

	/*
		플레이 중 화면이다.
	*/
    if (in_play == true) { 
        int winner = (x / 640) + pos;
		/*
			match up 배열 index
			16강전 : 16~31
			8강전 : 8~15
			4강전 : 4~7
			결승전 : 2~3
			우승자 : 1
			을 사용한다.
			승자의 현재 index에서 2를 나눈다.
		*/
        match_up[winner / 2] = match_up[winner];
        pos += 2; // 예를들여 16강의 첫 경기의 match up 배열 index 16~17이고, 다음 경기는 18~19로 현재 포지션에서 2를 더해주되


		// 각 스테이지에서 끝나는 index position이 있다. 16강전이면 32, 8강전이면 16이다.
		// stage를 바꾸고 position을 다음 라운드의 첫 인덱스로 바꿔준다.
        switch (stage)
        {
        case FINAL:
            if (pos == 4) {
                champion = match_up[winner];
				AppendWinner();
                game_over = true;
            }
            break;

        case ROUND_OF_16:
            /* code */
            if (pos == 32) {
                stage = QUATER_FINAL;
                pos = 8;
            }
            break;
        case QUATER_FINAL:
            if (pos == 16) {
                stage = SEMI_FINAL;
                pos = 4;
            }
            break;
        case SEMI_FINAL:
            if (pos == 8) {
                stage = FINAL;
                pos = 2;
            }
            break;

        default:
            break;
        }
    }
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



/*
	InitWorldCup함수는 professor list를 읽고 해당 리스트에 존재하는 교수님/연구실의 이미지 파일을 로드한다.
*/
void ofApp::InitWorldCup() {
    ofFile file;
    file.open(ofToDataPath("professor.list"), ofFile::ReadOnly, false);
    ofBuffer buff = file.readToBuffer();
    int index = 16;
	// 해당 반복문에서는 professor list에 나열된 순서대로 match_up 배열에 16~31까지 Professor 클래스로 추상화된 것으로 저장한다.
	// 시간복잡도는 O(# of Professor)
    for (ofBuffer::Line it = buff.getLines().begin(), end = buff.getLines().end(); it != end; ++it) {
        string line = *it;
        vector<string> words = ofSplitString(line, " ");
        match_up[index++] = new Professor(words[0].c_str(), words[1].c_str(), &font);  // 공간복잡도는 O(# of Professor)
    }

	// 여기 반복문에서는, 매번 professor list대로만 대진이 이루어지면 재미가 없으므로 랜덤으로 섞어준다.
	srand(time(0));
	for (int i = 16; i < 32; i++) {
		int randidx = rand() % 16 + 16;
		Professor* tmp = match_up[randidx];
		match_up[randidx] = match_up[i];
		match_up[i] = tmp;
	}

    stage = ROUND_OF_16; // 16강전으로 초기화
    pos = 16; // 16강전 시작 index는 16

}


void ofApp::appMenuFunction(string title, bool bChecked) {

	ofFileDialogResult result;
	string filePath;
	size_t pos;

	//
	// File menu
	//
	if (title == "심심한데 미로찾기 하기") {
		InitMaze();
		play_maze = true;
	}
	if (title == "가장 인기있는 연구실") {
		see_rank = true;
	}

	

} // end appMenuFunction


static bool rank_cmp(const struct list_elem* a, const struct list_elem *b) {
	RankElem* ra = list_entry(a, struct RankElem, elem);
	RankElem* rb = list_entry(b, struct RankElem, elem);
	if (ra->point_ > rb->point_) {
		return false;
	}
	return true;
}
void ofApp::ReadRankFile() {
	RankElem* r_elem;
	ofFile file;
	file.open(ofToDataPath("rank.txt"), ofFile::ReadOnly, false);
	ofBuffer buff = file.readToBuffer();

	// 디스크 랭크 파일 읽어서 메모리에 순서대로 정렬하기
	for (ofBuffer::Line it = buff.getLines().begin(), end = buff.getLines().end(); it != end; ++it) {
		string line = *it;
		vector<string> words = ofSplitString(line, " ");
		r_elem = new RankElem(words[0], atoi(words[1].c_str() ));
		list_insert_ordered(&rank_list, &r_elem->elem, rank_cmp);
	}
}

void ofApp::AppendWinner() { // 새로운 champion 업데이트
	RankElem* r_elem;
	struct list_elem*iter;
	for (iter = list_begin(&rank_list); iter != list_end(&rank_list); iter = list_next(iter)) {
		r_elem = list_entry(iter, struct RankElem, elem);
		if (r_elem->name_ == champion->GetName()) { // 만약 현재 우승자 리스트에 champion이 있는지 확인
			r_elem->point_++;
			goto sync; // 있다면 sync절로 이동
		}
	}
	r_elem = new RankElem(champion->GetName(), 1); // 만약 새로운 챔피언이라면 리스트에 추가
	list_push_back(&rank_list, &r_elem->elem);
sync:
	SyncRankFIle(); // 디스크 동기화함수
}

void ofApp::SyncRankFIle() {
	RankElem* r_elem;
	struct list_elem* iter;
	char buf[4096]; // 최대 4KB까지만 저장할 수 있음
	memset(buf, 0, sizeof(buf));
	for (iter = list_begin(&rank_list); iter != list_end(&rank_list); iter = list_next(iter)) {
		r_elem = list_entry(iter, struct RankElem, elem);
		//cout << r_elem->name_ << " " << r_elem->point_ << "\n";

		char tmp[128]; memset(tmp, 0, sizeof(tmp));
		sprintf(tmp, "%s %d\n", r_elem->name_.c_str(), r_elem->point_);
		strcat(buf, tmp);
	}
	FILE* fp = fopen("data/rank.txt", "w"); // 파일을 열고
	fprintf(fp, buf); // 버퍼의 내용을 써조고
	fflush(fp); // fflush
	fclose(fp); // 파일 닫기
}

// 랭크를 화면에 순서대로 보여주기
void ofApp::ShowRank() {
	ofClear(255);
	ofSetColor(0);
	struct list_elem *iter;
	struct RankElem* r_elem;
	float xpos = 500;
	float ypos = 50;
	int i = 1;
	for (iter = list_begin(&rank_list); iter != list_end(&rank_list); iter = list_next(iter)) {
		r_elem = list_entry(iter, struct RankElem, elem);
		char tmp[128];
		sprintf(tmp, "%d. %s %d", i,r_elem->name_.c_str(), r_elem->point_);
		font.drawString(tmp,xpos,ypos); 
		i++;
		ypos += 50;
	}
}

/*
미로 생성 함수들
*/
void ofApp::GiveGroupNumber(int row) {
	for (int i = 0; i < WIDTH; i++) {
		if (maze[row][i].group == NOGROUP) {
			maze[row][i].group = group_number++;
		}
	}
}

// 아래를 뚫는 함수
void ofApp::DigDownward(int row) {

	for (int i = 0; i < WIDTH - 1; i++) {
		int cur_g = maze[row][i].group;
		maze[row][i].direction[DOWN] = true;
		maze[row + 1][i].group = cur_g;
		while (i<WIDTH-1&&maze[row][i + 1].group == cur_g) {
			if (TOSS) {
				maze[row][i + 1].direction[DOWN] = true;
				maze[row + 1][i + 1].group = maze[row + 1][i].group;
			}
			i++;
		}
	}

	if (maze[row][WIDTH-1].group != maze[row][WIDTH-2].group) { // 마지막 칸이 옆과 연결되어있지 않으면 아래로 꼭 뚫어줘야함
		maze[row + 1][WIDTH-1].group = maze[row][WIDTH-1].group;
		maze[row][WIDTH-1].direction[DOWN] = true;
	}
}



void ofApp::InitMaze() {
	int i, j;
	me.x = 0;
	me.y = 0;
	for (i = 0; i < HEIGHT; i++) {
		for (j = 0; j < WIDTH; j++) {
			maze[i][j].x = j; maze[i][j].y = i;
			maze[i][j].group = NOGROUP;
			maze[i][j].visited = false;
			for (int k = 0; k < 4; k++) {
				maze[i][j].direction[k] = false;
			}
		}
	}
	group_number = 0;
	srand(time(NULL));

	// 첫줄 초기화
	GiveGroupNumber(0);
	for (i = 0; i < WIDTH - 1; i++) {
		if (TOSS) {
			maze[0][i].direction[RIGHT] = true;
			maze[0][i + 1].group = maze[0][i].group;
		}
	}
	DigDownward(0);


	// 2~H-1번째 줄 초기화
	for (i = 1; i < HEIGHT - 1; i++) {
		GiveGroupNumber(i);
		for (j = 0; j < WIDTH - 1; j++) {
			if (maze[i][j].group != maze[i][j + 1].group) {
				if (TOSS) {
					maze[i][j].direction[RIGHT] = true;
					maze[i][j + 1].group = maze[i][j].group;
				}
			}
		}
		DigDownward(i);
	}

	// 마지막 줄 초기화
	GiveGroupNumber(i);
	for (j = 0; j < WIDTH - 1; j++) {
		if (maze[i][j + 1].group != maze[i][j].group && maze[i-1][j].group!=maze[i][j].group ) { // 옆과 다른 그룹이면, 연결되어있지 않으면 옆 뚫기

		}

	}
	for (j = 1; j < WIDTH - 1; j++) {
		int g = maze[i][j].group;
		if (g != maze[i][j - 1].group&&g != maze[i - 1][j].group &&g != maze[i][j + 1].group) {
			if (false) {
				maze[i - 1][j].direction[DOWN] = true;
				maze[i][j].group = maze[i - 1][j].group;
			}
			else {
				maze[i][j].direction[RIGHT] = true;
				maze[i][j + 1].group = g;
			}

		}
	}
	

	// 위에서는 오른쪽/아래쪽 방향 결정을 통해 미로 그리기를 했는데, 직접 유저가 포인터를 움직이기 위해서는 왼쪽/위쪽 direction 정보도 각 cell에 줘야한다
	for (i = 0; i < HEIGHT ; i++) {
		for (j = 0; j < WIDTH; j++) {
			if (maze[i][j].direction[RIGHT] == true) {
				maze[i][j + 1].direction[LEFT] = true;
			}
			if (maze[i][j].direction[DOWN] == true) {
				maze[i + 1][j].direction[UP] = true;
			}
		}
	}
	//목적지 초가화 함수
	InitDest();
}


// cell들을 그리고, 현재 위치/ 목적지 위치를 그린다
void ofApp::DrawMaze() {
	ofClear(255);
	ofSetColor(0, 0, 255);
	ofDrawLine(0, 0, WIDTH * LINELENGTH, 0);
	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH; j++) {
			DrawCell(maze[i][j], i, j);
		}
	}
	ofSetColor(255, 0, 0);
	ofDrawCircle(me.x*LINELENGTH + LINELENGTH/2, me.y*LINELENGTH+LINELENGTH/2 , 5);
	ofSetColor(0, 255, 0);
	ofDrawCircle(dest.x*LINELENGTH + LINELENGTH / 2, dest.y*LINELENGTH + LINELENGTH / 2, 5);
}

void ofApp::DrawCell(Cell& cell, float y, float x) {

	float pos_y = y * LINELENGTH;
	float pos_x = x * LINELENGTH;
	float center_x = pos_x + LINELENGTH / 2;
	float center_y = pos_y + LINELENGTH / 2;

	// left, up, down, right

	if (cell.direction[RIGHT] == false) { //3 
		ofDrawLine(pos_x + LINELENGTH, pos_y, pos_x + LINELENGTH, pos_y + LINELENGTH);
	}
	if (cell.direction[DOWN] == false) { //2
		ofDrawLine(pos_x, pos_y + LINELENGTH, pos_x + LINELENGTH, pos_y + LINELENGTH);
	}

}


// 디버깅을 위한 콘솔 미로 출력함수
void ofApp::PrintMaze(void) {
	int i, j;
	for (i = 0; i < WIDTH; i++) {
		//fprintf(fp, "+-");
		printf("+-");
	}
	//fprintf(fp, "+\n");
	printf("+\n");
	for (i = 0; i < HEIGHT; i++) {
		//fprintf(fp, "|");
		printf("|");
		for (j = 0; j < WIDTH; j++) {
			if (maze[i][j].direction[RIGHT] == true) {
				//printf("%d ",maze[i][j].group);
				//fprintf(fp, "  ");
				printf("  ");
			}
			else {
				//printf("%d|",maze[i][j].group);
				//fprintf(fp, " |");
				printf(" |");
			}
		}
		//fprintf(fp, "\n");
		printf("\n");

		//fprintf(fp, "+");
		printf("+");
		for (j = 0; j < WIDTH; j++) {
			if (maze[i][j].direction[DOWN] == true) {
				//fprintf(fp, " +");
				printf(" +");
			}
			else {
				//fprintf(fp, "-+");
				printf("-+");
			}
		}
		//fprintf(fp, "\n");
		printf("\n");
	}
}


/* 
과제의 미로찾기에서는 항상 오른쪽/아래쪽 제일 아래칸을 목적지로 설정했다.
 그러나 최종프로젝트의 미로찾기에서의 좀더 복잡성과 랜덤성을 증가시키기 위해
 완전미로는 아니지만, 미로의 가로 1/2 , 세로 1/2을 넘는 지점 중 도달할 수 있는 지점을 목적지로 정한다.
 이때 도달 가능성은 BFS를 통해 테스트한다.
 */
void ofApp::InitDest() {
	while (1) {
		int x = WIDTH / 2 + rand()%(WIDTH/2) ;
		int y = HEIGHT / 2 +rand()%(HEIGHT/2) ;
		if (BFS(y, x)) {
			dest.x = x;
			dest.y = y;
			return;
		}
	}
}

bool ofApp::BFS(int y, int x) {
	queue<pair<int,int>> q;
	
	q.push({ y,x });
	while (!q.empty()) {
		int cur_y = q.front().first;
		int cur_x = q.front().second;
		q.pop();
		maze[cur_y][cur_x].visited = true;
		if (cur_y == 0 && cur_x == 0) { // 최초 목적지에서 출발지점, 0,0에 도달할 수 있으면 true를 리턴한다
			return true;
		}
		for (int k = 0; k < 4; k++) {
			int ny = cur_y + dy[k];
			int nx = cur_x + dx[k];
			if (Check(ny, nx) && maze[cur_y][cur_x].direction[k]==true&&maze[ny][nx].visited==false ) {
				q.push({ ny,nx });
			}
		}
	}

	return false;
}