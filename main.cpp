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

//TODO: Crashes if y-axis is of limit

#include "csv.hpp"
#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include <vector>
#include <string>
#include <stdexcept>

#define L 50
#define PAD 80
#define W 1200
#define H 800
#define SIDEBAR 400
#define MAX_INPUT 3

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
    const char* err = "Something is wrong with CSV file";
    char x_axis[MAX_INPUT+1] = "0\0";
    char y_axis[MAX_INPUT+1] = "1\0";
    int xcounter = 0;
    int ycounter = 0;
    bool xcol = false;
    bool ycol = false;
    vector<float> x;
    vector<float> y;
    int fontsize = 20;
    int i;
    bool loaded = false;
    bool input = false;

    CSVFormat format;
    format.delimiter(',');
    
    InitWindow(W, H, "DataViz");

    while (!WindowShouldClose()){
        if (IsFileDropped()){
            FilePathList files = LoadDroppedFiles();
            path = files.paths[0];

            if (!path.empty()){
                if (loaded){
                    x.clear();
                    y.clear();
                }
                CSVReader reader(path, format);
                loaded = true;
                i = 0;
                for (CSVRow& row: reader){
                    i = 0;
                    for (CSVField& field: row){
                        int xu = stoi((string)x_axis);
                        int yu = stoi((string)y_axis);
                        try{
                            if (i == xu && i == yu){
                                x.push_back(stof(field.get<>()));
                                y.push_back(stof(field.get<>()));
                            }
                            else{
                                if (i == xu){
                                    x.push_back(stof(field.get<>()));
                                }
                                if (i == yu){
                                    y.push_back(stof(field.get<>()));
                                }
                            }
                        }
                        catch (const exception& e){
                        }
                        i++;
                    }
                }
            }
            UnloadDroppedFiles(files);
        }

        if (input){
            if (loaded){
                x.clear();
                y.clear();
            }
            CSVReader reader(path, format);
            loaded = true;
            i = 0;
            for (CSVRow& row: reader){
                i = 0;
                for (CSVField& field: row){
                    int xu = stoi((string)x_axis);
                    int yu = stoi((string)y_axis);
                    try{
                        if (i == xu && i == yu){
                            x.push_back(stof(field.get<>()));
                            y.push_back(stof(field.get<>()));
                        }
                        else{
                            if (i == xu){
                                x.push_back(stof(field.get<>()));
                            }
                            if (i == yu){
                                y.push_back(stof(field.get<>()));
                            }
                        }
                    }
                    catch (const exception& e){
                    }
                    i++;
                }
            }
            input = false;
        }
            
        

        if(loaded){
            if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){L, 2*L, 100, 50})){
                xcol = true;
            }
            else if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){L, 5*L, 100, 50})){
                ycol = true;
            }
            else{
                xcol = false;
                ycol = false;
            }

            if (xcol){
                SetMouseCursor(MOUSE_CURSOR_IBEAM);

                int key = GetCharPressed();

                while (key > 0){
                    if ((key >= 48) && (key <= 57) && (xcounter < MAX_INPUT)){
                        x_axis[xcounter] = (char)key;
                        x_axis[xcounter+1] = '\0';
                        xcounter++;
                    }
                    key = GetCharPressed();
                }

                if (IsKeyPressed(KEY_BACKSPACE)){
                    xcounter--;
                    if (xcounter < 0){
                        xcounter = 0;
                    }
                    x_axis[xcounter] = '\0';
                }
                if (IsKeyPressed(KEY_ENTER)){
                    input = true;
                }
            }
            else if (ycol){
                SetMouseCursor(MOUSE_CURSOR_IBEAM);

                int ykey = GetCharPressed();

                while (ykey > 0){
                    if ((ykey >= 48) && (ykey <= 57) && (ycounter < MAX_INPUT)){
                        y_axis[ycounter] = (char)ykey;
                        y_axis[ycounter+1] = '\0';
                        ycounter++;
                    }
                    ykey = GetCharPressed();
                }

                if (IsKeyPressed(KEY_BACKSPACE)){
                    ycounter--;
                    if (ycounter < 0){
                        ycounter = 0;
                    }
                    y_axis[ycounter] = '\0';
                }

                if (IsKeyPressed(KEY_ENTER)){
                    input = true;        
                }
            }
            else{
                SetMouseCursor(MOUSE_CURSOR_DEFAULT);
            }
        }
    

        BeginDrawing();
        ClearBackground(GetColor(0x181818AA));
        if (loaded){
            DrawText("Column index of X axis",L, 2*L-30, fontsize, WHITE);
            DrawRectangle(L, 2*L, 100, 50, LIGHTGRAY);
            DrawText(x_axis, L+5, 2*L+8, 40, MAROON);

            DrawText("Column index of Y axis", L, 5*L-30, fontsize, WHITE);
            DrawRectangle(L, 5*L, 100, 50, LIGHTGRAY);
            DrawText(y_axis, L+5, 5*L+8, 40, MAROON);

            DrawLine(SIDEBAR, L, SIDEBAR, H-L, WHITE);
            DrawLine(SIDEBAR, H-L, W-L, H-L, WHITE);
            for (int i = 0; i < x.size(); i++){
                int yc = (int)reverse(Remap(y[i], 0, y[max_vector(y)], PAD, H-PAD));
                int xc = (int)Remap(x[i], 0, x[max_vector(x)], SIDEBAR, W-PAD);
                DrawCircle(xc, yc, 5, RED);
            }
        }
        else {
            DrawText(text, (W/2)-(sizeof(text)/2*fontsize), H/2, fontsize, WHITE);
        }
        EndDrawing();
    }
}
