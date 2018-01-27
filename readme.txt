Grzegorz Tomaszczyk, 277347

1. Problem
Surykatki skacz¹ce przez rzekê po krokodylach chc¹ przeskoczyæ rzekê
za pomoc¹ jak najmniejszej liczby skoków.

2. Program
-m1 [input.txt]  // graficzna prezentacja
-m2 [szerokosc/wysokosc rzeki] [zasieg skoku] [liczba sur] [czy priorytetowa kolejka(0/1)]	//graficzna prezentacja rozwi¹zania
-m3 [szerokosc rzeki] [liczba sur] [step(szerokosc+..)] [liczba problemow] [zasieg skoku] [czy priorytetowa]

3. Rozwi¹zanie
Wykorzystanie algorytmu przeszukiwania w szerz, równie¿ mo¿liwoœæ
sprawdzenia dzia³ania z kolejk¹ priorytetow¹, klasa zawieraj¹ca mapê
oraz klasa definiuj¹ca punkt tej mpay.

4.
Map.h		zawiera klasê definiuj¹c¹ mapê oraz punkty tej mapy
Map.cpp 	zawiera funkcje s³u¿¹ce do generacji, wczytywania mapy, oraz
		zawiera algorytm przeszukiwania w szerz
main.cpp	pobiera dane wejœciowe, tworzy mapê i uruchamia algorytm
		dan¹ iloœæ razy

5.Dodatkowe informacje

Problem zosta³ rozwi¹zany w sposób dyskretny, ca³a rzeka z krokodylami
generowana jest na samym pocz¹tku i rozwi¹zuj¹c problem mamy zapisan¹
w pamiêci ca³¹ mapê. Rozwi¹zanie to powoduje, ¿e dla bardzo du¿ych problemów
(n > 7000) program rzuca wyj¹tkiem bad_alloc, poniewa¿ brakuje pamiêci na mapê.

