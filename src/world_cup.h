#ifndef WORLD_CUP_H_
#define WORLD_CUP_H_

#include <string>
#include "ofMain.h"
#include <cstdio>

class Professor {
private:
    string image_path_;
    string name_;
    ofTrueTypeFont* font_;
    ofImage image_;

public:
    Professor(const char* name, const char* image_path, ofTrueTypeFont* font) :name_(name), image_path_(image_path), font_(font) {
        string image_professor = "image/professor/";
        image_professor.append(image_path);

        if (image_.load(image_professor.c_str())) {
            cout << image_professor << " load success\n";
        }
        else {
            cout << image_professor << "load fail\n";
        }
    }
    void DrawImage(bool left);
};






#endif#pragma once
