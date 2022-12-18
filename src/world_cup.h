#ifndef WORLD_CUP_H_
#define WORLD_CUP_H_

#include <string>
#include "ofMain.h"
#include <cstdio>


/*
교수님의 정보를 추상화한 클래스다.
*/
class Professor {
private:
    string image_path_; // 이미지 경로
    string name_; // 교수님 성함
    ofTrueTypeFont* font_; // ofTrueTypeFont 포인터를 를 ofApp에서 받아온다
    ofImage image_; // 교수님 이미지 파일을 객체로 저장한다.

public:
	/*
		생성자에서는 이미지를 로드해서 ofImage 객체로 저장한다.
	*/
    Professor(const char* name, const char* image_path, ofTrueTypeFont* font) :name_(name), image_path_(image_path), font_(font) {
        string image_professor = "image/professor/";
        image_professor.append(image_path);

        if (image_.load(image_professor.c_str())) {

        }
    }
	string GetName() {
		return name_;
	}
    void DrawImage(bool left);
};






#endif#pragma once
