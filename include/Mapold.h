#ifndef MAP_H
#define MAP_H

#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <queue>
#include <fstream>
#include <cstring>
#include <math.h>
#include <SFML/Graphics.hpp>

#define MAX 0b01111111111111111111111111111111
#define CHANCE 3800
#define NUMBER_OF_STEPS 20

class MapPoint
{
private:
    int x;
    int y;
    bool crocodile;
    int value;
    int came_from[2];
public:
    MapPoint(){}
    ~MapPoint(){}
    void init(int xx, int yy, bool croc)
    {
        x = xx;
        y = yy;
        crocodile = croc;
        value = MAX;
    }
    void setValue(int vvalue) { value = vvalue; }
    void setCameFrom(int x, int y) { came_from[0] = x; came_from[1] = y; }
    int getCameFromX() { return came_from[0]; }
    int getCameFromY() { return came_from[1]; }
    int getValue() { return value; }
    int getX() { return x; }
    int getY() { return y; }
    bool getCroc() { return crocodile; }
};



class Map
{
private:
    char* state;
    int n;                                      //szerokosc rzeki (i dlugosc)
    MapPoint **matrix;                          //mapa
    int nSur, yS;                               //ilosc surykatek, wspolrzedna y'kowa kazdej surykatki
    bool generated;                             //mapa generowana czy wczytywana z pliku?

public:
    void allocMap();
    void deleteMap();
    void generateMap(int &range);
    void makeCrocodile(int &length, int x, int y);
    void makeCrocodile(int &x1, int &y1, int &x2, int &y2);

};

#endif // MAP_H
