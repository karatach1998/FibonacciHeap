/*
 * Для сокращения письма будем иногда писать с. к. вместо список корней.
 */

#include <math.h>
#include "FibonacciHeap.h"
#include <stdlib.h>


enum {FALSE, TRUE};


// СОЗДАНИЕ И УНИЧТОЖЕНИЕ
// ==========================================================================


FbHeap makeFbHeap( void )
{
    FbHeap H;       // Создание дескриптора новой кучи.
    H.min = NULL;   // Инициализация адреса минимального элемента.
    H.n   = 0;      // Инициализация текущего количества узлов кучи нулем.
    return H;       // Возврат проинициализированного дескриптора.
}



// recDoneFbTree: Выделяет из pNode поддерево, если оно состоит из одного
// узла, то освобождает занимаемую им память. Дерево удаляется сверху вниз.
static void recListDelete( FbNode* pNode, FbNode* child )
{
    if (child == NULL)
        // Если дерево является листом, то удаляем его из динамической памяти.
        free(pNode);
    else {
        // Иначе сохраняем текущего сына.
        child->left->right = NULL;

        // Обратный прямой двух поддеревьев.
        recListDelete(pNode, child->right);
        recListDelete(child, child->child);
    }
}



void doneFbHeap( FbHeap* H )
{
    if (H->min != NULL) {
        FbNode* current = H->min;   // Текущий узел в списке корней.
        FbNode* next;               // Указатель на следующий за текущим узел.

        // Обозначаем опинак конца списка.
        current->left->right = NULL;
        while (current != NULL) {
            // Сохраняем адрес следуюшего за текущим корня.
            next = current->right;
            // Выполняем обратный обход с удаление.
            recListDelete(current, current->child);
            // Сдвигаем текущий указатель к следующему в с. к. узлу.
            current = next;
        }
    }
}


// ОПЕРАЦИЯ ПРЕДИКАТ
// ==========================================================================


int isEmptyFb( const FbHeap* H )
{ return H->min == NULL; }


// ОСНОВНЫЕ ОПЕРАЦИИ ВЗАИМОДЕЙСТВИЯ
// ==========================================================================


// swapPtr: Обмен указателей на нетипизированные указатели.
static inline void swapPtr( void** restrict p1, void** restrict p2 )
{
    void* tmp = *p1;
    *p1 = *p2;
    *p2 = tmp;
}



// insertLeft: Вставляет узел pNode в дусвязный циклический список
// левее узла list. Предполагаются, что оба адреса не равны нулю.
static FbNode* insertLeft( FbNode* list, FbNode* pNode )
{
    pNode->left = list->left;
    list->left->right = pNode;
    pNode->right = list;
    list->left = pNode;
    return pNode;
}



void insertFb( FbHeap* H, TData data )
{
    // Выделяем память для нового узла.
    FbNode* pNode   = (FbNode*) malloc(sizeof(FbNode));
    pNode->data     = data;     // Инициализация информационной части.
    pNode->parent   = NULL;     // У нового узла нет родителя.
    pNode->child    = NULL;     // У нового узла нет детей.
    pNode->degree   = 0;        // 0, по той же причине.
    pNode->mark     = FALSE;    // Новый узел не помечен.


    if (H->min == NULL)
        // Если в куче еще нет элементом, то pNode будет первым.
        // Причем также он будет сам себе левым и правым соседом.
        H->min = pNode->left = pNode->right = pNode;
    else {
        // В противном случае выполним вставку узла pNode в кучу H.
        insertLeft(H->min, pNode);
        // Проверим актуальность минимального узла H.top.
        if (pNode->data.key < H->min->data.key)
            // Если ключ нового узла меньше ключа текущего минимального узла,
            // то обноаим указатель H.top.
            H->min = pNode;
    }
    H->n += 1;
}



TData getMinimumFb( FbHeap* H )
{
    if (H->min != NULL)
        // Если куча не пуста, то возвращаем
        // иныормационную часть минимального элемента.
        return H->min->data;
}



// concatList: Связывает два двусвязных списка,
// представленных элементами list1 и list2.
// Предпологается, что они не равны нулевому указателю.
void concatList( FbNode* list1, FbNode* list2 )
{
    swapPtr((void**) &list1->right,       (void**) &list2->right);
    swapPtr((void**) &list1->right->left, (void**) &list2->right->left);
}



FbHeap mergeFb( FbHeap* H1, FbHeap* H2 )
{
    if (H1->min == NULL)
        // Если первая куча пуста, то результатом слияния
        // двух куч будет только вторая.
        return *H2;
    else if(H2->min == NULL)
        // Если пуста вторая, то результатом слияния будет первая.
        return *H1;
    else {
        // Если же обе кучи не пусты, то создаем дескриптор итоговой кучи.
        FbHeap H;

        // Выполняем конкатенацию списков корней куч H1 и H2.
        concatList(H1->min, H2->min);
        // Определяем минимальный из двух минимальных узлов куч H1 и H2,
        // и присваиваем результат соответствующему полю кучи H.
        H.min = (H2->min->data.key < H1->min->data.key) ? H2->min : H1->min;
        // Находим общее количество узлов в итоговой куче.
        H.n = H1->n + H2->n;
        // Возврат дескриптора итоговой кучи.
        return H;
    }
}



// deleteLeft: Удаляет узел pNode из циклического двусвязного списка
// и возвращает указатель на ПРАВОГО его соседа или NULL,
// если этот узел был единственным. Предполагается,
// что pNode ненулевой указатель.
static FbNode* deleteLeft( FbNode* pNode )
{
    if (pNode == pNode->right)
        // Если узел pNode единственный в списке.
        return NULL;
    else {
        FbNode* sibling = pNode->right; // Правый сосед удаляемого узла.

        // Связываем левого соседда удаляемого узла с правым.
        pNode->left->right = sibling;
        // А правого -- с левым.
        sibling->left = pNode->left;
        // Возвращаем правого соседа.
        return sibling;
    }
}



static FbNode* linkFb( FbHeap* H, FbNode* y, FbNode* x )
{
    // 1) Удаляем y из списка корней H.
    // ## Этот пунт должен быть пропущен, так как в процессе
    // ## работы функции consolidate соседи узла могут быть потеряны.
    // deleteLeft(y);

    // 2) Делаем узел y дочерним узлом для x.
    if (x->child == NULL)
        x->child = y->left = y->right = y;
    else
        x->child = insertLeft(x->child, y);
    // Устанавливаем x в качестве родителя узла y.
    y->parent = x;
    // Увеличиваем степень узла.
    x->degree += 1;

    // 4) Снимаем пометку с узла y.
    y->mark = FALSE;
    return x;
}



static void consolidate( FbHeap* H )
{
    /*
     * Максимальная степень узла в куче из H->n узлов равна сумме чисел:
     * - логарифм числа узлов H->n по основанию FI (золотое сечение);
     * - +1 для степени 0;
     * - +1 для признака конца массива.
     */
    static const double FI = 1.61803398874989484820458;
    const unsigned maxDegree = (const unsigned) (log(H->n) / log(FI)) + 2;

    // Массив указателей на корни-кандидаты на объединение.
    FbNode* A[maxDegree];
    FbNode* begin;          // Указатель на начало списка корней.
    FbNode* end;            // Указатель на конец списка корней.
    FbNode *x = NULL, *y;   // Указатели на текущие узлы,
                            // участвующие в объединении.
    unsigned d;             // Степень текущего узла x.
    unsigned i;

    // Инициализация массива A.
    for (i = 0; i < maxDegree; ++i)
        A[i] = NULL;


    /*
     * Для прохождения по списку корней будут использоваться два указателя
     * begin и end на начало и на конец списка.
     * Первым членом последовательности является H.top,а последним левый
     * сосед узла H.top, следовательно именно на него изначально будет
     * указывать begin. end должен указывать на элемент следующий за посленим,
     * то есть на H.top.
     * Цикл выполняется до тех пор пока begin и end не сойдутся,
     * то бишь не станут равны. Однако они изначально равны. Для решения этой
     * проблемы переменной x было предваритеьно присвоено хзначение NULL.
     * Это позволит использовать её значение для разграничения первой
     * и последней итерации.
     */

    for (begin = end = H->min; begin != end || !x;)
    {
        x = begin;
        begin = begin->right;
        d = x->degree;
        while (A[d] != NULL)
        {
            // Если для текущей степени узла x есть несвязанные узлы такой же
            // степени, то y будет указывать именно на этого кандидата.
            y = A[d];
            if (x->data.key > y->data.key)
                // Если ключ текущего узла больше ключа второго кандидата,
                // то выполним обмен указателей x и y.
                swapPtr((void**) &x, (void**) &y);
            // Делаем y прямым сыном узла x и удаляется из с. к.
            linkFb(H, y, x);
            // Удаляем узел y из списка кандидатов.
            A[d] = NULL;
            // Повышаем текущую степень ассоциированную с узлом x.
            ++d;
        }
        A[d] = x;
    }


    // Делаем список корней итоговой кучи пустым.
    H->min = NULL;
    // Далее выполняется поиск корневых узлов в массиве A.
    // Все они объединяются в новый список корней и из них
    // выбирается минимальный.
    for (i = 0; i < maxDegree; ++i)
        if (A[i])
            // Если в массиве A найден необработанный элемент, то:
            if (H->min == NULL)
                // Если с. к. пока не содержит элементов, то происходит
                // формирование циклического двусвязного списка из элемента
                // A[i], и H.top начинает указывать на него.
                H->min = A[i]->left = A[i]->right = A[i];
            else {
                // В противном случае вставляем узел A[i] левее узла H.top.
                insertLeft(H->min, A[i]);
                // При необходимости обновляем указатель H.top.
                if (A[i]->data.key < H->min->data.key)
                    H->min = A[i];
            }
}



TData extractMinFb( FbHeap* H )
{
    FbNode* z = H->min; // Указатель на удаляемый минимальный узел.
    if (z != NULL) {

        // 1) Если у узла z есть дочерние узлы, то:
        //    - необходимо у каждого из них удалить родителя.
        //    - определить, кто из них является минимальным.
        if (z->child) {
            FbNode* iterator = z->child->right;  // Текущий узел в списке.
            while (iterator != z->child) {
                // Проходимся по списку дочерних для z узлов
                // и для каждого из них устанавливаем указатель
                // на родителя равным NULL.
                iterator->parent = NULL;

                // Сдвиг к следующему узлу.
                iterator = iterator->right;
            }
            // Выполним удаление и у прямого сына узла z.
            z->child->parent = NULL;

            // Добавляем дочение элементы узла z в с. к.,
            // если таковые имеются.
            concatList(H->min, z->child->left);
        }


        // 2) Обновляем с. к. итоговой кучи.
        if ((H->min = deleteLeft(z)) != NULL)
            // Если после удаления узла z куча не стала пустой,
            // выполняем уплотнение с. к. кучи H.
            consolidate(H);

        // Уменьшаем количество узлов в куче H.
        H->n -= 1;

        // 3) Обрабатываем иформационную часть узла z.
        // Сохраняем полезную информацию, записанную в узле z.
        TData tmp = z->data;
        // Освобождаем память, занимаемую узлом z.
        free(z);
        // Возвращаем сохраненные данные.
        return tmp;
    }
}



// cutFb: Вырезает узел x и списка дочерних узлов y
// и помещает его в список корней кучи H, снимая с него пометку.
// Предпологается, что H->min, x и y не равны нулевому адресу.
static void cutFb( FbHeap* H, FbNode* x, FbNode* y )
{
    // 1) Удаляем x из списка дочерних узлов y-ка.
    if (y->child == x)
        // Если x -- прямой сын y (тот, на которого указывает),
        // то удаляем его из списка сыновей и обновляем сына y-ка.
        y->child = deleteLeft(x);
    else
        deleteLeft(x);

    // Уменьшаем степень узла y.
    y->degree -= 1;

    // 2) Добавляем узел x в список корней кучи H.
    insertLeft(H->min, x);

    // 3) Делаем его корнем.
    x->parent = NULL;

    // 4) Снимаем с него пометку.
    x->mark = FALSE;
}



// cascadingCutFb: Обрабатывае родительский узел вырезанного узла:
// -- если удаленный узел был первым удаленным сыном, то помечает y.
// -- если вторым (узел уже помечен), то y рассматривается с как сын
//    своего родителя и переносится в с. к. и т. д.
static void cascadingCutFb( FbHeap* H, FbNode* y )
{
    FbNode* z = y->parent;
    if (z != NULL) {
        // Если y не корень, то:
        if (y->mark == FALSE)
            // Если он еще не помечен, то помечаем.
            y->mark = TRUE;
        else {
            // Если уже помечен, то пора вырезать его
            // из списка дочерних узлов ...
            cutFb(H, y, z);
            // ... и применить эту функцию к его родителю.
            cascadingCutFb(H, z);
        }
    }
}



void decreaseKeyFb( FbHeap* H, FbNode* x, TKey k )
{
    // Если новое значение ключа k не превышает текущее, то в
    // соответствующее поле ула записывается запошенное значение k.
    // В противном случае операция является недопустимой и работа функции
    // завершается.
    if (k < x->data.key) {
        // Записываем новое значение ключа.
        x->data.key = k;

        FbNode* y = x->parent; // Указатель на родителя.
        if (y != NULL && y->data.key > x->data.key) {
            // Если x - не корень и его ключ теперь меньше, чем у родителя, то:
            // 1) Вырезаем его из списка дочерних узлов y-ка
            // и помещаем его в список корней H.
            cutFb(H, x, y);

            // 2) Применяем к узлу y каскадное вырезание.
            cascadingCutFb(H, y);

            // 3) При необходимости обновляем минимальный элемент кучи H.
            if (x->data.key < H->min->data.key)
                H->min = x;
        }
    }
}



void deleteFb( FbHeap* H, FbNode* x )
{
    FbNode* y = x->parent;
    if (y != NULL) {
        cutFb(H, x, y);
        cascadingCutFb(H, y);
    }
    H->min = x;
    extractMinFb(H);
}



// ДОПОЛНИТЕЛЬНЫЕ ОПЕРАЦИИ ВЗАИМОДЕЙСТВИЯ
// ==========================================================================


unsigned sizeFb( const FbHeap* H )
{ return H->n; }


// ОБХОДЫ
// ==========================================================================


static void
recPreOrderFb( FbNode* pNode, FbNode* child, void (*f)( const FbNode* ))
{
    if (child == NULL)
        // Если поддерево состоит из одного узла,
        // то применяем к нему функцию f.
        f(pNode);
    else {
        // Иначе выделяем в нем поддеревья.
        child->left->right = NULL;

        recPreOrderFb(pNode, child->right, f);
        recPreOrderFb(child, child->child, f);

        // Восстанавиваем полноту связи.
        child->left->right = child;
    }
}



static void
recPostOrderFb( FbNode* pNode, FbNode* child, void (*f)( const FbNode* ))
{
    if (child == NULL)
        // Если поддерево состоит из одного узла,
        // то применяем к нему функцию f.
        f(pNode);
    else {
        // Иначе выделяем в нем поддеревья.
        child->left->right = NULL;

        recPostOrderFb(child, child->child, f);
        recPostOrderFb(pNode, child->right, f);

        // Восстанавиваем полноту связи.
        child->left->right = child;
    }
}



void preOrderFb( FbHeap* H, void (* f)( const FbNode* ))
{
    if (H->min != NULL)
    {
        FbNode *begin, *end;
        unsigned char isFirstIteration = TRUE;


        for (begin = end = H->min; begin != end || isFirstIteration;
                begin = begin->right, isFirstIteration = FALSE)
            // Обрабатываем дерево каждого корня из списка корней кучи H.
            recPreOrderFb(begin, begin->child, f);
    }
}



void postOrderFb( FbHeap* H, void (* f)( const FbNode* ))
{
    if (H->min != NULL)
    {
        FbNode *begin, *end;
        unsigned char isFirstIteration = TRUE;


        for (begin = end = H->min; begin != end || isFirstIteration;
             begin = begin->right, isFirstIteration = FALSE)
            // Обрабатываем дерево каждого корня из списка корней кучи H.
            recPostOrderFb(begin, begin->child, f);
    }
}