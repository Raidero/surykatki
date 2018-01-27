#include <Map.h>



void makeCrocodile(MapPoint **tab, int &length, int x, int y, int &n);
void makeCrocodile(MapPoint **tab, int &n, int &x1, int &y1, int &x2, int& y2);
MapPoint** allocMap(MapPoint **tab, int &n);
void generateMap(MapPoint **tab, int &n, int &range);
void showMap(MapPoint **tab, int &n, int x, int y);


int main(int argc, char* argv[])
{
    using namespace std;
    if(argc < 2)
        return -1;

    std::fstream file;                          //mozliwy plik z danymi do mapy
    std::fstream output;                        //plik wynikowy
    std::queue<MapPoint*> fifo;                 //kolejka do przeszukiwania w szerz
    MapPoint *helper;                           //pomocnik do zapisywania referencji po wyciagnieciu z kolejki
    int n;                                      //szerokosc rzeki (i dlugosc)
    int step;                                   //krok
    int n_gen;                                  //ilosc generacji
    int range;                                  //dlugosc skoku
    MapPoint **matrix;                          //mapa
    int nSur, yS;                               //ilosc surykatek, wspolrzedna y'kowa kazdej surykatki
    bool generated;                             //mapa generowana czy wczytywana z pliku?

    //generowanie mapy lub wczytywanie z pliku

    if(argc > 1 && strcmp(argv[1], "-m1") == 0) //podajemy mape z pliku
    {
        generated = false;
        int nCroc, xC1, yC1, xC2, yC2;          //ilosc krokodyli, wspolrzedne poczatku krokodyla i jego konca
        file.open(argv[2]);
        file>>n>>range>>nCroc>>nSur;            //wczytywanie..
        matrix = allocMap(matrix, n);           //alokacja miejsca na mape rozmiaru n
        for(int i = 0; i < nCroc; ++i)
        {
            file>>xC1>>yC1>>xC2>>yC2;           //wczytywanie krokodyli
            makeCrocodile(matrix, n, xC1, yC1, xC2, yC2);//rysowanie ich na mapie
        }
        output.open(argv[3]);
        n_gen = 1;
        step = 0;
    }
    else if(argc > 1 && strcmp(argv[1], "-m2") == 0)//generujemy automatycznie
    {
        generated = true;
        n = stoi(argv[2], nullptr, 10);             //zapisujemy szerokosc
        range = stoi(argv[3], nullptr, 10);         //zapisujemy dlugosc skoku
        matrix = allocMap(matrix, n);               //alokacja miejsca na mape..
        nSur = 1;                                   //surykatek tyle, jaka wysokosc rzeki, rozstawione na calym lewym brzegu
        //generateMap(matrix, n, range);              //generacja mapy dla danej wielkosc n oraz dlugosci skoku
        output.open(argv[4]);
        n_gen = 1;
        step = 0;
    }
    else if(argc > 1 && strcmp(argv[1], "-m3") == 0)
    {
        generated = true;
        n = stoi(argv[2], nullptr, 10);
        matrix = allocMap(matrix, n);
        nSur = stoi(argv[3], nullptr, 10);
        step = stoi(argv[4], nullptr, 10);
        n_gen = stoi(argv[5], nullptr, 10);
        range = stoi(argv[6], nullptr, 10);
    }

    bool isFound;                               //odpowiada na pytanie, czy znaleziono dla danej surykatki przejscie na drugi brzeg
    int xr, yr;                                 //wspolrzedne okreslajace obwod kola
    int &r = range;                             //promien kola
    double p;                                   //zmienna p do algorytmu rysowania okregu
    int xP, yP;                                 //pomocnicze zmienne na wspolrzedne aktualnie rozpatrywanego punktu
    int x_result, y_result;
    srand(time(0));

    int ticks;

    for(int a = 0; a < NUMBER_OF_STEPS; ++a)
    {
        for(int b = 0; b < n_gen; ++b)
        {
            if(generated)
                generateMap(matrix, n, range);
            ticks = clock();
            for(int i = 0; i < nSur; ++i)
            {
                isFound = false;                                        //przejscie na drugi brzeg nie zostalo znalezione
                if(generated)                                           //jesli generowane, to surykatka na kazdej pozycji na lewym brzegu
                {
                    yS = rand()%n;
                    matrix[0][yS].setValue(0);
                    fifo.push(&matrix[0][yS]);
                }
                else                                                    //jesli wczytywane, czytamy pozycje z pliku
                {
                    file>>yS;
                    matrix[0][yS].setValue(0);
                    fifo.push(&matrix[0][yS]);
                }
                while(!fifo.empty())                                    //przeszukujemy wszystkie punkty, do ktorych sie mozemy dostac
                {
                    helper = fifo.front();                              //wyciagamy z kolejki pierwszego
                    fifo.pop();                                         // ..

                    //algorytm rysowania okregu
                    p = (5.0/4.0) - (double)r;
                    xr = 0;
                    yr = 0 + r;

                    xP = helper->getX();                                //zapisujemy
                    yP = helper->getY();                                // x i y pomocnicze punktu aktualnie rozpatrywanego

                    while(xr <= yr)                                     //rysujemy kolo, dla kazdego punktu nalezacego do kola
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
                                    if(matrix[xr+xP][i].getCroc() && helper->getValue()+1 < matrix[xr+xP][i].getValue())
                                    {           //jesli do danego punktu mozemy sie dostac za pomoca mniejszej ilosci skokow niz moglismy
                                                //do tej pory, oraz jest w tym miejscu krokodyl to zmieniamy wartosc
                                        matrix[xr+xP][i].setValue(helper->getValue()+1); //ustaw nowa ilosc skokow
                                        matrix[xr+xP][i].setCameFrom(xP, yP);            //zapisz skad przyszedles
                                        fifo.push(&matrix[xr+xP][i]);                    //wejdz do kolejki
                                        if(xr+xP == n - 1)                          //jesli jestes na drugim brzegu, zakoncz poszukiwanie
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
                                    if(matrix[-xr+xP][i].getCroc() && helper->getValue()+1 < matrix[-xr+xP][i].getValue())
                                    {
                                        matrix[-xr+xP][i].setValue(helper->getValue()+1);
                                        matrix[-xr+xP][i].setCameFrom(xP, yP);
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
                                    if(matrix[yr+xP][i].getCroc() && helper->getValue()+1 < matrix[yr+xP][i].getValue())
                                    {
                                        matrix[yr+xP][i].setValue(helper->getValue()+1);
                                        matrix[yr+xP][i].setCameFrom(xP, yP);
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
                                    if(matrix[-yr+xP][i].getCroc() && helper->getValue()+1 < matrix[-yr+xP][i].getValue())
                                    {
                                        matrix[-yr+xP][i].setValue(helper->getValue()+1);
                                        matrix[-yr+xP][i].setCameFrom(xP, yP);
                                        fifo.push(&matrix[-yr+xP][i]);
                                    }
                                }
                            }
                        }

                        if(isFound)                                 //znaleziono punkt
                        {
                            //cout<<"F";
                            if(strcmp("-m3", argv[1]) != 0)
                            {
                                showMap(matrix, n, x_result, y_result);
                                int temp;
                                cout<<"Found\n";
                                while(matrix[x_result][y_result].getValue())
                                {
                                    output<<x_result<<' '<<y_result<<" <- ";
                                    temp = x_result;
                                    x_result = matrix[x_result][y_result].getCameFromX();
                                    y_result = matrix[temp][y_result].getCameFromY();
                                }
                                output<<x_result<<' '<<y_result<<'\n';
                            }

                            for(int yi = 0; yi < n; yi++)
                                for(int xi = 0; xi < n; xi++)
                                    matrix[xi][yi].setValue(MAX);   //zerowanie ilosci skokow na mapie dla kolejnej surykatki

                            while(!fifo.empty())                    //czyszczenie kolejki
                            {
                                fifo.pop();
                            }
                            break;
                        }
                        xr++;
                    }
                }
            }
            std::cout<<(10000.0/5.85)*(double)(clock() - ticks)/(double)(n*n)<<'\n';
        }
        if(step == 0)
            break;
        else
        {
            for(int i = 0 ; i < n; ++i)
            {
                delete [] matrix[i];
            }
            delete [] matrix;
            matrix = nullptr;
            n += step;
            matrix = allocMap(matrix, n);
        }
    }

    if(strcmp("-m1", argv[1]) == 0)
    {
        file.close();
        output.close();
    }
    else if(strcmp("-m2", argv[1]) == 0)
    {
        output.close();
    }
    for(int i = 0 ; i < n; ++i)
    {
        delete [] matrix[i];
    }
    delete [] matrix;
    return 0;
}


// functions for generation and loading maps


void makeCrocodile(MapPoint **tab, int &n, int &x1, int &y1, int &x2, int &y2)
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
                tab[x1][i].init(x1, i, true);
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
                tab[i][y1].init(i, y1, true);
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
                tab[x1][y1].init(x1, y1, true);
                ++x1;
                --y1;
            }
        else
            for(int i = x1; i <= x2; ++i)
            {
                tab[x1][y1].init(x1, y1, true);
                ++x1;
                ++y1;
            }
    }
}

void makeCrocodile(MapPoint **tab, int &length, int x, int y, int &n)
{
    switch(length%4)
    {
    case 0:
        length = length%10;
        for(int i = 0; i < length && x < n && y < n; ++i)
        {
            tab[x][y].init(x, y, true);
            ++x;
            ++y;
        }
        break;
    case 1:
        length = length%15;
        for(int i = 0; i < length && x < n; ++i)
        {
            tab[x][y].init(x, y, true);
            ++x;
        }
        break;
    case 2:
        length = length%15;
        for(int i = 0; i < length && y < n; ++i)
        {
            tab[x][y].init(x, y, true);
            ++y;
        }
        break;
    case 3:
        length = length%10;
        for(int i = 0; i < length && x < n && y >= 0; ++i)
        {
            tab[x][y].init(x, y, true);
            ++x;
            --y;
        }
        break;
    default:
        break;
    }
}

MapPoint** allocMap(MapPoint **tab, int &n)
{
    tab = new MapPoint *[n];
    for(int i = 0; i < n; ++i)
    {
        tab[i] = new MapPoint [n];
    }

    for(int i = 0; i < n; ++i)
        for(int j = 0; j < n; ++j)
            if(i == 0 || i == n-1)
                tab[i][j].init(i, j, true);
            else
                tab[i][j].init(i, j, false);
    return tab;
}

void generateMap(MapPoint **tab, int &n, int &range)
{
    int length;
    int real_chance;
    srand(time(NULL));

    for(int i = 0; i < n; ++i)
        for(int j = 0; j < n; ++j)
            if(i == 0 || i == n-1)
                tab[i][j].init(i, j, true);
            else
                tab[i][j].init(i, j, false);

    for(int i = 0; i < n; ++i)
        for(int j = 0; j < n; ++j)
        {
            if(CHANCE-(range*50) > 100)
                real_chance = (CHANCE-(range*50))/range;
            else
                real_chance = (CHANCE / (range*sqrt(range))) + 1;
            if(!tab[i][j].getCroc() && (length = rand()) <= real_chance)
                makeCrocodile(tab, length, i, j, n);
        }
}

void showMap(MapPoint **tab, int &n, int x, int y)
{
    sf::RenderWindow window(sf::VideoMode(1000, 1000), "Projekt");
    window.setFramerateLimit(60);
    sf::View view(sf::Vector2f(500,500), sf::Vector2f(1000,1000));
    int counter = 0;
    for(int i = 0; i < n; ++i)
        for(int j = 0; j < n; ++j)
        {
            if(tab[i][j].getCroc()) counter++;
        }
    sf::Vertex *points = new sf::Vertex[counter];
    counter = 0;
    sf::Vertex *edges = new sf::Vertex[2*(tab[x][y].getValue())];

    for(int i = 0; i < n; ++i)
    {
        for(int j = 0; j < n; ++j)
        {
            if(tab[i][j].getCroc())
            {
                points[counter++] = sf::Vector2f(i, j);
            }
        }
    }

    int counter2 = 0;
    int temp;
    while(tab[x][y].getValue())
    {
        edges[counter2++] = sf::Vector2f(x, y);
        temp = x;
        x = tab[x][y].getCameFromX();
        y = tab[temp][y].getCameFromY();
        edges[counter2++] = sf::Vector2f(x, y);
        edges[counter2-2].color = sf::Color::Red;
        edges[counter2-1].color = sf::Color::Red;
    }
    view.setViewport(sf::FloatRect(0, 0, 1, 1));
    bool pressed = false;
    float zoom = 1.0;
    sf::Vector2i mouse_coords;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
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
        window.draw(points, counter, sf::Points);

        window.display();
    }
}

