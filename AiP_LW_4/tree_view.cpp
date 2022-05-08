#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#ifndef FILE
#include <stdio.h>
#endif

#ifndef worker
#include "tree_types.h"
#endif

#include "tree_view.h"

/// <summary>
/// ����������� ������� ��� ������ ����������� ������ � ������� (����� ����� �������)
/// </summary>
/// <param name="_root">��������� �� ������ ������</param>
/// <param name="_is_root">������ �������</param>
void print_tree_table(worker* _root, int _is_root) {
    // ���� ��� ������ / ���������, ������ ������
    if (!_root) return;

    if (_is_root) printf("+------+----------------------------------+----------------------------------+------+\n|  ID  |          �������  �. �.          |            ���������             | ���  |\n+------+----------------------------------+----------------------------------+------+\n");

    // ���� ���� ����� ���������, ������� ���, ��������� ��������
    if (_root->l) print_tree_table(_root->l, 0);

    // ������� ������� �������
    printf("| %4d | %32s | %32s | %4d |\n", _root->d.id, _root->d.fio, _root->d.job, _root->d.year);

    // ���� ���� ������ ���������, ������� ���, ��������� �������
    if (_root->r) print_tree_table(_root->r, 0);

    if (_is_root) printf("+------+----------------------------------+----------------------------------+------+\n");
}

/// <summary>
/// ����������� ������� ��� ������ ��������� ������ (������� ID � ��� �����������)
/// </summary>
/// <param name="_root">��������� �� ������ ������</param>
/// <param name="_pos">������ �� ������ ���� ������</param>
void print_tree_struct(worker* _root, int _pos) {
    // ���� ��� ������ / ���������, ������ ������
    if (!_root) return;

    // ���� ���� ������ ���������, ������� ���, ��������� �������
    if (_root->r) print_tree_struct(_root->r, _pos + 1);

    // ������� ������ ��� �������� ��������
    for (int _i = 0; _i < _pos; _i++) printf("    ");

    // ������� ������� �������
    printf("%d. %s\n", _root->d.id, _root->d.fio);

    // ���� ���� ����� ���������, ������� ���, ��������� ��������
    if (_root->l) print_tree_struct(_root->l, _pos + 1);
}