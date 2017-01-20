#ifndef BINARY_HEAP_H
#define BINARY_HEAP_H


typedef int TKey;               // Базовый тип ключа.

// Информационная часть узла.
typedef struct {
    TKey key;                   // Ключ.
    // ..... Другие данные.
} TData;


// Узел бинарной кучи.
typedef struct bnnode
{
    TData data;                 // Информационная часть.
} BnNode;


// Дескриптор бинарной кучи.
typedef struct
{
    BnNode* top;                // Вершина кучи.
    unsigned n;                 // Текущее количество элементов.
    unsigned currentMax;        // Текущий максимальный размер кучи.
} BnHeap;


// Количество элементо кучи, определнное по-умолчанию.
enum {DEFAULT = 16};


// Содает и возвращаетинициалиированный дескриптор бинарной кучи.
BnHeap makeBnHeap( unsigned maxSize );

// Уничтожает кучу H. Освобождает занимаемую ею память.
void doneBnHeap( BnHeap* H );

// Возвращает значение 1, если куча H пуста. Иначе 0.
int isEmptyBn( BnHeap* H );

// Возвращает результат слияния двух бинарных куч H1 и H2.
// В процессе выполнения функции кучи H1 и H2 "уничтожаются".
BnHeap mergeBn( BnHeap* H1, BnHeap* H2 );

// Вставляет в кучу H элемент с данными data.
void insertBn( BnHeap* H, TData data );

// Возвращает информационную часть минимального узла кучи H.
// Если куча пуста, то возвращаемое значение не определено.
TData getMinimumBn( BnHeap* H );

// Извлекает из кучи H минимальный узел и возвращает его информационную часть.
// Если куча пуста, то возвращаемое значение не определено.
TData extractMinBn( BnHeap* H );

// Устанавливает значение ключевого поля узла x бинарной кучи H
// равным k. Новое значение не должно превыщать старое.
void decreaseKeyBn( BnHeap* H, BnNode* x, TKey k );

// Удаляет узел x из кучи H.
void deleteBn( BnHeap* H, BnNode* x );

// Возвращает количество элементов кучи H.
unsigned sizeBn( BnHeap* H );

// Выполняет прямой обход кучи H. К каждому узлу применяется фукция f.
void preOrderBn( BnHeap* H, void (* f)( const BnNode* ));

// Выполняет обратный обход кучи H. К каждому узлу применяется фукция f.
void postOrderBn( BnHeap* H, void (* f)( const BnNode* ));


#endif // BINARY_HEAP_H
