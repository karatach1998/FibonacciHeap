#include "BinomialHeap.h"
#include <stdlib.h>





BlHeap makeBlHeap( void )
{
    BlHeap H;       // Создание кучи.
    H.head = NULL;  // Инициализация поля списка корней.
    H.n = 0;        // Установка количества узлов в ноль.
    return H;       // Возврат результата.
}



void deleteBlHeap( BlHeap* H )
{
    postOrderBl(H, &free);
}


int isEmptyBl( BlHeap* H )
{ return H->head == NULL; }



BlNode* getMinimumBl( BlHeap* H )
{
    BlNode* minNode = H->head;  // Указатель на минимальный узел кучи H.
    BlNode* current;            // Указатель на текущий обрабатываемый узел.


    if (minNode == NULL)
        // Если список корней кучи H -- пуст, то работа функции окончена.
        return NULL;

    // Поиск минимального узла в с. к. кучи H.
    for (current = minNode->sibling;
         current != NULL;
         current = current->sibling)
    {
        if (current->key < minNode->key)
            // Если текущий узел меньше текущего минимального,
            // то он становится минимальным.
            minNode = current;
    }

    // Возврат минимального.
    return minNode;
}


#if 01

#define TESTING

unsigned operationCompare, operationAssignment;
#define c operationCompare
#define a operationAssignment

#endif


// linkBl: Выполняет связывание узлов P и C, делая C дочерним для P.
// Возвращает указатель на корневой узел.
static inline BlNode* 
linkBl( BlNode* restrict C, BlNode* restrict P )
{
    /*
     * C -- child
     * P -- parent
     */
//    a+=4;
    C->sibling = P->child;  // Соседом С становиться сын P.
    C->parent = P;          // Отцем C становиться P.
    P->child = C;           // Сыном P становиться C.
    P->degree += 1;         // Увеличение степени родителя.
    return P;               // Возврат P.
}




#if 01

BlHeap mergeBl( BlHeap* H1, BlHeap* H2 )
{
    BlHeap H;

    if (H1->head == NULL) {
        ++c;
        ++a;
        H.head = H2->head;
    }
    else if (H2->head == NULL) {
        c += 2;
        ++a;
        H.head = H1->head;
    }
    else {

        BlNode* curH;
        BlNode* curH1 = H1->head;
        BlNode* curH2 = H2->head;


        c += 3;
        a += 5;
        if (curH1->degree < curH2->degree) {
            H.head = curH = curH1;
            curH1 = curH1->sibling;
        } else {
            H.head = curH = curH2;
            curH2 = curH2->sibling;
        }

        c += 3;
        while (curH1 != NULL && curH2 != NULL) {
            c += 4;
            a += 3;
            if (curH1->degree < curH2->degree) {
                curH = curH->sibling = curH1;
                curH1 = curH1->sibling;
            } else {
                curH = curH->sibling = curH2;
                curH2 = curH2->sibling;
            }
        }

        ++c;
        if (curH1 == NULL) {
            ++a;
            curH->sibling = curH2;
        }
        ++c;
        if (curH2 == NULL) {
            ++a;
            curH->sibling = curH1;
        }

        a += 3;
        curH = H.head;
        BlNode *prev = NULL, *next = curH->sibling;

        c += 3;
        while (next != NULL && curH->degree == next->degree) {
            c += 4;
            a += 3;
            if (curH->key >= next->key) {
                H.head = curH = linkBl(curH, next);
                next = next->sibling;
            } else {
                next = next->sibling;
                curH = linkBl(curH->sibling, curH);
                curH->sibling = next;
            }
        }


        ++c;
        while (next != NULL) {
            c += 2;
            a += 3;
            if (curH->degree == next->degree) {
                ++c;
                if (curH->key >= next->key) {
                    prev->sibling = curH = linkBl(curH, next);
                    next = next->sibling;
                } else {
                    next = next->sibling;
                    curH = linkBl(curH->sibling, curH);
                    curH->sibling = next;
                }
            } else {
                prev = curH;
                curH = next;
                next = next->sibling;
            }
        }
    }

    ++a;
    H.n = H1->n + H2->n;

    /*if (H1->head == NULL)
        H.head = H2->head;
    else if (H2->head == NULL)
        H.head = H1->head;
    else {
        BlNode* curH;
        BlNode* curH1 = H1->head;
        BlNode* curH2 = H2->head;

        if (curH1->degree < curH2->degree) {
            H.head = curH = curH1;
            curH1 = curH1->sibling;
        } else {
            H.head = curH = curH2;
            curH2 = curH2->sibling;
        }

        while (curH1 != NULL && curH2 != NULL)
            if (curH1->degree < curH2->degree) {
                curH->sibling = curH1;
                curH = curH1;
                curH1 = curH1->sibling;
            } else {
                curH->sibling = curH2;
                curH = curH2;
                curH2 = curH2->sibling;
            }

        if (curH1 == NULL)
            curH->sibling = curH2;
        if (curH2 == NULL)
            curH->sibling = curH1;

        curH = H.head;
        BlNode *prev = NULL, *next = curH->sibling;
        while (next != NULL && curH->degree == next->degree) {
            if (curH->key >= next->key) {
                H.head = curH = linkBl(curH, next);
                next = next->sibling;
            } else {
                next = next->sibling;
                curH = linkBl(curH, curH->sibling);
                curH->sibling = next;
            }
        }


        while (next != NULL) {
            if (curH->degree == next->degree) {
                if (curH->key >= next->key) {
                    prev = prev->sibling = curH = linkBl(curH, next);
                    next = next->sibling;
                } else {
                    next = next->sibling;
                    prev = prev->sibling = curH = linkBl(next, curH);
                    curH->sibling = next;
                }
            } else {
                prev = curH;
                curH = next;
                next = next->sibling;
            }
        }
    }

    H.n = H1->n + H2->n;*/
    return H;
}

#else
BlHeap mergeBl( BlHeap* H1, BlHeap* H2 )
{
#ifdef TESTING
    BlHeap H;


    if (H1->head == NULL) {
        ++c;
        ++a;
        H.head = H2->head;
    }
    else if (H2->head == NULL) {
        c += 2;
        ++a;
        H.head = H1->head;
    }
    else {

        BlNode* curF;
        BlNode* curS;
        BlNode* prev = NULL; ++a;
        BlNode* tmp;


        c += 3;
        a += 3;
        if (H1->head->degree < H2->head->degree) {
            H.head = curF = H1->head;
            curS = H2->head;
        } else {
            H.head = curF = H2->head;
            curS = H1->head;
        }

        c += 3;
        while (curF != NULL && curS != NULL) {
            prev = curF; ++a;
            c += 4;
            if (curF->degree >= curS->degree) {
                a += 4;
                tmp = curS->sibling;
                curS->sibling = curF->sibling;
                prev->sibling = curS;
                curS = tmp;
            } else {
                ++a;
                curF = curF->sibling;
            }
        }

        ++c;
        if (curS != NULL) {
            ++a;
            prev->sibling = curS;
        }


        a += 2;
        curF = H.head;
        BlNode* next = curF->sibling;

        c += 3;
        while (next != NULL && curF->degree == next->degree) {

            c += 4;
            if (curF->key > next->key) {
                a += 3;
                H.head = curF = linkBl(curF, next);
                next = curF->sibling;
            } else {
                a += 2;
                next = next->sibling;
                linkBl(curF->sibling, curF)->sibling = next;
            }
        }

        ++c;
        while (next != NULL) {
            c += 2;
            if (curF->degree == next->key) {
                ++c;
                if (curF->key > next->key) {
                    a += 3;
                    prev->sibling = curF = linkBl(curF, next);
                    next = curF->sibling;
                } else {
                    a += 2;
                    next = next->sibling;
                    linkBl(curF->sibling, curF)->sibling = next;
                }
            }
            else {
                a += 3;
                prev = curF;
                curF = next;
                next = next->sibling;
            }
        }
    }

    a += 3;
    H.n = H1->n + H2->n;

    return H;
#else
    BlHeap H;     // Итоговая куча.

    // Если список корней кучи H1 пуст, то работа функции окончена.
    if (H1->head == NULL)
        H.head = H2->head;
    // Если список корней кучи H2 пуст, то работа функции окончена.
    else if (H2->head == NULL)
        H.head = H1->head;
    else {
        /*
         * Если обе кучи непусты, то выберем из них ту,
         * первый элемент в списке корней которой имеет наименьшую
         * степень в качестве основной (первичной),
         * а другую в качестве побочной (вторичной).
         */

        BlNode* curF;           // Текущий корень в с. к. основной кучи.
        BlNode* curS;           // Текущий корень в с. к. вторичной кучи.
        BlNode* prev = NULL;    // Указатель на предыдущий узел списка.
        BlNode* tmp;            // Служебный указатель на узел.

        if (H1->head->degree < H2->head->degree) {
            // Если степень первого узла в с. к. кучи H1 меньше,
            // то выбрать её в качестве основной;
            H.head = curF = H1->head;
            // а вторую в качестве побочной.
            curS = H2->head;
        } else {
            // Иначе основной станет куча H2,
            H.head = curF = H2->head;
            // а H1 побочной.
            curS = H1->head;
        }


        // ::1 Формирование итогового списка корней.
        // Пока обе кучи не пусты.
        while (curF != NULL && curS != NULL) {
            // Сохраняем указатель на предыдущий.
            prev = curF;
            if (curF->degree >= curS->degree) {
                // Если степень текущего узла основной кучи не
                // превышает степени текущего узла побочной кучи,
                // то необходимо:

                // 1. Сохранить указатель на следующий узел
                // вторичной кучи.
                tmp = curS->sibling;

                // 2. Произвести вставку узла curS в односвязный
                // список между узлами prev и curF.
                curS->sibling = curF->sibling;
                prev->sibling = curS;

                // 3. Установить укаатель вторичной кучи на
                // сохраненный следующий элемент.
                curS = tmp;
            } else
                // Иначе переместить указатель основной кучи
                // на следующий в списке элемент.
                curF = curF->sibling;
        }

        // Если во вторичной куче остались элементы, то необходимо
        // добавить их в конец с. к. итоговой кучи.
        if (curS != NULL)
            prev->sibling = curS;


        // ::2 Объединение корней с одинаковыми степенями.
        curF = H.head;  // Установка текущего указателя в начало
                        // списка корней итоговой кучи.

        // Указатель на узел следующий за текущим.
        BlNode* next = curF->sibling;

        // Слияние певых узлов итогового с. к.
        while (next != NULL && curF->degree == next->degree)
            // Пока степени текущего и последующего узлов совпадают.

            if (curF->key > next->key) {
                // Если ключ текущего узла больше ключа следующего,
                // то он становиться сыном последнего.

                // Значение в дескрипторе тоже должно меняться.
                H.head = curF = linkBl(curF, next);

                // Переход к следующему узлу.
                next = curF->sibling;
            } else {
                // Иначе наоборот: следующий узел попадает в сыновья к текущему.

                // Сохранение указателя на следующего
                // за текущим узла.
                tmp = next->sibling;

                // Связывание следующего и текущего узлов.
                next = linkBl(next, curF)->sibling = tmp;

                // Восстановление указателя на следующего из
                // сохраненного значения.
                next = tmp;
            }

        // Окончательное связыание оставшихся узлов.
        while (next != NULL) {
            // Пока указатель на текущий узел не стал указывать
            // на последний в списке.

            if (curF->degree == next->key)
            // Если степени текущего и следующего узлов совпали, то:
                if (curF->key < next->key) {
                    // Если ключ первого меньше, то:

                    // Сохранить указатель на следующий
                    // за next элемент.
                    tmp = next->sibling;

                    // Текущим узлом станет корневой узел.
                    curF = linkBl(next, curF);

                    // Восстановление указателя на следующего из
                    // сохраненного значения.
                    curF->sibling = next = tmp;
                } else {
                    // Иначе первый должен стать сыном второго,
                    prev->sibling = curF = linkBl(curF, next);
                    // а следущий будет указывать на следующий
                    // за текущим.
                    next = curF->sibling;
                }
            else {
            // Иначе необходимо сместиться к следующей тройке узлов.

                prev = curF;            // Предыдущий <- текущий.
                curF = next;            // Текущий <- следующий.
                next = next->sibling;   // Следующий <- следующий
                                        // за следующим.
            }
        }
    }

    // Вычисление общего количества узлов в итоговой куче.
    H.n = H1->n + H2->n;

    // Возврат дескриптора полученной биномиальной кучи.
    return H;

#endif
}
#endif


void insertBl( BlHeap* H, TBase k )
{
    BlHeap _H;  // Создание дескриптора биномиальной кучи.

    // Выделение в куче памяти под узел.
    _H.head            = malloc(sizeof(BlNode));
    _H.head->key       = k;    // Ининциализация ключа.
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
        // Если у узла x нет дочерних, то ффункция возвращает NULL.
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

    // Возврат первого узла нового списка.
    return current;
}



TBase extractMinBl( BlHeap* H )
{
    if (H->head == NULL)
        // Если куча пуста, то вернуть пустой указатель.
        return NULL;
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
            if (current->key < minNode->key) {
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
        TBase tmp = minNode->key;

        // И освобождение памяти, занимаемой узлом.
        free(minNode);
        return tmp; // Возврат минимального узла.
    }
}



// swapKey: Обмен значений ключе узлов x и y.
static void swapKey( BlNode* x, BlNode* y )
{
    TBase tmp = x->key;
    x->key = y->key;
    y->key = tmp;
}



void decreaseKeyBl( BlHeap* H, BlNode* x, TBase k )
{
    // Если ключ узла не превышает текущего его значения.
    if (k <= x->key)
    {
        // Записываем в узел x значение k.
        x->key = k;
        // Создание и инициализация указателя на родителя x.
        BlNode* y = x->parent;
        // Пока x не корень и наруено свойство неубывающей кучи.
        while (y != NULL && x->key < y->key) {
            // Обмен значениями ключей узлов x и y.
            swapKey(x, y);
            x = y;          // x теперь указывает на своего родителя,
            y = y->parent; // а y -- на своего.
        }
    } else
        ; // ERROR TOO BIG KEY
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
        x->key = y->key;

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


// ОБХОДЫ
//===================================================================


void recPreOrderBl( BlNode* pNode, void (* f)( BlNode* ))
{
    BlNode* child = pNode->child;

    f(pNode);
    while (child != NULL) {
        recPreOrderBl(child, f);
        child = child->sibling;
    }
}


void recPostOrderBl( BlNode* pNode, void (* f)( BlNode* ))
{
    BlNode* child = pNode->child;

    while (child != NULL) {
        recPostOrderBl(child, f);
        child = child->sibling;
    }
    f(pNode);
}


void preOrderBl( BlHeap* H, void (* f)( BlNode* ))
{
    if (H->head != NULL)
        recPreOrderBl(H->head, f);
}


void postOrderBl( BlHeap* H, void (* f)( BlNode* ))
{
    if (H->head != NULL)
        recPostOrderBl(H->head, f);
}