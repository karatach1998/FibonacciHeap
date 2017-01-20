/*
 * Стуктура TData может быть расширена, но она обязана содержать поле key
 * любого типа, над которым определены операции сравнения.
 */

#ifndef FIBONACCI_HEAP_H
#define FIBONACCI_HEAP_H


typedef int TKey;               // Базовый тип ключа.

// Информационная часть узла.
typedef struct {
    TKey key;                   // Ключ.
    // ..... Другие данные.
} TData;

// Узел фибоначчивой кучи.
typedef struct fbnode
{
    TData data;                 // Информационная часть.
    struct fbnode* parent;      // Указатель на родителя.
    struct fbnode* child;       // Указатель на сына.
    struct fbnode* left;        // Указатель на левого соседа.
    struct fbnode* right;       // Указатель на правого соседа.
    unsigned degree;            // Степень узла (количество сыновей).
    unsigned char mark;         // Метка -- был ли удален хотя бы один сын.
} FbNode;

// Дескриптор фибоначчивой кучи.
typedef struct {
    FbNode* min;                // Указатель на минимальный узел кучи.
    unsigned n;                 // Текущее количество узлов в куче.
} FbHeap;

// Создает и возвращает инициализированный дескриптор фибоначчиевой кучи.
FbHeap makeFbHeap( void );

// Уничтожает кучу H. Освобождает всю занимаемую ею память.
void doneFbHeap( FbHeap* H );

// Возвращает значение 1, если куча H пуста. Иначе 0.
int isEmptyFb( const FbHeap* H );

// Вставляет в кучу H элемент с данными data.
void insertFb( FbHeap* H, TData data );

// Возвращает результат слияния двух фибоначчиевых куч H1 и H2.
// В процессе выполнения функции кучи H1 и H2 "уничтожаются".
FbHeap mergeFb( FbHeap* H1, FbHeap* H2 );

// Возвращает информационную часть минимального узла кучи H.
// Если куча пуста, то возвращаемое значение не определено.
TData getMinimumFb( FbHeap* H );

// Извлекает из кучи H минимальный узел и возвращает его информационную часть.
// Если куча пуста, то возвращаемое значение не определено.
TData extractMinFb( FbHeap* H );

// Устанавливает значение ключевого поля узла x фибоначчиевой кучи H
// равным k. Новое значени не должно превышать старое.
void decreaseKeyFb( FbHeap* H, FbNode* x, TKey k );

// Удаляет узел x из кучи H.
void deleteFb( FbHeap* H, FbNode* x );

// Возвращает количество  элементов в куче H.
unsigned sizeFb( const FbHeap* H );

// Выполняет прямой обход кучи H. К каждому узлу применяется фукция f.
void preOrderFb( FbHeap* H, void (* f)( const FbNode* ));

// Выполняет обратный обход кучи H. К каждому узлу применяется фукция f.
void postOrderFb( FbHeap* H, void (* f)( const FbNode* ));


#endif // FIBONACCI_HEAP_H