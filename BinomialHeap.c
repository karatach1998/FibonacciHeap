/*
 * Для сокращения письма будем иногда писать с. к. вместо список корней.
 */

#include "BinomialHeap.h"
#include <stdlib.h>



// СОЗДАНИЕ И УНИЧТОЖЕНИЕ
// ==========================================================================

BlHeap makeBlHeap( void )
{
    BlHeap H;       // Создание кучи.
    H.head = NULL;  // Инициализация поля списка корней.
    H.n = 0;        // Установка количества узлов в ноль.
    return H;       // Возврат результата.
}



// recDoneBlTree: Выделяет из pNode поддерево, если оно состоит из одного
// узла, то освобождает занимаемую им память.
static void recDoneBlTree( BlNode* pNode )
{

    if (pNode->child == NULL)
        // Если дерево является листом, то удаляем его из динамической памяти.
        free(pNode);
    else {
        // Иначе сохраняем текущего сына.
        BlNode* child = pNode->child;
        // Уменьшаем дерево Bk до Bk-1.
        pNode->child = child->sibling;

        // Обратный обход двух поддеревьев.
        recDoneBlTree(child);
        recDoneBlTree(pNode);
    }
}



void doneBlHeap( BlHeap* H )
{
    BlNode* current = H->head;  // Текущий узел в списке корней.
    BlNode* next;               // Указатель на следующий за текущим узел.


    // Пока не конец списка.
    while (current != NULL) {
        // Сохраняем адрес следуюшего за текущим корня.
        next = current->sibling;
        // Выполняем обратный обход с удаление.
        recDoneBlTree(current);
        // Сдвигаем текущий указатель к следующему в с. к. узлу.
        current = next;
    }
}


// ОПЕРАЦИЯ ПРЕДИКАТ
// ==========================================================================


int isEmptyBl( const BlHeap* H )
{ return H->head == NULL; }


// ОСНОВНЫЕ ОПЕРАЦИИ ВЗАИМОДЕЙСТВИЯ
// ==========================================================================


TData getMinimumBl( const BlHeap* H )
{
    if (H->head != NULL) {
        BlNode* minNode = H->head;// Указатель на минимальный узел кучи H.
        BlNode* current;          // Указатель на текущий обрабатываемый узел.


        // Поиск минимального узла в с. к. кучи H.
        for (current = minNode->sibling;
             current != NULL;
             current = current->sibling) {
            if (current->data.key < minNode->data.key)
                // Если текущий узел меньше текущего минимального,
                // то он становится минимальным.
                minNode = current;
        }

        // Возврат информационной части минимального элемента.
        return minNode->data;
    }
}



// linkBl: Выполняет связывание узлов P и C, делая C дочерним для P.
// Возвращает указатель на корневой узел.
static inline BlNode* 
linkBl( BlNode* restrict C, BlNode* restrict P )
{
    /*
     * C -- child
     * P -- parent
     */

    C->sibling = P->child;  // Соседом С становиться сын P.
    C->parent = P;          // Отцем C становиться P.
    P->child = C;           // Сыном P становиться C.
    P->degree += 1;         // Увеличение степени родителя.
    return P;               // Возврат P.
}



BlHeap mergeBl( BlHeap* H1, BlHeap* H2 )
{
    BlHeap H;

    if (H1->head == NULL)
        // Если первая куча пуста, то с. к. итоговой кучи будет состоять из
        // элементов списка корней второй кучи.
        H.head = H2->head;
    else if (H2->head == NULL)
        // Аналогично для второй кучи.
        H.head = H1->head;
    else {

        BlNode* curH;             // Текущий узел с. к. итоговой кучи.
        BlNode* curH1 = H1->head; // Текущий обрабатыаемый узел с. к. кучи H1.
        BlNode* curH2 = H2->head; // Текущий обрабатыаемый узел с. к. кучи H2.


        // Необходимо найти первый узел в итоговой куче.
        if (curH1->degree < curH2->degree) {
            // Если узел curH1 имеет меньшую степень, чем curH2,
            // то список корней итоговой кучи будет начинаться именно с него.
            H.head = curH = curH1;
            // Сдвиг к следующему узлу в списке корней.
            curH1 = curH1->sibling;
        } else {
            // Аналогично в обратной ситуации.
            H.head = curH = curH2;
            curH2 = curH2->sibling;
        }


        // Пока в обоих списках корней остались нераспределенные элементы,
        // выполним следущие действия.
        while (curH1 != NULL && curH2 != NULL)
            if (curH1->degree < curH2->degree) {
                // Если степень узла curH1 меньше степени узла curH2,
                // то добавляем его в список корней итоговой кучи.
                curH = curH->sibling = curH1;
                // Сдвигаем curH1 к следущющему узлу списка.
                curH1 = curH1->sibling;
            } else {
                // В противном случае поступаем аналогичным образом
                // со списком curH2.
                curH = curH->sibling = curH2;
                curH2 = curH2->sibling;
            }


        // Если в списке корней curH1 остались элементы,
        if (curH1 != NULL)
            // то добавляем их к итоговому списку корней.
            curH->sibling = curH1;

        // Если в списке корней curH2 остались элементы,
        if (curH2 != NULL)
            // то добавляем их к итоговому списку корней.
            curH->sibling = curH2;


        curH = H.head; // Устанавливаем указатель на начало списка корней.
        BlNode* prev = NULL;            // Указатель на предыдущий узел.
        BlNode* next = curH->sibling;   // Указатель на следущий узел.

        // Обрабатываем узлы с одинаковыми степенями,
        // расположенные в начале списка.
        while (next != NULL && curH->degree == next->degree)
            if (curH->data.key >= next->data.key) {
                // Если ключ текущего элемента больше ключа последущего,
                // то текущий узел становиться сыном последущего.
                // Это также фиксируется в дескрипторе.
                H.head = curH = linkBl(curH, next);

                // Сдвигаем указатель на следующий элемент.
                next = next->sibling;
            } else {
                // В протисном случае сохраняем указатель на элемент,
                // расположенный за текущей рассматриваемой парой
                // соседних элементов..
                next = next->sibling;
                // Сосед текущего становиться его сином.
                curH = linkBl(curH->sibling, curH);
                // Восстанавливаем корректность списка.
                curH->sibling = next;
            }


        // Далее пока не конец списка.
        while (next != NULL)
            if (curH->degree == next->degree)
                // Если степени узлов равны, то:
                if (curH->data.key >= next->data.key) {
                    // Если ключ текущего узла превышает значение ключа
                    // последующего, то текущий становится сыном последующего.
                    prev->sibling = curH = linkBl(curH, next);

                    // Сдвигаем указатель на следующий элемент.
                    next = next->sibling;
                } else {
                    // В протисном случае сохраняем указатель на элемент,
                    // расположенный за текущей рассматриваемой парой
                    // соседних элементов..
                    next = next->sibling;
                    // Сосед текущего становиться его сином.
                    curH = linkBl(curH->sibling, curH);
                    // Восстанавливаем корректность списка.
                    curH->sibling = next;
                }
            else {
                // Иначе продвигаемся вперед по списку.
                prev = curH;
                curH = next;
                next = next->sibling;
            }
    }

    // Вычисление общего количества элементов в обоих кучах.
    H.n = H1->n + H2->n;

    // Возврат дескриптора итоговой кучи.
    return H;
}



void insertBl( BlHeap* H, TData data )
{
    BlHeap _H;  // Создание дескриптора биномиальной кучи.

    // Выделение в куче памяти под узел.
    _H.head            = malloc(sizeof(BlNode));
    _H.head->data      = data; // Ининциализация ключа.
    _H.head->parent    = NULL; // У узла нет родителей.
    _H.head->child     = NULL; // У узла нет сыновей.
    _H.head->sibling   = NULL; // У узла нет соседей.
    _H.head->degree    = 0;    // Степень узла равна нулю.
    _H.n               = 1;    // Количество узлов в куче = 1.


    // Выполняем слияние созданной одноэлеемнтной кучи
    // и исходной кучи H.
    *H = mergeBl(H, &_H);
}



// toRootList: Возвращает указатель на список корней, полученный из дочерних
// элементов узла x путем обращения их порядка и удаления родителя.
static BlNode* toRootList( BlNode* x )
{
    BlNode* previous;   // Указатель на предшествующий в списке. узел.
    BlNode* current;    // Указатель на текущий узел.
    BlNode* next;       // Указатель на следующий в списке узел.


    if (x->child == NULL)
        // Если у узла x нет дочерних, то функция возвращает NULL.
        return NULL;

    for (previous = NULL, current = x->child, next = current->sibling;
         next != NULL;
         previous = current, current = next, next = next->sibling)
    {
        // Кажый узел из списка (кроме последнего) связаять с предыдущим.
        current->sibling = previous;

        // И каждый узел списка сделаем корневым.
        current->parent = NULL;
    }

    // Связывание оставшегося узла с предыдущим
    current->sibling = previous;

    // и удаление у него родителя.
    current->parent = NULL;

    // Возвращаем первый узел нового списка.
    return current;
}



TData extractMinBl( BlHeap* H )
{
    if (H->head == NULL)
        // Если куча пуста, то вернуть нулевые данные.
        return (TData) {0};
    else {
        BlNode* minNode = H->head;// Указатель на минимальный узел кучи.
        BlNode* minPrev = NULL;   // Указатель на узел,
                                  // предществующий минимальному.
        BlNode* current;          // Указатель на текущий обрабатываемый узел.
        BlNode* previous;         // Указатель на предыдущий узел.


        // Поиск минимального узла в списке корней кучи.
        for (previous = minNode, current = minNode->sibling;
             current != NULL;
             previous = current, current = current->sibling) {
            if (current->data.key < minNode->data.key) {
                // Если ключ текущего узла меньше ключа текущего минимального,
                // то необходимо сохранить указатель на него и узел,
                // стоящий перед ним в списке.
                minNode = current;
                minPrev = previous;
            }
        }


        if (minPrev == NULL)
            // Если минимальный корень расположен в начале списка,
            // то для его удаления из списка еорней нужно сдвинуть указатель
            // на начало списка на слудющий за минимальным узел.
            H->head = minNode->sibling;
        else
            // Иначе в куче определенно есть еще корневые узлы,
            // по крайней мере один и его необходимо связать
            // с соседом минимального элемента.
            minPrev->sibling = minNode->sibling;


        if (minNode->child != NULL)
            // Если у минимального узла есть дочерние узлы, то необходимо
            // выполнить слияние списка корней исходной кучи с дочерними
            // узлами минимального элемента.
            *H = mergeBl(H, &(BlHeap) {toRootList(minNode), 0});


        H->n -= 1;      // Уменьшаем количество узлов в исходной куче.

        // Сохраняем полезную информацию, хранящуюся в узле.
        TData tmp = minNode->data;

        // И освобождаем память, занимаемую узлом.
        free(minNode);
        return tmp; // Возврат данных минимального узла.
    }
}



// swapKey: Обмен значений ключей узлов x и y.
static void swapKey( BlNode* x, BlNode* y )
{
    TData tmp = x->data;
    x->data = y->data;
    y->data = tmp;
}



void decreaseKeyBl( BlHeap* H, BlNode* x, TKey k )
{
    // Если ключ узла не превышает текущего его значения.
    if (k <= x->data.key)
    {
        // Записываем в узел x значение k.
        x->data.key = k;
        // Создание и инициализация указателя на родителя x.
        BlNode* y = x->parent;
        // Пока x не корень и наруено свойство неубывающей кучи.
        while (y != NULL && x->data.key < y->data.key) {
            // Обмен значениями ключей узлов x и y.
            swapKey(x, y);
            x = y;          // x теперь указывает на своего родителя,
            y = y->parent; // а y -- на своего.
        }
    }
}



void deleteBl( BlHeap* H, BlNode* x )
{
    /*
     * Для удаления узла x из кучи H необходимо:
     * 1) Сдвинуть этот узел x в корень дерева, в котором он находится.
     * 2) Удалить данный корень и выполнить слияние исходной кучи с кучей,
     *    полученной из списка дочених элементов удаляемого узла.
     */

    BlNode* y = x->parent; // Указатель на родителя.


    // Пока x -- не корень.
    while (y != NULL) {
        // Сдвинуть значение ключа y в ключ x;
        x->data = y->data;

        // И перейти на уровень выше.
        x = y;
        y = y->parent;
    }


    // Теперь x корневой узел дерева, который нужно извлечь.
    if (H->head == x)
        // Если удаляется узел первого поддерева (x - его корень),
        // то выполнить
        H->head = x->sibling;
    else {
        // Указатель на узел, который предшествует удаляемому в списке корней.
        BlNode* previous = H->head;

        // Поиск в с. к. узла, который предшествует удаляемому.
        while (previous->sibling != x)
            previous = previous->sibling; // Переход к следующему кандидату.

        // Исключаем x из списка корней.
        previous->sibling = x->sibling;
    }

    // Слияние исходной кучи H и кучи, полученной из списка дочерних узлов
    // удаляемого корневого узла.
    *H = mergeBl(H, &(BlHeap) {toRootList(x), 0});

    // Освобождение памяти, занимаемой узлом x.
    free(x);
}


// ДОПОЛНИТЕЛЬНЫЕ ОПЕРАЦИИ ВЗАИМОДЕЙСТВИЯ
// ==========================================================================


unsigned sizeBl( BlHeap* H )
{ return H->n; }


// ОБХОДЫ
// ==========================================================================


// recPreOrderBl: Выполняет рекурсивный прямой обход текущего поддерева,
// где pNode его корен, а child - левый сын.
// Применяет к каждому узлу функцию f.
static void recPreOrderBl( BlNode* pNode, BlNode* child,
                           void (* f)( const BlNode* ))
{
    if (child == NULL)
        // Если поддерево состоит из одного узла,
        // то применяем к нему функцию f.
        f(pNode);
    else {
        // Иначе выделяем в нем поддеревья.
        recPreOrderBl(pNode, child->sibling, f);
        recPreOrderBl(child, child->child, f);
    }
}



// recPostOrderBl: Выполняет рекурсивный обратный обход текущего поддерева,
// где pNode его корен, а child - левый сын.
// Применяет к каждому узлу функцию f.
static void recPostOrderBl( BlNode* pNode, BlNode* child,
                            void (* f)( const BlNode* ))
{
    if (child == NULL)
        // Если поддерево состоит из одного узла,
        // то применяем к нему функцию f.
        f(pNode);
    else {
        // Иначе выделяем в нем поддеревья.
        recPreOrderBl(child, child->child, f);
        recPreOrderBl(pNode, child->sibling, f);
    }
}



void preOrderBl( BlHeap* H, void (* f)( const BlNode* ))
{
    BlNode* current = H->head;  // Текущий узел в списке корней.

    // Проходимся по списку корней и обрабатываем деревья.
    while (current != NULL) {
        recPreOrderBl(current, current->child, f);
        current = current->sibling;
    }
}



void postOrderBl( BlHeap* H, void (* f)( const BlNode* ))
{
    BlNode* current = H->head;  // Текущий узел в списке корней.

    // Проходимся по списку корней и обрабатываем деревья.
    while (current != NULL) {
        recPostOrderBl(current, current->child, f);
        current = current->sibling;
    }
}