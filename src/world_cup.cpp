#include "world_cup.h"
ofTrueTypeFont font;
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