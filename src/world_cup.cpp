#include "world_cup.h"
ofTrueTypeFont font;

/*
	DrawImage 함수에서는 좌측에 그릴지, 우측에 그릴지를 파라미터로 500*500의 크기로 이미지를 그리고 상단에 교수님의 영문 성함을 적는다.
*/
void Professor::DrawImage(bool left) {
    image_.resize(500, 500);
    vector<string> words = ofSplitString(image_path_, ".");
    if (left) {
        ofSetColor(ofColor::black);
        font_->drawString(words[0], 100, 100);
        ofSetColor(ofColor::white);
        image_.draw(100, 150);
    }
    else {
        ofSetColor(ofColor::black);
        font_->drawString(words[0], 750, 100);
        ofSetColor(ofColor::white);
        image_.draw(750, 150);
    }
}