/*
MIT License

Copyright (c) 2024 Luka Jovanovic

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#include "csv.hpp"
#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include <vector>

#define L 50
#define PAD 80
#define W 1000
#define H 800

using namespace csv;
using namespace std;

int max_vector(vector<float> x){
    float max = x[0];
    int index = 0;
    for (int i = 1; i < x.size(); i++){
        if (x[i] > max){
            max = x[i];
            index = i;
        }
    }

    return index;
}

float reverse(float x){
    return (float)fabsf(x-H);
}

int main(){

    string path;
    const char* text = "Drop CSV file in";
    vector<float> x;
    vector<float> y;
    int fontsize = 20;
    int i;
    bool loaded = false;

    CSVFormat format;
    format.delimiter(',');
    
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);   
    InitWindow(W, H, "DataViz");
    while (!WindowShouldClose()){
        if (IsFileDropped()){
            FilePathList files = LoadDroppedFiles();
            path = files.paths[0];

            if (!path.empty()){
                if (loaded){
                }
                CSVReader reader(path, format);
                loaded = true;
                i = 0;
                for (CSVRow& row: reader){
                    i = 0;
                    for (CSVField& field: row){
                        if (i == 0){
                            x.push_back(stof(field.get<>()));
                        }
                        else if (i == 1){
                            y.push_back(stof(field.get<>()));
                        }
                        i++;
                    }
                }
            }
            UnloadDroppedFiles(files);
        }
    

        BeginDrawing();
        ClearBackground(GetColor(0x181818AA));
        if (loaded){
            DrawLine(200, L, 200, H-L, WHITE);
            DrawLine(200, H-L, W-L, H-L, WHITE);
            for (int i = 0; i < x.size(); i++){
                int yc = (int)reverse(Remap(y[i], 0, y[max_vector(y)], PAD, H-PAD));
                int xc = (int)Remap(x[i], 0, x[max_vector(x)], 200, W-PAD);
                DrawCircle(xc, yc, 5, RED);
            }
        }
        else{
            DrawText(text, (W/2)-(sizeof(text)/2*fontsize), H/2, fontsize, WHITE);
        }
        EndDrawing();
    }
}
