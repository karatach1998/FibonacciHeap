/*
 * Стуктура TData может быть расширена, но она обязана содержать поле key
 * любого типа, над которым определены операции сравнения.
 */

#ifndef BINOMIAL_HEAP_H
#define BINOMIAL_HEAP_H


typedef int TKey;           // Базовый тип ключа.

// Информационная часть узла.
typedef struct {
    TKey key;               // Ключ.
    // ..... Другие данные.
} TData;

// Узел биномиальной кучи.
typedef struct blnode
{
    TData data;             // Данные.
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

// Удаляет кучу H. Освобождает всю занимаемую её память.
void doneBlHeap( BlHeap* H );

// Возвращает значение 1, если куча H пуста. Иначе 0.
int isEmptyBl( const BlHeap* H );

// Вставляет в кучу H элемент с данными data.
void insertBl( BlHeap* H, TData data );

// Возвращает результат слияния двух биномиальных куч H1 и H2.
// В процессе выполнения функции кучи H1 и H2 "уничтожаются".
BlHeap mergeBl( BlHeap* H1, BlHeap* H2 );

// Возвращает информационную часть минимального узла биномиальной кучи H.
// Если куча пуста, то возвращаемое значение не определено.
TData getMinimumBl( const BlHeap* H );

// Удаляет минимальный узел из кучи H и его информационную часть.
// Если куча пуста, то возвращаемое значение не определено.
TData extractMinBl( BlHeap* H );

// Устанавливает значение ключевого поля узла x биомиальной кучи H
// равным k. Новое значени не должно превышать старое.
void decreaseKeyBl( BlHeap* H, BlNode* x, TKey k );

// Удаляет узел x из кучи H.
void deleteBl( BlHeap* H, BlNode* x );

// Возвращает количество элементов в куче H.
unsigned sizeBl( BlHeap* H );

// Выполняет прямой обход кучи H. К каждому узлу применяется фукция f.
void preOrderBl( BlHeap* H, void (* f)( const BlNode* ));

// Выполняет обратный обход кучи H. К каждому узлу применяется фукция f.
void postOrderBl( BlHeap* H, void (* f)( const BlNode* ));

#endif // BINOMIAL_HEAP_H