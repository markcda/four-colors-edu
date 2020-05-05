#include <stdio.h>
#include <stdlib.h>

#define DEFAULT_STR_LENGTH 1024

typedef enum { false, true } bool;

typedef struct {
  int num;
  bool status;
} int_nstd;

bool isPartOfInt(char sym) {
  if ((((int)sym < 48) || ((int)sym > 57)) && (sym != '-'))
    return false;
  return true;
}

int_nstd scanInt() {
  // удобная функция для ввода числа
  int_nstd fns = {0, true};
  char number[30];
  fgets(number, 30, stdin);
  fns.num = strtof(number, 0);
  fns.status = isPartOfInt(number[0]);
  return fns;
}

//int main() {
//  printf("Программа раскрашивания карты минимальным количеством цветов.\n");
//  printf("Вводите новые страны, затем оставьте строку ввода пустой.\n");
//  unsigned int i = 1;
//  printf("%ui) Страна: ", i);
//  char *str = (char *)malloc(DEFAULT_STR_LENGTH * sizeof(char));
//  fgets(str, DEFAULT_STR_LENGTH, stdin);
//  printf("\tСоседи (вводите страны через запятую): ");
//  return 0;
//}

//Последовательная раскраска вершин графа при помощи
//обхода графа в глубину.
//Граф представлен структурой Вирта.
typedef int Boolean;
typedef struct L *Lref; // Тип: указатель на заголовочный узел.
typedef struct T *Tref; // Тип: указатель на дуговой узел.
//Описание типа заголовочного узла.
typedef struct L {
  int Key;           //Имя заголовочного узла.
  char* CountryName; //Имя страны.
  int Color;         //Цвет раскраски.
  int Count;         //Количество предшественников.
  Boolean Flag;      //Флаг посещения узла при обходе.
  Tref Trail;        //Указатель на список смежности.
  Lref Next;         //Указатель на следующий узел в списке заголовочных узлов.
} L;

//Описание типа дугового узла.
typedef struct T {
  Lref Id;
  Tref Next;
} T;

Lref Head; //Указатель на голову списка заголовочных узлов.
Lref Tail; //Указатель на фиктивный элемент
           // в конце списка заголовочных узлов.
int MSet[256]; //Вспомогательное множество, содер-
               //жащее 0,1,2...,n.
void SearchGraph(int, Lref *);
Lref GetHead() { return Head; }
Lref GetTail() { return Tail; }
void MakeGraph();
void PrintGraph();
void Color(Lref, int);
void Postr(int);

void Postr(int n)
//Построение вспомогательного множества MSet.
{
  for (int i = 0; i < 256; i++)
    MSet[i] = (i <= n) ? 1 : 0;
}

void SearchGraph(int w, Lref *h)
//Функция возвращает указатель на заголовочный узел
//с ключом w в графе, заданном структурой Вирта с указателем Head.
{
  *h = Head;
  (*Tail).Key = w;
  while ((**h).Key != w)
    *h = (**h).Next;
  if (*h == Tail)
  //В списке заголовочных узлов нет узла с ключом w.
  //Поместим его в конец списка Head.
  {
    Tail = (L*) malloc(sizeof(L)); //new (L);
    (**h).Count = 0;
    (**h).Trail = NULL;
    (**h).Next = Tail;
  }
}

void MakeGraph()
//Функция возвращает указатель Head на структуру
//Вирта, соответствующую ориентированному графу.
{
  int x, y;
  Lref p, q;   //Рабочие указатели.
  Tref t, r;   //Рабочие указатели.
  Boolean Res; //Флаг наличия дуги.
  printf("Вводите начальную вершину дуги: ");
  scanf("%d", &x); //cin >> x;
  while (x != 0) {
    printf("Вводите конечную вершину дуги: ");
    scanf("%d", &y);
    //Определим, существует ли в графе дуга (x,y)?
    SearchGraph(x, &p);
    SearchGraph(y, &q);
    r = (*p).Trail;
    Res = false;
    while ((r != NULL) && (!Res))
      if ((*r).Id == q)
        Res = true;
      else
        r = (*r).Next;
    if (!Res) //Если дуга отсутствует, то поместим её в граф.
    {
      t = (T*) malloc(sizeof(T)); //new (T);
      (*t).Id = q;
      (*t).Next = (*p).Trail;
      (*p).Trail = t;
      (*q).Count++;
    }
    printf("Вводите начальную вершину дуги: ");
    scanf("%d", &x);
  }
}

void PrintGraph()
//Вывод структуры Вирта, заданной указателем
// Head и соответствующей ориентированному графу.
{
  Lref p; //Рабочий указатель.
  Tref q; //Рабочий указатель.

  p = Head;
  while (p != Tail) {
    printf("%d(", (*p).Key);
    q = (*p).Trail;
    while (q != NULL) {
      printf("%d ", (*(*q).Id).Key);
      q = (*q).Next;
    }
    printf(")");//cout << ")";
    p = (*p).Next;
    printf(" ");
  }
}

void Color(Lref r, int n)
//Последовательная раскраска графа при помощи
//рекурсивного обхода графа в глубину.
// r - указатель на структуру Вирта.
// MSet - глобальное множество.
// n    - количество вершин в графе.
{
  Tref t, t1;
  int i; //Параметр цикла.
  Boolean Fl;

  t = r->Trail;
  r->Flag = false;
  //Сейчас мы находимся в вершине r->Key.
  //Исключим цвета всех вершин, смежных вершине
  // r->Key, из множества MSet.
  t1 = t;
  while (t1 != NULL) {
    MSet[t1->Id->Color] = 0;
    t1 = t1->Next;
  }
  //Выбор цвета вершины: это "первый" элемент множества MSet.
  Fl = true;
  i = 1;
  while (Fl)
    if (MSet[i])
      Fl = false;
    else
      i++;
  r->Color = i; //Цвет присвоен!
  printf("(%d,%d) ", r->Key, r->Color);
  //Восстановление вспомогательного множества MSet.
  for (i = 0; i < 256; MSet[i++] = 0)
    ;
  for (i = 0; i <= n; MSet[i++] = 1)
    ;
  // -------------
  while (t != NULL) {
    if (t->Id->Flag)
      Color(t->Id, n);
    t = t->Next;
  }
}

int main() {
  Lref t;    //Рабочий указатель для перемещения
             // по списку заголовочных звеньев.
  int n = 0; //Количество вершин в графе.
  Head = Tail = (L*) malloc(sizeof(L));// new (L);
  //Построение графа и вывод его структуры Вирта.
  MakeGraph();
  PrintGraph();
  //Раскраска с помощью рекурсивного обхода графа в глубину.
  //Инициализация.
  t = GetHead(); //Установлен рабочий указатель.
  while (t != GetTail()) {
    t->Flag = true;
    t->Color = 0;
    n++;
    t = (*t).Next;
  }
  // ------------------------------------
  //Построение вспомогательного множества MSet.
  Postr(n);
  printf("Результат раскраски: ");
  Color(GetHead(), n);
  printf("\n");
  return 0;
}
