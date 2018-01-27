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

#define MAX 0x7FFFFFFF
#define CHANCE 3800

class MapPoint
{
private:
    int x;
    int y;
    bool crocodile;
    int value;
    int came_from[2];
    bool road;
public:
    MapPoint(){}
    ~MapPoint(){}
    void init(int xx, int yy, bool croc)
    {
        x = xx;
        y = yy;
        crocodile = croc;
        value = MAX;
        came_from[0] = -1;
        came_from[1] = -1;
        road = false;
    }
    void setValue(int vvalue) { value = vvalue; }
    void setCameFrom(int xx, int yy) { came_from[0] = xx; came_from[1] = yy; }
    int getCameFromX() { return came_from[0]; }
    int getCameFromY() { return came_from[1]; }
    int getValue() { return value; }
    int getX() { return x; }
    int getY() { return y; }
    void setRoad(bool r) { road = r; }
    bool getRoad() { return road; }
    bool getCroc() { return crocodile; }
};



class Map
{
private:
    int n;                                      //szerokosc rzeki (i dlugosc)
    MapPoint **matrix;                          //mapa
    char* state;

public:
    void allocMap();
    void deleteMap();
    void generateMap(int &range);
    void makeCrocodile(int &length, int x, int y);
    void makeCrocodile(int &x1, int &y1, int &x2, int &y2);
    void setSize(int size) { n = size; }
    void breadthSearch(int& range, int& nSur, std::fstream& file, bool prioON);
    void setState(char* st) { state = st; }
    void showMap(int** results, int& nSur);
};

#endif // MAP_H
