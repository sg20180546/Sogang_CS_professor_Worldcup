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

    ofBackground(ofColor::white); // �Ͼ�� ȭ������
    sogang_logo.load("image/download.png"); // �������б� �ΰ� ����
    title.load("image/title.png"); // ���������ӿ�ũ������ �ѱ� ��Ʈ ���°� �ȵǼ� �������б� ��ǻ�Ͱ��а� ���п� ... ������ ����
	be_happy.load("image/be_happy.png"); // �ູ������ ���� ��ư
	font.load(OF_TTF_SANS, 20);
    ofSetBackgroundAuto(false);
    in_play = false;
    stage = NOT_START; // state : ���� �������� ���� ���´�.

	hWnd = WindowFromDC(wglGetCurrentDC());

	menu = new ofxWinMenu(this, hWnd);
	menu->CreateMenuFunction(&ofApp::appMenuFunction);
	HMENU hMenu = menu->CreateWindowMenu();

	HMENU hPopup = menu->AddPopupMenu(hMenu, "File");
	menu->AddPopupItem(hPopup, "�ɽ��ѵ� �̷�ã�� �ϱ�", false, false); // Not checked and not auto-checked
	menu->AddPopupItem(hPopup, "���� �α��ִ� ������", false, false); // Not checked and not auto-checked
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


    if (!in_play) { // ù ȭ��
        ofClear(255);
        ofSetColor(255);

        title.resize(560, 100);
        title.draw(350, 150);
        sogang_logo.draw(512, 300);
        ofScale(2);
        ofSetColor(248, 53, 76);
        ofDrawRectangle(130, 270, 400, 60); // ������ �簢��, Ŭ�������� event�� �Ʒ����� event�� listen �Ҽ� �ְ� �����Ұ��̴�.
        ofSetColor(ofColor::black);

        font.drawString("get start !", 230, 300); // �ѱ��� �ȶ������ get start ! �� ��ư ���� ��Ʈ�� ����.
    }
    else if (in_play && game_over) {
        ofClear(255);

        ofSetColor(ofColor::black);
        font.drawString("Champion : ", 100, 70);
        champion->DrawImage(LEFT_IMAGE); // ������ ����� ������/������ ������ ����

        ofSetColor(248, 53, 76);
        ofDrawRectangle(700, 150, 400, 60); // ����� ��ư(���簢��)�� ����
        ofSetColor(ofColor::black);
        font.drawString("Restart ? ", 740, 190);

        ofSetColor(76, 53, 248);
        ofDrawRectangle(700, 300, 400, 60); //���α׷� ���� ��ư(���簢��)�� ����.
        ofSetColor(ofColor::black);
        font.drawString("Good bye", 740, 340);
		ofSetColor(ofColor::white);
		be_happy.resize(400, 80);
		be_happy.draw(700, 450);
    }
    else { // ���� ������ ����
        ofClear(255);
        ofSetColor(0);
        ofDrawLine(640, 0, 640, 720); // ����/���� ���� ����
        char buf[64];

        switch (stage)
        {
        case ROUND_OF_16: //  16��, �������ױ��� ����⿡ �������� ���� �����ϴ�.
            /* code */
            snprintf(buf, sizeof buf, "ROUND OF 16 %d/%d", (pos - 16) / 2 + 1, 8);
            break;
        case QUATER_FINAL: // 8��
            snprintf(buf, sizeof buf, "QUATER_FINAL %d/%d", (pos - 8) / 2 + 1, 4);
            break;
        case SEMI_FINAL:// 4��
            snprintf(buf, sizeof buf, "QUATER_FINAL %d/%d", (pos - 4) / 2 + 1, 2);
            break;
        case FINAL: // �����
            snprintf(buf, sizeof buf, "FINAL");
            break;
        default:
            break;
        }
        font.drawString(string(buf), 560, 30);
        ofSetColor(255);
        match_up[pos]->DrawImage(LEFT_IMAGE); // ���ʿ� ���� ����
        match_up[pos + 1]->DrawImage(RIGHT_IMAGE); // �����ʿ� ���� ����
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
	if (me.x == dest.x&&me.y == dest.y) { // ���� �������� ã�� ���
		ofSetColor(0);
		play_maze = false;
		return;
	}

	int nx = me.x + dx[dir];
	int ny = me.y + dy[dir];

	if (Check(ny,nx)&&maze[me.y][me.x].direction[dir]==true) { // �̵��������� üũ�ѵ� ��ġ ����
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
void ofApp::mousePressed(int x, int y, int button) { // ��ũ ����, �̷�ã�� ��忡�� ȭ���� ������ ���ư� �� �ֵ���
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
		ù ȭ���̴�
	*/
    if (x >= 264 && x <= 1050 && y >= 540 && y <= 650 && in_play == false) { // get start! ���簢���� ������.

		//ShellExecute(0, 0, L"http://www.google.com", 0, 0, SW_SHOW);
        InitWorldCup(); // InitWorldcup���� ������ �о� ����/�ؽ�Ʈ�� �޸𸮷� ž���Ѵ�
        in_play = true;

        return;
    }
	/*
		����� ȭ���̴�.
	*/
    if (in_play == true && game_over == true) {  // restart ���簢���� ������. ������ ùȭ������ ���ư���.
        if (y >= 150 && y <= 210 && x >= 702 && x <= 1098) {
			for (int i = 16; i < 32; i++) {
				delete match_up[i];  // �ٽ� �����ϹǷ� �޸� �Ҵ� ������ index 16���� ���ش�. ������ 1~15�� 16~31�� ���� �ϳ��̴�.
			}
            in_play = false;
            game_over = false;
        }

        if (y >= 300 && y <= 360 && x >= 702 && x <= 1098) { // GOODBYE ���簢�� ������. ������ ���α׷��� �����Ѵ�.
            // goodbye
            OF_EXIT_APP(0);
        }
		
		if (y > 450 && y <= 530 && x >= 700 && x <= 1100) {
			ShellExecute(0, 0, L"https://cs.sogang.ac.kr/cs/cs04_1.html", 0, 0, SW_SHOW); // �����ϱ� ��ư
			return;
		}

        return;
    }

	/*
		�÷��� �� ȭ���̴�.
	*/
    if (in_play == true) { 
        int winner = (x / 640) + pos;
		/*
			match up �迭 index
			16���� : 16~31
			8���� : 8~15
			4���� : 4~7
			����� : 2~3
			����� : 1
			�� ����Ѵ�.
			������ ���� index���� 2�� ������.
		*/
        match_up[winner / 2] = match_up[winner];
        pos += 2; // �����鿩 16���� ù ����� match up �迭 index 16~17�̰�, ���� ���� 18~19�� ���� �����ǿ��� 2�� �����ֵ�


		// �� ������������ ������ index position�� �ִ�. 16�����̸� 32, 8�����̸� 16�̴�.
		// stage�� �ٲٰ� position�� ���� ������ ù �ε����� �ٲ��ش�.
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
	InitWorldCup�Լ��� professor list�� �а� �ش� ����Ʈ�� �����ϴ� ������/�������� �̹��� ������ �ε��Ѵ�.
*/
void ofApp::InitWorldCup() {
    ofFile file;
    file.open(ofToDataPath("professor.list"), ofFile::ReadOnly, false);
    ofBuffer buff = file.readToBuffer();
    int index = 16;
	// �ش� �ݺ��������� professor list�� ������ ������� match_up �迭�� 16~31���� Professor Ŭ������ �߻�ȭ�� ������ �����Ѵ�.
	// �ð����⵵�� O(# of Professor)
    for (ofBuffer::Line it = buff.getLines().begin(), end = buff.getLines().end(); it != end; ++it) {
        string line = *it;
        vector<string> words = ofSplitString(line, " ");
        match_up[index++] = new Professor(words[0].c_str(), words[1].c_str(), &font);  // �������⵵�� O(# of Professor)
    }

	// ���� �ݺ���������, �Ź� professor list��θ� ������ �̷������ ��̰� �����Ƿ� �������� �����ش�.
	srand(time(0));
	for (int i = 16; i < 32; i++) {
		int randidx = rand() % 16 + 16;
		Professor* tmp = match_up[randidx];
		match_up[randidx] = match_up[i];
		match_up[i] = tmp;
	}

    stage = ROUND_OF_16; // 16�������� �ʱ�ȭ
    pos = 16; // 16���� ���� index�� 16

}


void ofApp::appMenuFunction(string title, bool bChecked) {

	ofFileDialogResult result;
	string filePath;
	size_t pos;

	//
	// File menu
	//
	if (title == "�ɽ��ѵ� �̷�ã�� �ϱ�") {
		InitMaze();
		play_maze = true;
	}
	if (title == "���� �α��ִ� ������") {
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

	// ��ũ ��ũ ���� �о �޸𸮿� ������� �����ϱ�
	for (ofBuffer::Line it = buff.getLines().begin(), end = buff.getLines().end(); it != end; ++it) {
		string line = *it;
		vector<string> words = ofSplitString(line, " ");
		r_elem = new RankElem(words[0], atoi(words[1].c_str() ));
		list_insert_ordered(&rank_list, &r_elem->elem, rank_cmp);
	}
}

void ofApp::AppendWinner() { // ���ο� champion ������Ʈ
	RankElem* r_elem;
	struct list_elem*iter;
	for (iter = list_begin(&rank_list); iter != list_end(&rank_list); iter = list_next(iter)) {
		r_elem = list_entry(iter, struct RankElem, elem);
		if (r_elem->name_ == champion->GetName()) { // ���� ���� ����� ����Ʈ�� champion�� �ִ��� Ȯ��
			r_elem->point_++;
			goto sync; // �ִٸ� sync���� �̵�
		}
	}
	r_elem = new RankElem(champion->GetName(), 1); // ���� ���ο� è�Ǿ��̶�� ����Ʈ�� �߰�
	list_push_back(&rank_list, &r_elem->elem);
sync:
	SyncRankFIle(); // ��ũ ����ȭ�Լ�
}

void ofApp::SyncRankFIle() {
	RankElem* r_elem;
	struct list_elem* iter;
	char buf[4096]; // �ִ� 4KB������ ������ �� ����
	memset(buf, 0, sizeof(buf));
	for (iter = list_begin(&rank_list); iter != list_end(&rank_list); iter = list_next(iter)) {
		r_elem = list_entry(iter, struct RankElem, elem);
		//cout << r_elem->name_ << " " << r_elem->point_ << "\n";

		char tmp[128]; memset(tmp, 0, sizeof(tmp));
		sprintf(tmp, "%s %d\n", r_elem->name_.c_str(), r_elem->point_);
		strcat(buf, tmp);
	}
	FILE* fp = fopen("data/rank.txt", "w"); // ������ ����
	fprintf(fp, buf); // ������ ������ ������
	fflush(fp); // fflush
	fclose(fp); // ���� �ݱ�
}

// ��ũ�� ȭ�鿡 ������� �����ֱ�
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
�̷� ���� �Լ���
*/
void ofApp::GiveGroupNumber(int row) {
	for (int i = 0; i < WIDTH; i++) {
		if (maze[row][i].group == NOGROUP) {
			maze[row][i].group = group_number++;
		}
	}
}

// �Ʒ��� �մ� �Լ�
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

	if (maze[row][WIDTH-1].group != maze[row][WIDTH-2].group) { // ������ ĭ�� ���� ����Ǿ����� ������ �Ʒ��� �� �վ������
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

	// ù�� �ʱ�ȭ
	GiveGroupNumber(0);
	for (i = 0; i < WIDTH - 1; i++) {
		if (TOSS) {
			maze[0][i].direction[RIGHT] = true;
			maze[0][i + 1].group = maze[0][i].group;
		}
	}
	DigDownward(0);


	// 2~H-1��° �� �ʱ�ȭ
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

	// ������ �� �ʱ�ȭ
	GiveGroupNumber(i);
	for (j = 0; j < WIDTH - 1; j++) {
		if (maze[i][j + 1].group != maze[i][j].group && maze[i-1][j].group!=maze[i][j].group ) { // ���� �ٸ� �׷��̸�, ����Ǿ����� ������ �� �ձ�

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
	

	// �������� ������/�Ʒ��� ���� ������ ���� �̷� �׸��⸦ �ߴµ�, ���� ������ �����͸� �����̱� ���ؼ��� ����/���� direction ������ �� cell�� ����Ѵ�
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
	//������ �ʰ�ȭ �Լ�
	InitDest();
}


// cell���� �׸���, ���� ��ġ/ ������ ��ġ�� �׸���
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


// ������� ���� �ܼ� �̷� ����Լ�
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
������ �̷�ã�⿡���� �׻� ������/�Ʒ��� ���� �Ʒ�ĭ�� �������� �����ߴ�.
 �׷��� ����������Ʈ�� �̷�ã�⿡���� ���� ���⼺�� �������� ������Ű�� ����
 �����̷δ� �ƴ�����, �̷��� ���� 1/2 , ���� 1/2�� �Ѵ� ���� �� ������ �� �ִ� ������ �������� ���Ѵ�.
 �̶� ���� ���ɼ��� BFS�� ���� �׽�Ʈ�Ѵ�.
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
		if (cur_y == 0 && cur_x == 0) { // ���� ���������� �������, 0,0�� ������ �� ������ true�� �����Ѵ�
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