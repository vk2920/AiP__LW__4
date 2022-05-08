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
/// �������, ���������� ����� ���� � �������� ������, �� ������� ��� ���������
/// </summary>
/// <param name="root">��������� �� ������ ������/���������</param>
/// <param name="node">��������� �� ���� ������/���������</param>
/// <returns>1, ���� �� ������ �������, ��� 0 � ������ ������</returns>
int paste_node(worker* _root, worker* _node) {
    // ���� ������ ������ / ��������� ��� ���� ���� NULL ��� ID'����� �����
    if (!_root || !_node || (_root->d.id == _node->d.id)) return 0;

    if (_node->d.id < _root->d.id) {  // ���� ���� ������� �����
        if (_root->l) return paste_node(_root->l, _node);
        _node->p = _root, _root->l = _node;
    }
    else {  // ���� ���� ������� ������
        if (_root->r) return paste_node(_root->r, _node);
        _node->p = _root, _root->r = _node;
    }
    return 1;
}

/// <summary>
/// ������� ��� �������� ���� � ������ � ������������� ����������� ����� ����
/// </summary>
/// <param name="_node">�������� �� ��������� ����</param>
/// <returns>1 � ������ ������, ��� 0 � ������ ������</returns>
worker* remove_node(worker* _root, worker* _node) {
    if (!_node) return NULL;

    // ���� ���� �������� �������, ������ ������ ���, ������������ ��������� � ��������
    if (!_node->l && !_node->r) {
        worker* _p = _node->p;
        if (_p && _p->l == _node) _p->l = NULL;
        else if (_p && _p->r == _node) _p->r = NULL;
        else return NULL;
        free(_node);
        return _root;
    }

    // ���� ������� ���� ���� ������� (������ ������), �� ������ �������� ��� �� ����� ����������
    else if (!_node->l && _node->r) {
        worker* _p = _node->p;
        if (_p && _p->l == _node) _p->l = _node->r;
        else if (_p && _p->r == _node) _p->r = _node->r;
        else return NULL;
        _node->r->p = _p;
        free(_node);
        return _root;
    }

    // ���� ������� ���� ���� ������� (������ �����), �� ������ �������� ��� �� ����� ����������
    else if (_node->l && !_node->r) {
        worker* _p = _node->p;
        if (_p && _p->l == _node) _p->l = _node->l;
        else if (_p && _p->r == _node) _p->r = _node->l;
        else return NULL;
        _node->l->p = _p;
        free(_node);
        return _root;
    }

    // ���� ���� ��� ����
    else if (_node->l && _node->r) {
        // ����� ����� ����� ������� ������� ���������
        worker* _tmp = find_left(_node->r);
        if (!_tmp) return NULL;

        // ��������� �������������� ���� �� ����� ����������
        _node->d = _tmp->d;

        remove_node(_root, _tmp);

        return _root;
    }
    else return NULL;
}