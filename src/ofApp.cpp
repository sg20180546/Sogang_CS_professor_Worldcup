#include "ofApp.h"
#include <string>
int xPos;
//--------------------------------------------------------------
ofImage sogang_logo;
ofImage title;

bool in_play;
Professor* match_up[32];
#define LEFT true
#define RIGHT false

void ofApp::setup() {

    ofBackground(ofColor::white); // black background
    sogang_logo.load("image/download.png");
    title.load("image/title.png");
    font.load(OF_TTF_SANS, 20);
    ofSetBackgroundAuto(false);
    in_play = false;
    stage = NOT_START;
}
void ofApp::update() {
    // xPos += 2;
    // if(ofGetWidth()<xPos){ // if horizontal position is off the screen (width)
    // xPos = 5; // reset horizontal position
    // }
}
void ofApp::draw() {

    if (!in_play) {
        ofClear(255);
        ofSetColor(255);

        title.resize(560, 100);
        title.draw(350, 150);
        sogang_logo.draw(512, 300);
        ofScale(2);
        ofSetColor(248, 53, 76);
        ofDrawRectangle(130, 270, 400, 60);
        ofSetColor(ofColor::black);
        // font.drawString("시작하기",200,300);
        font.drawString("get start !", 230, 300);
    }
    else if (in_play && game_over) {
        ofClear(255);

        ofSetColor(ofColor::black);
        font.drawString("Champion : ", 100, 70);
        champion->DrawImage(LEFT);

        ofSetColor(248, 53, 76);
        ofDrawRectangle(700, 270, 400, 60);
        ofSetColor(ofColor::black);
        font.drawString("Restart ? ", 740, 320);

        ofSetColor(76, 53, 248);
        ofDrawRectangle(700, 400, 400, 60);
        ofSetColor(ofColor::black);
        font.drawString("Good bye", 740, 450);
    }
    else {
        ofClear(255);
        ofSetColor(0);
        ofDrawLine(640, 0, 640, 720);
        char buf[64];
        // snprintf(buf,)

        switch (stage)
        {
        case ROUND_OF_16:
            /* code */
            snprintf(buf, sizeof buf, "ROUND OF 16 %d/%d", (pos - 16) / 2 + 1, 8);
            break;
        case QUATER_FINAL:
            snprintf(buf, sizeof buf, "QUATER_FINAL %d/%d", (pos - 8) / 2 + 1, 4);
            break;
        case SEMI_FINAL:
            snprintf(buf, sizeof buf, "QUATER_FINAL %d/%d", (pos - 4) / 2 + 1, 2);
            break;
        case FINAL:
            snprintf(buf, sizeof buf, "FINAL");
            break;
        default:
            break;
        }
        font.drawString(string(buf), 560, 30);
        ofSetColor(255);
        match_up[pos]->DrawImage(LEFT);
        match_up[pos + 1]->DrawImage(RIGHT);
    }

}

//--------------------------------------------------------------x
void ofApp::keyPressed(int key) {
    // if (key == 'v') {
    //     // HACK: only needed on windows, when using ofSetAutoBackground(false)
    //     glReadBuffer(GL_FRONT);
    //     ofSaveScreen("savedScreenshot_"+ofGetTimestampString()+".png");
    // }
    // if( key == 'q'){
    //     if( !load_flag ) return;

    //     // Reset flags
    //     draw_flag = 0;
    //     waterfall_start_flag = 0;

    //     // Free the Dynamic_allocation entities.
    //     if( lineseg != NULL){
    //         free(lineseg);
    //         lineseg = NULL;
    //     }
    //     cout << "Memory for line segement has been freed." << endl;
    //     if( dot != NULL){
    //         free(dot);
    //         dot = NULL;
    //     }
    //     cout << "Memory for dot has been freed." << endl;

    //     _Exit(0);
    // }
    // if (key == 'd'){
    //     if( !load_flag ) return;
    //     draw_flag = 1;
    //     target_dot.x = dot[selection_dot].x1;
    //     target_dot.y = dot[selection_dot].y1;
    // }
    // if (key == 's'){
    //     if( load_flag ){
    //         resetWater();
    //         for(unsigned int i = 0; i < wl.size(); i++){
    //             if( wl[i].calc_complete == 0)
    //                 wl[i].calculate_path(lineseg, num_of_line);
    //         }
    //         if(draw_flag){
    //             waterfall_start_flag = 1;
    //         }
    //     }
    // }
    // if( key == 'e'){
    //     if(draw_flag)
    //         waterfall_start_flag = 0;
    // }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {
    // printf("moving\n");
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
    // printf("pressing %d %d\n",y,x);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
    if (x >= 264 && x <= 1050 && y >= 540 && y <= 650 && in_play == false) {
        // printf("hello\n");
        InitWorldCup();
        in_play = true;

        return;
    }
    if (in_play == true && game_over == true) {
        if (y >= 271 && y <= 329 && x >= 702 && x <= 1098) {
            in_play = false;
            game_over = false;
        }
        if (y >= 404 && y <= 459 && x >= 702 && x <= 1098) {
            // goodbye
            OF_EXIT_APP(0);
        }
        return;
    }

    if (in_play == true) {
        // if(x<630||x>650){
        //     if(x<630){
        //         cout<<"left won\n";
        //     }else{
        //         cout<<"right woen\n";
        //     }
        int winner = (x / 640) + pos;
        // }

        match_up[winner / 2] = match_up[winner];
        pos += 2;

        switch (stage)
        {
        case FINAL:
            if (pos == 4) {
                champion = match_up[winner];
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
    // printf("releasing %d %d \n",x,y);
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


void ofApp::InitWorldCup() {
    ofFile file;

    file.open(ofToDataPath("professor.list"), ofFile::ReadOnly, false);
    ofBuffer buff = file.readToBuffer();
    int index = 16;
    for (ofBuffer::Line it = buff.getLines().begin(), end = buff.getLines().end(); it != end; ++it) {
        string line = *it;
        cout << line << "\n";
        vector<string> words = ofSplitString(line, " ");
        cout << words[0] << "\n";
        match_up[index++] = new Professor(words[0].c_str(), words[1].c_str(), &font);
    }
    stage = ROUND_OF_16;
    pos = 16;

}
