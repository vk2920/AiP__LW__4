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
/// ������� ��� ������ ��������� �� ����� ����� ���� ������ / ���������
/// </summary>
/// <param name="_root">��������� �� ������ ������ / ���������</param>
/// <returns>��������� �� ����� ����� ���� ������ / ���������</returns>
worker* find_left(worker* _root) {
    // ���� ������ ����� ��� ��� �� �������� ����� �����, ������ ����� ���
    if (!_root || !_root->l) return _root;
    // ����� ����� ����� ������� � ������ ��������� �� ����
    for (worker* _node = _root; _node->l; _node = _node->l) if (!_node->l) return _node;
    // ����� NULL � ������ ������
    return NULL;
}

/// <summary>
/// ������� ��� ������ ���� ������ � �������� ID ��������
/// </summary>
/// <param name="_root">������ ������ / ���������, � ������� ����������� �����</param>
/// <param name="_id">ID ����, ������� ���� �����</param>
/// <returns>��������� �� ������ ���� ������ / ���������</returns>
worker* find_node(worker* _root, int _id) {
    // ����� NULL, ���� ��� ������
    if (!_root) return NULL;

    if (_id > _root->d.id) {
        // ���� ��������� ID ������, ��� ID ����� ������ / ���������, �� ���� � ������ ���������
        if (_root->r) return find_node(_root->r, _id);
        else return NULL;  // ���� ��� ���������, �� ������ ����� NULL
    }
    else if (_id < _root->d.id) {
        // ���� ��������� ID ������, ��� ID ����� ������ / ���������, �� ���� � ������ ���������
        if (_root->l) return find_node(_root->l, _id);
        else return NULL;  // ���� ��� ���������, �� ������ ����� NULL
    }
    else return _root;  // ��� �������� ����������� ����������� ����,
                       // ��������� �� ������� ���� ������� ������ � ������ ���������
}