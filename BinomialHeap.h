#ifndef BINOMIAL_HEAP_H
#define BINOMIAL_HEAP_H





typedef int TBase;          // Базовый тип ключа.

// Узел биномиальной кучи.
typedef struct blnode
{
    TBase key;              // Ключ.
    struct blnode* parent;  // Указатель на родителя.
    struct blnode* child;   // Указатель на сына.
    struct blnode* sibling; // Указатель на (правого) соседа.
    unsigned degree;        // Степень узла (количество сыновей).
} BlNode;

// Дескриптор биномиальной кучи.
typedef struct
{
    BlNode* head;           // Указатель на первый узел в списке корней.
    unsigned n;             // Количество узлов в куче.
} BlHeap;


// Создает и возвращает инициализированный дескриптор биномиальной кучи.
BlHeap makeBlHeap( void );

void deleteBlHeap( BlHeap* H );

// Возвращает указатель на минимальный узел биномиальной кучи H.
BlNode* getMinimumBl( BlHeap* H );

// Возвращает результат слияния двух биномиальных куч H1 и H2.
// В процессе выполнени функции кучи H1 и H2 "уничтожаются".
BlHeap mergeBl( BlHeap* H1, BlHeap* H2 );

// Вставка в кучу H элемента со значением k.
void insertBl( BlHeap* H, TBase k );

// Удаляет минимальный узел из кучи H и возвращает адрес этого узла.
TBase extractMinBl( BlHeap* H );

// Устанавливает значение ключевого поля узла x биомиальной кучи H
// равным k. Новое значени не должно превышать старое.
void decreaseKeyBl( BlHeap* H, BlNode* x, TBase k );

// Удаляет узел x из кучи H.
void deleteBl( BlHeap* H, BlNode* x );

void preOrderBl( BlHeap* H, void (* f)( BlNode* ));
void postOrderBl( BlHeap* H, void (* f)( BlNode* ));

void directBypass( BlHeap* H );

int isEmptyBl( BlHeap* H );

#endif // BINOMIAL_HEAP_H