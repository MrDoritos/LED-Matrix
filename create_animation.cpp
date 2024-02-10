#include <stdio.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include <cstring>

using namespace std;

bool frame[216];

void set_led(int x, int y, int z, bool value) {
    frame[(z * 36) + (y * 6) + x] = value;
}

bool get_led(int x, int y, int z) {
    return frame[(z * 36) + (y * 6) + x];
}

void print_frame() {
    for (int c = 0; c < 216; c++) {
        if (frame[c])
            cout << "#";
        else
            cout << "-";
    }

    
    cout << endl;

    cout << "50" << endl;
}

void xy_wave(int frame, int x, int y, int z) {
    float xval = (sinf((frame / 3.0f) + (x * (3.14 / 6.0f))) * 3) + 2;
    float yval = (sinf((frame / 3.0f) + (y * (3.14 / 6.0f))) * 3) + 2;
    float val = (xval + yval) * 0.5f;
    if (z > val && z < val + 1) {
        set_led(x,y,z,true);
    }
}

void random_plot(int frame, int x, int y, int z) {
    if (rand() % 2 == 0)
        set_led(x,y,z,true);
}

void rain(int frame, int x, int y, int z) {
    //spawn rain
    //50ms per frame, 20 frames per second
    if (z == 0 && (frame % 5) == 1) { //every second
        if (rand() % 20 == 0)
            set_led(x,y,z,true);
    }

    if (z > 0 && (frame % 15) == 0) {
        if (get_led(x,y,z-1) && !get_led(x,y,z)) {
            set_led(x,y,z-1, false);
            set_led(x,y,z, true);
        }            
    }

    //move rain down
}

int main() {
    for (int i = 0; i < 1500; i++) {    
        for (int x = 0; x < 6; x++) {
            for (int y = 0; y < 6; y++) {
                for (int z = 5; z > -1; z--) {
                    rain(i, x, y, z);
                }
            }
        }
    
        //memset(&frame[0], 0, 216);
        print_frame();
    }

    return 0;
}