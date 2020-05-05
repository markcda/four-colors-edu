#include <stdio.h>
#include <stdlib.h>

#define DEFAULT_STR_LENGTH 256

typedef enum { false, true } bool;
typedef struct L *Lref; // Тип: указатель на заголовочный узел.
typedef struct T *Tref; // Тип: указатель на дуговой узел.
char colors[4][20] = {"зелёный\0", "красный\0", "жёлтый\0" ,"синий\0"};

//Описание типа заголовочного узла.
typedef struct L {
  char *CountryName; //Имя страны.
  int Color;         //Цвет раскраски.
  int Count;         //Количество предшественников.
  bool Flag;         //Флаг посещения узла при обходе.
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
void SearchGraph(char *, Lref *);
Lref GetHead() { return Head; }
Lref GetTail() { return Tail; }
void MakeGraph();
void PrintGraph();
void Color(Lref, int);
void Postr(int);

bool isEndOfString(char sym) {
  //Проверка на конец строки.
  if ((sym == '\0') || (sym == '\n') || ((int)sym == 10))
    return true;
  return false;
}

char *scanString() {
  //Ввод строки.
  char *str = malloc(DEFAULT_STR_LENGTH * sizeof(char));
  fgets(str, DEFAULT_STR_LENGTH, stdin);
  int cntr = 0;
  for (int i = 0; i < DEFAULT_STR_LENGTH; i++) {
    if (!isEndOfString(str[i]))
      cntr++;
    else
      break;
  }
  char *k = malloc((cntr + 1) * sizeof(char));
  for (int i = 0; i < cntr; i++)
    k[i] = str[i];
  k[cntr] = '\0';
  free(str);
  return k;
}

bool isEqual(char *str1, char *str2) {
  //Сравнение строк.
  unsigned short int len1 = DEFAULT_STR_LENGTH, len2 = DEFAULT_STR_LENGTH;
  for (int i = 0; i < DEFAULT_STR_LENGTH; i++)
    if (isEndOfString(str1[i])) {
      len1 = i;
      break;
    }
  for (int i = 0; i < DEFAULT_STR_LENGTH; i++)
    if (isEndOfString(str2[i])) {
      len2 = i;
      break;
    }
  if (len1 != len2)
    return false;
  bool isEqual = true;
  for (int i = 0; i < len1; i++)
    if (str1[i] != str2[i])
      isEqual = false;
  return isEqual;
}

void Postr(int n) {
  //Построение вспомогательного множества MSet.
  for (int i = 0; i < 256; i++)
    MSet[i] = (i <= n) ? 1 : 0;
}

void SearchGraph(char *w, Lref *h) {
  //Функция возвращает указатель на заголовочный узел
  //с ключом w в графе, заданном структурой Вирта с указателем Head.
  *h = Head;
  (*Tail).CountryName = w;
  while (!isEqual((**h).CountryName, w))
    *h = (**h).Next;
  if (*h == Tail)
  //В списке заголовочных узлов нет узла с ключом w.
  //Поместим его в конец списка Head.
  {
    Tail = (L *)malloc(sizeof(L)); // new (L);
    (**h).Count = 0;
    (**h).Trail = NULL;
    (**h).Next = Tail;
  }
}

void MakeGraph() {
  //Функция возвращает указатель Head на структуру
  //Вирта, соответствующую ориентированному графу.
  char *x, *y;
  Lref p, q; //Рабочие указатели.
  Tref t, r; //Рабочие указатели.
  bool Res;  //Флаг наличия дуги.
  printf("Введите название страны: ");
  x = scanString();
  while (!isEndOfString(x[0])) {
    printf("Введите соседа: ");
    y = scanString();
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
      t = (T *)malloc(sizeof(T)); // new (T);
      (*t).Id = q;
      (*t).Next = (*p).Trail;
      (*p).Trail = t;
      (*q).Count++;
    }
    //Определим, существует ли в графе дуга (y,x) (обратная)?
    SearchGraph(y, &p);
    SearchGraph(x, &q);
    r = (*p).Trail;
    Res = false;
    while ((r != NULL) && (!Res))
      if ((*r).Id == q)
        Res = true;
      else
        r = (*r).Next;
    if (!Res) //Если дуга отсутствует, то поместим её в граф.
    {
      t = (T *)malloc(sizeof(T)); // new (T);
      (*t).Id = q;
      (*t).Next = (*p).Trail;
      (*p).Trail = t;
      (*q).Count++;
    }
    printf("Введите название страны: ");
    x = scanString();
  }
}

void PrintGraph() {
  //Вывод структуры Вирта, заданной указателем Head и соответствующей
  //ориентированному графу.
  Lref p; //Рабочий указатель.
  Tref q; //Рабочий указатель.
  p = Head;
  while (p != Tail) {
    printf("%s( ", (*p).CountryName);
    q = (*p).Trail;
    while (q != NULL) {
      printf("%s ", (*(*q).Id).CountryName);
      q = (*q).Next;
    }
    printf(")");
    p = (*p).Next;
    printf(" ");
  }
  printf("\n");
}

void Color(Lref r, int n) {
  //Последовательная раскраска графа при помощи
  //рекурсивного обхода графа в глубину.
  // r    - указатель на структуру Вирта.
  // MSet - глобальное множество.
  // n    - количество вершин в графе.
  Tref t, t1;
  int i; //Параметр цикла.
  bool Fl;
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
  printf("%s - %s\n", r->CountryName, colors[r->Color]);
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
  //Последовательная раскраска вершин графа при помощи
  //обхода графа в глубину.
  //Граф представлен структурой Вирта.
  Lref t;    //Рабочий указатель для перемещения
             // по списку заголовочных звеньев.
  int n = 0; //Количество вершин в графе.
  Head = Tail = (L *)malloc(sizeof(L)); // new (L);
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
  printf("Результат раскраски:\n");
  Color(GetHead(), n);
  return 0;
}
