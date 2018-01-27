Grzegorz Tomaszczyk, 277347

1. Problem
Surykatki skacz�ce przez rzek� po krokodylach chc� przeskoczy� rzek�
za pomoc� jak najmniejszej liczby skok�w.

2. Program
-m1 [input.txt]  // graficzna prezentacja
-m2 [szerokosc/wysokosc rzeki] [zasieg skoku] [liczba sur] [czy priorytetowa kolejka(0/1)]	//graficzna prezentacja rozwi�zania
-m3 [szerokosc rzeki] [liczba sur] [step(szerokosc+..)] [liczba problemow] [zasieg skoku] [czy priorytetowa]

3. Rozwi�zanie
Wykorzystanie algorytmu przeszukiwania w szerz, r�wnie� mo�liwo��
sprawdzenia dzia�ania z kolejk� priorytetow�, klasa zawieraj�ca map�
oraz klasa definiuj�ca punkt tej mpay.

4.
Map.h		zawiera klas� definiuj�c� map� oraz punkty tej mapy
Map.cpp 	zawiera funkcje s�u��ce do generacji, wczytywania mapy, oraz
		zawiera algorytm przeszukiwania w szerz
main.cpp	pobiera dane wej�ciowe, tworzy map� i uruchamia algorytm
		dan� ilo�� razy

5.Dodatkowe informacje

Problem zosta� rozwi�zany w spos�b dyskretny, ca�a rzeka z krokodylami
generowana jest na samym pocz�tku i rozwi�zuj�c problem mamy zapisan�
w pami�ci ca�� map�. Rozwi�zanie to powoduje, �e dla bardzo du�ych problem�w
(n > 7000) program rzuca wyj�tkiem bad_alloc, poniewa� brakuje pami�ci na map�.

