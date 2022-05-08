#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#ifndef FILE
#include <stdio.h>
#endif

#ifndef worker
#include "tree_types.h"
#endif

#include "tree_find_nodes.h"

/// <summary>
/// Функция для поиска указателя на самый левый узел дерева / поддерева
/// </summary>
/// <param name="_root">Указатель на корень дерева / поддерева</param>
/// <returns>Указатель на самый левый узел дерева / поддерева</returns>
worker* find_left(worker* _root) {
    // Если самого корня нет или он является самым левым, просто вернём его
    if (!_root || !_root->l) return _root;
    // Найдём самый левый элемент и вернйм указатель на него
    for (worker* _node = _root; _node->l; _node = _node->l) if (!_node->l) return _node;
    // Вернём NULL в случае ошибки
    return NULL;
}

/// <summary>
/// Функция для поиска узла дерева с заданным ID элемента
/// </summary>
/// <param name="_root">Корень дерева / поддерева, в котором выполняется поиск</param>
/// <param name="_id">ID узла, который надо найти</param>
/// <returns>Указатель на искомы узел дерева / поддерева</returns>
worker* find_node(worker* _root, int _id) {
    // Вернём NULL, если нет дерева
    if (!_root) return NULL;

    if (_id > _root->d.id) {
        // Если требуемый ID больше, чем ID корня дерева / поддерева, то уйдём в правое поддерево
        if (_root->r) return find_node(_root->r, _id);
        else return NULL;  // Если нет поддерева, то просто вернём NULL
    }
    else if (_id < _root->d.id) {
        // Если требуемый ID больше, чем ID корня дерева / поддерева, то уйдём в правое поддерево
        if (_root->l) return find_node(_root->l, _id);
        else return NULL;  // Если нет поддерева, то просто вернём NULL
    }
    else return _root;  // Оба варианта неравенства рассмотрены выше,
                       // указатель на текущий узел вернётся только в случае раверства
}