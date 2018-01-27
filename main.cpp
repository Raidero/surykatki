#include <Map.h>

#define NUMBER_OF_STEPS 70

int main(int argc, char* argv[])
{
    using namespace std;
    if(argc < 2)
        return -1;

    Map map1;                                   //mapa
    std::fstream file;                          //mozliwy plik z danymi do mapy
    std::fstream output;
    output.open("result.txt");
    int n;                                      //szerokosc rzeki (i dlugosc)
    int step = 0;                               //krok
    int n_gen = 1;                              //ilosc generacji
    int range;                                  //dlugosc skoku
    int nSur = 20;                              //ilosc surykatek, z zalozenia jest jedna
    bool generated = false;                     //mapa generowana czy wczytywana z pliku?
    bool is_prio = false;
    double result[NUMBER_OF_STEPS][2];

    //generowanie mapy lub wczytywanie z pliku
    map1.setState(argv[1]);
    if(strcmp(argv[1], "-m1") == 0)                 //podajemy mape z pliku
    {
        //generated = false;
        int nCroc, xC1, yC1, xC2, yC2;              //ilosc krokodyli, wspolrzedne poczatku krokodyla i jego konca
        file.open(argv[2]);
        file>>n>>range>>nCroc>>nSur;                //wczytywanie..
        map1.setSize(n);
        map1.allocMap();                            //alokacja miejsca na mape rozmiaru n
        for(int i = 0; i < nCroc; ++i)
        {
            file>>xC1>>yC1>>xC2>>yC2;               //wczytywanie krokodyli
            map1.makeCrocodile(xC1, yC1, xC2, yC2); //rysowanie ich na mapie
        }
    }
    else if(strcmp(argv[1], "-m2") == 0)            //generujemy automatycznie
    {
        generated = true;
        n = stoi(argv[2], nullptr, 10);             //zapisujemy szerokosc
        map1.setSize(n);
        map1.allocMap();                            //alokacja miejsca na mape..
        range = stoi(argv[3], nullptr, 10);         //zapisujemy dlugosc skoku
        nSur = stoi(argv[4], nullptr, 10);
        is_prio = stoi(argv[5], nullptr, 10);
        //n_gen = stoi(argv[6], nullptr, 10);
    }
    else if(strcmp(argv[1], "-m3") == 0)
    {
        generated = true;
        n = stoi(argv[2], nullptr, 10);
        map1.setSize(n);
        map1.allocMap();
        nSur = stoi(argv[3], nullptr, 10);
        step = stoi(argv[4], nullptr, 10);
        n_gen = stoi(argv[5], nullptr, 10);
        range = stoi(argv[6], nullptr, 10);
        is_prio = stoi(argv[7], nullptr, 10);
    }
    else
    {
        return -2;
    }

    int ticks;
    int sum = 0;
    for(int i = 0; i < NUMBER_OF_STEPS; ++i)    //number of steps, np n = 1000 1100 1200 1300.. 1900 2000
    {
        for(int j = 0; j < n_gen; ++j)          //ilosc instancji problemu, czyli ilosc map wygenerowanych dla danej wielkosc n
        {
            if(generated)
                map1.generateMap(range);
            ticks = clock();
            map1.breadthSearch(range, nSur, file, is_prio);
            sum += clock() - ticks;
            //output<<j<<' '<<(double)(clock() - ticks)<<'\n';
        }
        std::cout<<"For n = "<<n<<", q(n) = "<< sum / (double)(n*n)<<'\n';
        output<<sum<<'\n';
        result[i][0] = (double)n;
        result[i][1] = (double)sum;
        sum = 0;
        if(step == 0)
            break;
        else
        {
            map1.deleteMap();
            n += step;
            map1.setSize(n);
            map1.allocMap();
        }
    }
    output<<'\n';
    if(strcmp("-m3", argv[1]) == 0)
    {
        double n_med = result[NUMBER_OF_STEPS>>1][0];
        double t_n_med = result[NUMBER_OF_STEPS>>1][1];
        for(int i = 0; i < NUMBER_OF_STEPS; ++i)
        {
            std::cout<<"For n = "<<result[i][0]<<"\t, q(n) = "<<(result[i][1]*n_med*n_med)/(result[i][0]*result[i][0]*t_n_med)<<'\n';
            output<<result[i][0]<<' '<<(result[i][1]*n_med*n_med)/(result[i][0]*result[i][0]*t_n_med)<<'\n';
        }
    }

    if(strcmp("-m1", argv[1]) == 0)
    {
        file.close();
    }
    map1.deleteMap();
    return 0;
}

