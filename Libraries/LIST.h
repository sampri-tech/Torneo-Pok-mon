#ifndef LIST_H
#define LIST_H

#include<stddef.h>

typedef struct NodeList NodeL;
typedef struct List List;

/**
 * @brief Crea una nueva lista vacía.
 *
 * @return List*: puntero a la lista creada, o NULL si falla la memoria.
 * @note El tamaño de elemento se establece con la primera inserción exitosa.
 */
List *NewList(void);

/**
 * @brief Crea una copia de una lista.
 *
 * @param list: lista a copiar.
 * @return List*: nueva lista con copia de los elementos, o NULL si list es NULL o falla la memoria.
 * @note La lista devuelta debe liberarse con DestroyList.
 */
List *CopyList(const List *list);

/**
 * @brief Concatena dos listas en una nueva lista.
 *
 * @param list1: primera lista.
 * @param list2: segunda lista.
 * @return List*: nueva lista con los elementos de list1 seguidos de list2, o NULL si ambas son NULL, si falla la memoria o si los tamaños de elemento son incompatibles.
 * @note La lista devuelta debe liberarse con DestroyList.
 */
List *ConcatList(const List *list1, const List *list2);

/**
 * @brief Busca los elementos que coinciden con un valor dado.
 *
 * @param list: lista donde se busca.
 * @param ele: elemento a comparar.
 * @param cmp: función de comparación; retorna distinto de 0 si hay coincidencia.
 * @return List*: nueva lista con copias de los elementos coincidentes, o NULL si los parámetros son inválidos o falla la memoria.
 * @note La lista devuelta debe liberarse con DestroyList.
 */
List *SearchElement(const List *list, void *ele, int (*cmp)(const void *, const void *));

/**
 * @brief Obtiene el primer nodo de la lista.
 *
 * @param list: lista consultada.
 * @return const NodeL*: nodo cabeza, o NULL si list es NULL o la lista está vacía.
 */
const NodeL *GetHead(const List *list);

/**
 * @brief Obtiene el último nodo de la lista.
 *
 * @param list: lista consultada.
 * @return const NodeL*: nodo cola, o NULL si list es NULL o la lista está vacía.
 */
const NodeL *GetTail(const List *list);

/**
 * @brief Obtiene el nodo anterior a uno dado.
 *
 * @param node: nodo consultado.
 * @return const NodeL*: nodo anterior, o NULL si node es NULL o no tiene anterior.
 */
const NodeL *GetPrev(const NodeL *node);

/**
 * @brief Obtiene el nodo siguiente a uno dado.
 *
 * @param node: nodo consultado.
 * @return const NodeL*: nodo siguiente, o NULL si node es NULL o no tiene siguiente.
 */
const NodeL *GetNext(const NodeL *node);

/**
 * @brief Obtiene la cantidad de elementos de la lista.
 *
 * @param list: lista consultada.
 * @return size_t: tamaño de la lista, o 0 si list es NULL.
 */
size_t GetLSize(const List *list);

/**
 * @brief Obtiene el elemento almacenado en un nodo.
 *
 * @param node: nodo consultado.
 * @return void*: puntero al elemento, o NULL si node es NULL.
 * @note El puntero pertenece al nodo; no debe liberarse directamente.
 */
void *GetElement(const NodeL *node);

/**
 * @brief Indica si una lista está vacía.
 *
 * @param list: lista consultada.
 * @return int: 1 si está vacía o list es NULL, 0 en caso contrario.
 */
int IsLEmpty(const List *list);

/**
 * @brief Inserta un elemento en la lista en una posición dada.
 *
 * @param list: lista a modificar.
 * @param elem: elemento a insertar.
 * @param elemSize: tamaño en bytes del elemento.
 * @param pos: posición de inserción.
 * @note No inserta si los parámetros son inválidos, si la posición está fuera de rango o si el tamaño no coincide con el de la lista.
 */
void Add(List *list, void *elem, size_t elemSize, size_t pos);

/**
 * @brief Elimina el elemento de una posición de la lista.
 *
 * @param list: lista a modificar.
 * @param pos: posición del elemento a eliminar.
 * @note Si la posición está fuera de rango, no modifica la lista.
 */
void Delete(List *list, size_t pos);

/**
 * @brief Obtiene el elemento en una posición de la lista.
 *
 * @param list: lista consultada.
 * @param pos: posición del elemento.
 * @return void*: puntero al elemento, o NULL si no existe o los parámetros son inválidos.
 * @note El puntero pertenece al nodo; no debe liberarse directamente.
 */
void *SearchInPosition(const List *list, size_t pos);

/**
 * @brief Divide una lista en dos listas y deja la original vacía.
 *
 * @param list: lista origen.
 * @param list1: primera lista destino.
 * @param list2: segunda lista destino.
 * @note Las listas destino deben ser distintas entre sí y distintas de la lista origen.
 */
void SplitList(List *list, List *list1, List *list2);

/**
 * @brief Ordena una lista usando merge sort.
 *
 * @param list: lista a ordenar.
 * @param cmp: función de comparación; retorna > 0 si el primer elemento va antes que el segundo.
 */
void MergeSortList(List *list, int (*cmp)(const void *, const void *));

/**
 * @brief Elimina todos los elementos de la lista.
 *
 * @param list: lista a vaciar.
 * @note Conserva la lista y su configuración interna para reutilización.
 */
void ClearList(List *list);

/**
 * @brief Libera la memoria de una lista y de todos sus elementos.
 *
 * @param list: lista a destruir.
 * @note Si list es NULL, no realiza ninguna acción.
 */
void DestroyList(List *list);

#endif
