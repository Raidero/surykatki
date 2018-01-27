#include "Map.h"

void Map::allocMap()
{
    matrix = new MapPoint *[n];
    for(int i = 0; i < n; ++i)
    {
        matrix[i] = new MapPoint [n];
    }

    for(int i = 0; i < n; ++i)
        for(int j = 0; j < n; ++j)
            if(i == 0 || i == n-1)
                matrix[i][j].init(i, j, true);
            else
                matrix[i][j].init(i, j, false);
}

void Map::deleteMap()
{
    for(int i = 0 ; i < n; ++i)
    {
        delete [] matrix[i];
    }
    delete [] matrix;
    matrix = nullptr;
}

void Map::generateMap(int &range)
{
    int length;
    int real_chance;
    srand(time(nullptr));

    for(int i = 0; i < n; ++i)
        for(int j = 0; j < n; ++j)
            if(i == 0 || i == n-1)
                matrix[i][j].init(i, j, true);
            else
                matrix[i][j].init(i, j, false);

    for(int i = 0; i < n; ++i)
        for(int j = 0; j < n; ++j)
        {
            if(CHANCE-(range*50) > 0)
                real_chance = (CHANCE-(range*50))/range;
            else
                real_chance = 0;
            if(!matrix[i][j].getCroc() && (length = rand()) <= real_chance)
                makeCrocodile(length, i, j);
        }
}

void Map::makeCrocodile(int &length, int x, int y)
{
    switch(length%4)
    {
    case 0:
        length = length%10;
        for(int i = 0; i < length && x < n && y < n; ++i)
        {
            matrix[x][y].init(x, y, true);
            ++x;
            ++y;
        }
        break;
    case 1:
        length = length%15;
        for(int i = 0; i < length && x < n; ++i)
        {
            matrix[x][y].init(x, y, true);
            ++x;
        }
        break;
    case 2:
        length = length%15;
        for(int i = 0; i < length && y < n; ++i)
        {
            matrix[x][y].init(x, y, true);
            ++y;
        }
        break;
    case 3:
        length = length%10;
        for(int i = 0; i < length && x < n && y >= 0; ++i)
        {
            matrix[x][y].init(x, y, true);
            ++x;
            --y;
        }
        break;
    default:
        break;
    }
}

void Map::makeCrocodile(int &x1, int &y1, int &x2, int &y2)
{
    int temp;
    if(x1 < n && x2 < n && y1 < n && y2 < n)
    {
        if(x1 == x2)
        {
            if(y1 > y2)
            {
                temp = y1;
                y1 = y2;
                y2 = temp;
            }
            for(int i = y1; i <= y2; ++i)
                matrix[x1][i].init(x1, i, true);
            return;
        }
        if(y1 == y2)
        {
            if(x1 > x2)
            {
                temp = x1;
                x1 = x2;
                x2 = temp;
            }
            for(int i = x1; i <= x2; ++i)
                matrix[i][y1].init(i, y1, true);
            return;
        }
        if(x1 > x2)
        {
            temp = x1;
            x1 = x2;
            x2 = temp;
            temp = y1;
            y1 = y2;
            y2 = temp;
        }
        if(y1 > y2)
            for(int i = x1; i <= x2; ++i)
            {
                matrix[x1][y1].init(x1, y1, true);
                ++x1;
                --y1;
            }
        else
            for(int i = x1; i <= x2; ++i)
            {
                matrix[x1][y1].init(x1, y1, true);
                ++x1;
                ++y1;
            }
    }
}

void Map::breadthSearch(int &range, int& nSur, std::fstream& file, bool prioON)
{
    std::queue<MapPoint*> fifo;                 //kolejka do przeszukiwania w szerz

    auto compare = [](MapPoint* left, MapPoint* right) { return left->getX() < right->getX(); };    //ewentualnie to
    std::priority_queue<MapPoint*, std::vector< MapPoint* >, decltype(compare)> prio_queue(compare);

    MapPoint *helper;                           //pomocnik do zapisywania referencji po wyciagnieciu z kolejki
    bool isFound;                               //odpowiada na pytanie, czy znaleziono dla danej surykatki przejscie na drugi brzeg
    int xr, yr;                                 //wspolrzedne okreslajace obwod kola
    int &r = range;                             //promien kola
    double p;                                   //zmienna p do algorytmu rysowania okregu
    int xP, yP;                                 //pomocnicze zmienne na wspolrzedne aktualnie rozpatrywanego punktu
    int x_result = 0, y_result = 0;
    int yS;
    int *results[2];
    results[0] = new int[nSur];
    results[1] = new int[nSur];
    for(int i = 0; i < nSur; ++i)
    {
        results[0][i] = -1;
        results[1][i] = -1;
    }
    srand(time(nullptr));

    for(int a = 0; a < nSur; ++a)
    {
        isFound = false;                                        //przejscie na drugi brzeg nie zostalo znalezione
        if(strcmp(state, "-m1") != 0)                           //jesli generowane, to surykatka na kazdej pozycji na lewym brzegu
        {
            yS = rand()%n;
            if(matrix[0][yS].getValue() != MAX)
            {
                continue;
            }
            matrix[0][yS].setValue(0);
            if(prioON)
                prio_queue.push(&matrix[0][yS]);
            else
                fifo.push(&matrix[0][yS]);
        }
        else                                                    //jesli wczytywane, czytamy pozycje z pliku
        {
            file>>yS;
            if(matrix[0][yS].getValue() != MAX)
            {
                continue;
            }
            matrix[0][yS].setValue(0);
            if(prioON)
                prio_queue.push(&matrix[0][yS]);
            else
                fifo.push(&matrix[0][yS]);
        }

        while(1)                                    //przeszukujemy wszystkie punkty, do ktorych sie mozemy dostac
        {
            if(prioON)
            {
                if(prio_queue.empty())
                    break;
                helper = prio_queue.top();
                prio_queue.pop();
            }
            else
            {
                if(fifo.empty())
                    break;
                helper = fifo.front();                           //wyciagamy z kolejki pierwszego
                fifo.pop();                                      // ..
            }

            //algorytm rysowania okregu
            p = (5.0/4.0) - (double)r;
            xr = 0;
            yr = 0 + r;

            xP = helper->getX();            //zapisujemy
            yP = helper->getY();            // x i y pomocnicze punktu aktualnie rozpatrywanego

            while(xr <= yr)                 //rysujemy kolo, dla kazdego punktu nalezacego do kola
                                            //ustawiamy, ze kosztuje nas dojscie do niego +1 skok wzgledem aktualnego punktu
            {
                if(p < 0)                   //decydujemy, czy yr bedzie zmniejszone czy nie, algorytm rysowania okregu
                {
                    p = p + 2*xr + 1;
                }else
                {
                    p = p + 2*xr + 1 - 2*yr + 1;
                    yr--;
                }

                for(int i = -yr+yP; i <= yr+yP; i++)            //wypelniamy okrag
                {
                    if(i >= 0 && i < n)                         //punkt nie moze wychodzic poza mape
                    {
                        if(xr+xP >= 0 && xr+xP < n)             //to, co wyzej
                        {
                            if(matrix[xr+xP][i].getRoad())
                            {
                                x_result = xP;
                                y_result = yP;
                                isFound = true;
                                break;
                            }
                            if(matrix[xr+xP][i].getCroc() && helper->getValue()+1 < matrix[xr+xP][i].getValue())
                            {           //jesli do danego punktu mozemy sie dostac za pomoca mniejszej ilosci skokow niz moglismy
                                        //do tej pory, oraz jest w tym miejscu krokodyl to zmieniamy wartosc
                                matrix[xr+xP][i].setValue(helper->getValue()+1); //ustaw nowa ilosc skokow
                                matrix[xr+xP][i].setCameFrom(xP, yP);            //zapisz skad przyszedles
                                if(prioON)
                                    prio_queue.push(&matrix[xr+xP][i]);
                                else
                                    fifo.push(&matrix[xr+xP][i]);                //wejdz do kolejki
                                if(xr+xP == n - 1)                               //jesli jestes na drugim brzegu, zakoncz poszukiwanie
                                {
                                    x_result = xr + xP;
                                    y_result = i;
                                    isFound = true;
                                    break;
                                }
                            }
                        }
                        if(-xr+xP >= 0 && -xr+xP < n)           //to samo, co wyzej, tylko dla punktu wzgledem osi OX okregu
                                                                // o przeciwniej wspolrzednej x, nie moze byc rozwiazaniem, jesli to wyzej nie bylo
                        {
                            if(matrix[-xr+xP][i].getRoad())
                            {
                                x_result = xP;
                                y_result = yP;
                                isFound = true;
                                break;
                            }
                            if(matrix[-xr+xP][i].getCroc() && helper->getValue()+1 < matrix[-xr+xP][i].getValue())
                            {
                                matrix[-xr+xP][i].setValue(helper->getValue()+1);
                                matrix[-xr+xP][i].setCameFrom(xP, yP);
                                if(prioON)
                                    prio_queue.push(&matrix[-xr+xP][i]);
                                else
                                    fifo.push(&matrix[-xr+xP][i]);
                            }
                        }
                    }
                }

                for(int i = -xr+yP; i <= xr+yP; i++)
                {
                    if(isFound) break;
                    if(i >= 0 && i < n)
                    {
                        if(yr+xP >= 0 && yr+xP < n)
                        {
                            if(matrix[yr+xP][i].getRoad())
                            {
                                x_result = xP;
                                y_result = yP;
                                isFound = true;
                                break;
                            }
                            if(matrix[yr+xP][i].getCroc() && helper->getValue()+1 < matrix[yr+xP][i].getValue())
                            {
                                matrix[yr+xP][i].setValue(helper->getValue()+1);
                                matrix[yr+xP][i].setCameFrom(xP, yP);
                                if(prioON)
                                    prio_queue.push(&matrix[yr+xP][i]);
                                else
                                    fifo.push(&matrix[yr+xP][i]);
                                if(yr+xP == n - 1)
                                {
                                    x_result = yr + xP;
                                    y_result = i;
                                    isFound = true;
                                    break;
                                }
                            }
                        }
                        if(-yr+xP >= 0 && -yr+xP < n)
                        {
                            if(matrix[-yr+xP][i].getRoad())
                            {
                                x_result = xP;
                                y_result = yP;
                                isFound = true;
                                break;
                            }
                            if(matrix[-yr+xP][i].getCroc() && helper->getValue()+1 < matrix[-yr+xP][i].getValue())
                            {
                                matrix[-yr+xP][i].setValue(helper->getValue()+1);
                                matrix[-yr+xP][i].setCameFrom(xP, yP);
                                if(prioON)
                                    prio_queue.push(&matrix[-yr+xP][i]);
                                else
                                    fifo.push(&matrix[-yr+xP][i]);
                            }
                        }
                    }
                }

                if(isFound)                                 //znaleziono punkt
                {
                    results[0][a] = x_result;
                    results[1][a] = y_result;
                    int temp;
                    while(matrix[x_result][y_result].getValue())
                    {
                        matrix[x_result][y_result].setRoad(true);
                        temp = x_result;
                        x_result = matrix[x_result][y_result].getCameFromX();
                        y_result = matrix[temp][y_result].getCameFromY();
                    }
                    matrix[x_result][y_result].setRoad(true);

                    for(int yi = 0; yi < n; yi++)
                        for(int xi = 0; xi < n; xi++)
                        {
                            if(!matrix[xi][yi].getRoad())
                            {
                                matrix[xi][yi].setValue(MAX);   //zerowanie ilosci skokow na mapie dla kolejnej surykatki
                            }
                        }


                    while(1)                    //czyszczenie kolejki
                    {
                        if(prioON)
                        {
                            if(prio_queue.empty())
                                break;
                            prio_queue.pop();
                        }
                        else
                        {
                            if(fifo.empty())
                                break;
                            fifo.pop();
                        }
                    }
                    break;
                }
                xr++;
            }
        }
    }

    if(strcmp(state, "-m3") != 0)
        showMap(results, nSur);
}

void Map::showMap(int **results, int& nSur)
{
    sf::RenderWindow window(sf::VideoMode(1000, 1000), "Projekt");
    window.setFramerateLimit(60);
    sf::View view(sf::Vector2f(500,500), sf::Vector2f(1000,1000));
    int counter = 0;
    for(int i = 0; i < n; ++i)
        for(int j = 0; j < n; ++j)
        {
            if(matrix[i][j].getCroc()) counter++;
        }
    int sum = 0;
    for(int i = 0; i < nSur; ++i)
    {
        if(results[0][i] != -1)
            sum += matrix[results[0][i]][results[1][i]].getValue();
    }
    sf::Vertex *points = new sf::Vertex[counter];
    counter = 0;
    sf::Vertex *edges = new sf::Vertex[2*sum + 1];
    for(int i = 0; i < n; ++i)
    {
        for(int j = 0; j < n; ++j)
        {
            if(matrix[i][j].getCroc())
            {
                points[counter++] = sf::Vector2f(i, j);
            }
        }
    }

    int counter2 = 0;
    int temp;
    for(int i = 0; i < nSur; ++i)
    {
        int x = results[0][i];
        int y = results[1][i];
        if(x == -1)
            continue;
        int color = counter2;
        while(matrix[x][y].getValue())
        {
            edges[counter2++] = sf::Vector2f(x, y);
            temp = x;
            x = matrix[x][y].getCameFromX();
            y = matrix[temp][y].getCameFromY();
            edges[counter2++] = sf::Vector2f(x, y);
            edges[counter2-2].color = sf::Color::Red;
            edges[counter2-1].color = sf::Color::Red;
        }
        edges[color].color = sf::Color::Blue;
        edges[counter2-1].color = sf::Color::Green;
    }

    view.setViewport(sf::FloatRect(0, 0, 1, 1));
    bool pressed = false;
    bool hide = false;
    float zoom = 1.0;
    sf::Vector2i mouse_coords;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (sf::Event::KeyPressed == event.type)
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::H))
                    hide = !hide;
            if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                if(pressed == false)
                {
                    mouse_coords = sf::Mouse::getPosition(window);
                    pressed = true;
                }
                else
                {
                    view.move((sf::Vector2f)(mouse_coords - sf::Mouse::getPosition(window))*zoom);
                    mouse_coords = sf::Mouse::getPosition(window);
                }
            }
            else
            {
                pressed = false;
            }
            if(event.type == sf::Event::MouseWheelMoved)
            {
                if(event.mouseWheel.delta > 0)
                {
                    zoom *= 0.8f;
                    //view.setCenter(view.getCenter() - (sf::Vector2f)sf::Mouse::getPosition());
                    view.zoom(0.8f);
                    event.mouseWheel.delta--;
                }
                else if(event.mouseWheel.delta < 0)
                {
                    zoom *= 1.25f;
                    view.zoom(1.25f);
                    event.mouseWheel.delta++;
                }
            }
        }
        //view.move(1,1);
        window.clear();

        window.setView(view);
        window.draw(edges, counter2, sf::Lines);
        if(!hide)
            window.draw(points, counter, sf::Points);

        window.display();
    }
}
