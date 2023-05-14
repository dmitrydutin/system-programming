# system-programming

1. Внедрить логику работы алгоритма с графами в логику сервера (Сергей) (в отдельный файл) (DONE)
2. Парсер на клиенте параметров командной строки (Дима) (DONE)

client.exe --points-count=6 --start-point=0 --end-point=5 --array=[[1,2,3,4,5,6],[1,2,3,4,5,6],[],[],[],[]]

'--count=6' -> '6'

int pointsCount = 6;
int startPoint = 0;
int endPoint = 5; 
int arraySize = число-длинна-строки-ниже
string array = '[[], [], [], []]';

send to server

3. Парсер на сервере (Сергей) (DONE)

int pointsCount = 6;
int startPoint = 0;
int endPoint = 5;
int [][]array = [[1,2,3,4,5,6],[]];

4. Запустить алгоритм с набором параметров (Сергей) (DONE)
5. Отправить результат на клиент (Дима) (DONE)
6. Отобразить результат на клиенте (Дима) (DONE)
7. Исправить баг с крашем сервера (Дима) (DONE)
