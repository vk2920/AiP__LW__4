#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#ifndef FILE
#include <stdio.h>
#endif

#ifndef worker
#include "tree_types.h"
#endif

#include "tree_nodes_edit.h"

/// <summary>
/// Функция, помещающая новый узел в бинарное дерево, не нарушая его структуру
/// </summary>
/// <param name="root">Указатель на корень дерева/поддерева</param>
/// <param name="node">Указатель на узел дерева/поддерева</param>
/// <returns>1, если всё прошло успешно, или 0 в случае ошибки</returns>
int paste_node(worker* _root, worker* _node) {
    // Если корень дерева / поддерева или узел есть NULL или ID'шники равны
    if (!_root || !_node || (_root->d.id == _node->d.id)) return 0;

    if (_node->d.id < _root->d.id) {  // Если надо уходить влево
        if (_root->l) return paste_node(_root->l, _node);
        _node->p = _root, _root->l = _node;
    }
    else {  // Если надо уходить вправо
        if (_root->r) return paste_node(_root->r, _node);
        _node->p = _root, _root->r = _node;
    }
    return 1;
}

/// <summary>
/// Функция для удаления узла в дереве и реорганизации поддеревьев этого узла
/// </summary>
/// <param name="_node">Указаель на удаляемый узел</param>
/// <returns>1 в случае успеха, или 0 в случае ошибки</returns>
worker* remove_node(worker* _root, worker* _node) {
    if (!_node) return NULL;

    // Если узел является листком, просто удалим его, переназначив указатель у родителя
    if (!_node->l && !_node->r) {
        worker* _p = _node->p;
        if (_p && _p->l == _node) _p->l = NULL;
        else if (_p && _p->r == _node) _p->r = NULL;
        else return NULL;
        free(_node);
        return _root;
    }

    // Если имеется лишь один потомок (только правый), то просто поставим его на место удаляемого
    else if (!_node->l && _node->r) {
        worker* _p = _node->p;
        if (_p && _p->l == _node) _p->l = _node->r;
        else if (_p && _p->r == _node) _p->r = _node->r;
        else return NULL;
        _node->r->p = _p;
        free(_node);
        return _root;
    }

    // Если имеется лишь один потомок (только левый), то просто поставим его на место удаляемого
    else if (_node->l && !_node->r) {
        worker* _p = _node->p;
        if (_p && _p->l == _node) _p->l = _node->l;
        else if (_p && _p->r == _node) _p->r = _node->l;
        else return NULL;
        _node->l->p = _p;
        free(_node);
        return _root;
    }

    // Если есть оба узла
    else if (_node->l && _node->r) {
        // Найдём самый левый элемент правого поддерева
        worker* _tmp = find_left(_node->r);
        if (!_tmp) return NULL;

        // Скопируем информационное поле на место удаляемого
        _node->d = _tmp->d;

        remove_node(_root, _tmp);

        return _root;
    }
    else return NULL;
}