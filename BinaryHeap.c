/*
 * В данном файле располагается реализация СД типа бинарная куча
 * как отображение на массив.
 */

#include <stdlib.h>
#include "BinaryHeap.h"


#define PARENT(i)   ((i - 1) / 2)
#define LEFT(i)     (2 * i + 1)
#define RIGHT(i)    (2 * i + 2)


// R -- макрос, который позволяет, путем замены его подстановки,
// изменять порядок упорядочнности элементов кучи.
// Куча:: "неубывающая" : "<", "невозрастающая" : ">"
#define R <

enum {FALSE, TRUE};


// СОЗДАНИЕ И УНИЧТОЖЕНИЕ
// ==========================================================================


BnHeap makeBnHeap( unsigned maxSize )
{
    static const unsigned BEGIN_DEFAULT_SIZE = 16;


    BnHeap H;

    H.n = 0;
    H.currentMax = maxSize > BEGIN_DEFAULT_SIZE
                   ? maxSize : BEGIN_DEFAULT_SIZE;
    H.top = (BnNode*) malloc(sizeof(BnNode) * H.currentMax);
    return H;
}



void doneBnHeap( BnHeap* H )
{
    free(H->top);
}


// ОПЕРАЦИЯ ПРЕДИКАТ
// ==========================================================================


int isEmptyBn( BnHeap* H )
{ return H->n == 0; }


// ОСНОВНЫЕ ОПЕРАЦИИИ ВЗАИМОДЕЙСТВИЯ
// ==========================================================================


// swapData: Обмен значений типа TData, хранящихся по адресам ptr1 и ptr2.
static void swapData( TData* ptr1, TData* ptr2 )
{
    TData tmp = *ptr1;
    *ptr1 = *ptr2;
    *ptr2 = tmp;
}



// shiftDown: Сдвигает i-й элемент кучи (при её отображении на массив) вниз,
// до тех пор пока всё поддерево (корнем которого является этот элемент) не
// будет удовлетворять свойству неубывающей/невозрастающей пирамиды.
static void shiftDown( BnHeap* H, unsigned i )
{
    unsigned parent = i;                // Текущий родитель.
    unsigned current;                   // Наименьший из сыновей родителя.
    unsigned left   = LEFT(parent);     // Текущий левый сын parent.
    unsigned right  = RIGHT(parent);    // Текущий правый сын parent.


    /*
     * Если куча неубывающая, то выбираем минимальный из сыновей,
     * так как родитель ложен быть меньше или равен любого из свих сыновей.
     */

    current = (right < H->n && H->top[right].data.key R H->top[left].data.key)
              ? right : left;

    // Пока не вышли за границу.
    while (current < H->n) {
        if (H->top[current].data.key R H->top[parent].data.key)
            // Если сын меньше родителя, то обмен информации узлов.
            swapData(&H->top[parent].data, &H->top[current].data);
        else
            return;

        // Текущий сын становиться текущим родителем.
        parent  = current;
        left    = LEFT(parent);
        right   = RIGHT(parent);
        // Выбирается новый сын.
        current = (right < H->n && H->top[right].data.key
                                   R H->top[left].data.key)
                  ? right : left;
    }
}



// shiftUp: Сдвигает i-й элемент кучи (при её отображении на массив) вверх
// по куче, до тех пор пока всё поддерево (корнем которого является этот
// элемент) не будет удовлетворять свойству неубывающей/невозрастающей
// пирамиды.
static void shiftUp( BnHeap* H, unsigned i )
{
    unsigned parent;


    // Пока i-й узел не стал корнем.
    while (i > 0) {
        parent = PARENT(i);
        if (H->top[i].data.key R H->top[parent].data.key)
            // Если i-й узел вместе со своим родителем нарушают свойство кучи,
            // то производится их обмен.
            swapData(&H->top[parent].data, &H->top[i].data);
        else
            return;
        i = parent;
    }
}



TData getMinimumBn( BnHeap* H )
{ 
    return H->top[0].data;
}



TData extractMinBn( BnHeap* H )
{
    if (H->n > 0) {
        // Если куча не пуста, то
        // 1) Сохраняем полнзную информацию минимального узла (корня)
        // во временной переменной;
        TData tmp = H->top[0].data;
        // 2) Перемещаем в корень последний элемент кучи;
        H->top[0] = H->top[--H->n];
        // 3) Сдвигаем этот корень вниз по куче.
        shiftDown(H, 0);
        return tmp;
    }
}



// reallocMem: Увеличивает вдвое допустимое количество элементов кучи H,
// сохраняя все текущие.
static BnNode* reallocMem( BnHeap* H )
{
    unsigned cSize = H->currentMax;
    unsigned newSize = H->currentMax * 2;
    BnNode* ptr1;
    BnNode* ptr2;

    ptr1 = H->top;
    H->top = ptr2 = (BnNode*) malloc(newSize * sizeof(BnNode));
    H->currentMax = newSize;
    while (cSize--) {
        *ptr2++ = *ptr1++;
    }
    return H->top;
}



void insertBn( BnHeap* H, TData data )
{
    if (H->n == H->currentMax)
        // Если массив, в котором хранится куча заполнен,
        // то необходимо перевыделить память и скопировать туда
        // содержимое текущего массива.
        reallocMem(H);

    H->top[H->n].data = data;
    // Сдвигаем новый элемент вверх по куче.
    shiftUp(H, H->n);
    H->n += 1;
}



BnHeap mergeBn( BnHeap* H1, BnHeap* H2 )
{
#if 1
    while (H2->n > 0)
        insertBn(H1, extractMinBn(H2));
    free(H2->top);
    return *H1;
#else
    BnHeap H = makeBnHeap(0);
#endif
}



void decreaseKeyBn( BnHeap* H, BnNode* x, TKey k )
{
    if (k < x->data.key) {
        // Если новое значение ключа меньше старого,
        // то обновляем значение ключевого поля в узле x ...
        x->data.key = k;
        // ... и сдвигаем его вверх, с помощью процедуры shiftUp.
        shiftUp(H, x - H->top);
    }
}



void deleteBn( BnHeap* H, BnNode* x )
{
    // Находим индекс элемента x в массиве H->top.
    unsigned long i = x - H->top;
    // Записываем последний элемент кучи вместо i-го элемента.
    H->top[i] = H->top[--H->n];
    // Сдвигаем его вниз, с помощью процедуры shiftDown.
    shiftDown(H, i);
}


// ДОПОЛНИТЕЛЬНЫЕ ОПЕРАЦИИ ВЗАИМОДЕЙСТВИЯ
// ==========================================================================


unsigned sizeBn( BnHeap* H )
{ return H->n; }


// ОБХОДЫ
// ==========================================================================


static void recPreOrderBn( BnHeap* H, unsigned i, void (*f)( const BnNode* ))
{
    if (i < H->n)
    {
        // Применяем функцию f к текущему i-му узлу.
        f(H->top + i);
        // Переходим к левому поддереву.
        recPreOrderBn(H, LEFT(i), f);
        // Переходим к правому поддереву.
        recPreOrderBn(H, RIGHT(i), f);
    }
}



static void recPostOrderBn( BnHeap* H, unsigned i, void (*f)( const BnNode* ))
{
    if (i < H->n)
    {
        // Переходим к левому поддереву.
        recPostOrderBn(H, LEFT(i), f);
        // Переходим к правому поддереву.
        recPostOrderBn(H, RIGHT(i), f);
        // Применяем функцию f к текущему i-му узлу.
        f(H->top + i);
    }
}



void preOrderBn( BnHeap* H, void (* f)( const BnNode* ))
{
    // Обрабатываем дерево кучи от корня.
    recPreOrderBn(H, 0, f);
}



void postOrderBn( BnHeap* H, void (* f)( const BnNode* ))
{
    // Обрабатываем дерево кучи от корня.
    recPostOrderBn(H, 0, f);
}
