#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#ifndef printf
#include <stdio.h>
#endif

#ifndef system
#include <windows.h>
#endif

#ifndef worker
#include "tree_types.h"
#endif

#ifndef enter_node_data
#include "tree_create_node.h"
#endif

#ifndef paste_node
#include "tree_nodes_edit.h"
#endif

/// <summary>
/// ������� ��� �������� ����� ������ (������������ ��� ������ �� ���������)
/// </summary>
/// <param name="_root">��������� �� ������ ������ / ���������</param>
/// <returns>����� �������� ����� ������ / ���������</returns>
int remove_tree(worker* _root) {
    if (!_root) return 0;
    int c = 0;
    if (_root->l) c += remove_tree(_root->l);
    if (_root->r) c += remove_tree(_root->r);
    remove_node(_root, _root);
    c++;
    return c;
}

/// <summary>
/// ������� ��� ����������� ������ �� ������ �������� ������������� ������
/// </summary>
/// <returns>��������� �� ������ ������ ������</returns>
worker* organize_tree() {
    // �������� �������� ������ � ���������� � �������� ������� ������
    worker_data _d = enter_node_data("������ ����. ��� ������ ������������� ������� ������ � ID < 0\n");
    if (_d.id < 0) return NULL;

    // ���� �� �����, ������ �������� ���� ������
    worker* _root = create_node(_d);

    // �������� ����� ������ � ������ � ��� ���, ���� ������������ �� ������� ������������
    // ��� �� ������ OOM (�� ��� ��� ������ �������)
    while (1) {
        // ������� ����� ������
        _d = enter_node_data("��� ���������� ����������� ������ ������� ID < 0\n");
        // ������, ���� ���� ����� ������������
        if (_d.id < 0) break;
        // �������� � ��������� � ������ ����� ����
        paste_node(_root, create_node(_d));
        // ������� �� ���� ������������
        system("cls"), printf("���� ������ ����������\n\n"), _getch();
    }

    // ����� ��������� �� ������ ������
    return _root;
}