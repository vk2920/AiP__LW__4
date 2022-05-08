#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#ifndef FILE
#include <stdio.h>
#endif

#ifndef system
#include <windows.h>
#endif

#ifndef worker
#include "tree_types.h"
#endif

#ifndef create_node
#include "tree_create_node.h"
#endif

#ifndef paste_node
#include "tree_nodes_edit.h"
#endif

#include "tree_files.h"

/// <summary>
/// ������� ���������� � ������������ ��� ����� ��� ��������
/// � ��������� ��� ��� ������ � ������ (��� ������������� ������ ���)
/// </summary>
/// <returns>��������� �� ����, �������� ��� ������ � ������</returns>
FILE* open_file(char* fname) {
    FILE* file;
    while (1) {
        system("cls");  // ������� �������
        printf("������� �������� ����� (��� ��������): ");
        gets_s(fname, 256);               // ���� �������� �����
        file = fopen(fname, "rb+");       // ������� ���� ��� ������ � ������
        if (!file) {                      // ���� ���� �� ������
            file = fopen(fname, "wb+");     // �������� ��� � ������� ��� ������ � ������
            if (file) return file;          // ���� ���� ������, ����� ��������� �� ����
        }
        else return file;                 // ���� ���� ������, ����� ��������� �� ����
    }
}

/// <summary>
/// ������� ��� ������ ������ / ��������� � ����
/// ����� �������������� ����������� ��������� � ����� �� ������ �������
/// </summary>
/// <param name="root">��������� �� ������ ������������� ������ / ���������</param>
/// <param name="file">��������� �� ����, � ������� ���� ��������</param>
/// <returns>����� ���������� ����� ������ / ���������</returns>
int save_tree_to_file(worker* _root, FILE* _file) {
    // ���� ������/��������� �� ���������� ��� ����� �� ����������, �� ������ ������
    if (!_root || !_file) return 0;

    // ������� �������
    int c = 0;

    // ������� � ���� ������
    fwrite(&(_root->d), dz, 1, _file);
    c++;

    // ������� ����� ���������, ���� ��� ����������
    if (_root->l) c += save_tree_to_file(_root->l, _file);

    // ������� ������ ���������, ���� ��� ����������
    if (_root->r) c += save_tree_to_file(_root->r, _file);

    // ����� ��������� ���������� ����� ������
    return c;
}

/// <summary>
/// �������, ��������� ����� ������ �� ������ �������������� ����� � �����
/// </summary>
/// <param name="file">�������� �� ����, �������� ��� ������ (����������� ������ �� ����� ��������)</param>
/// <returns>��������� �� ������ ������ ������</returns>
worker* read_tree_from_file(FILE* _file) {
    // ���� ��� �����, ������ ������
    if (!_file) return NULL;

    // ������� ����� ��� ��������������� ����
    worker_data* _p_data = (worker_data*)malloc(dz);

    // ������� ������ �������������� ���� � �������� � ������ ������
    if (!_p_data || !fread(_p_data, dz, 1, _file)) return NULL;

    // �������� ������ ������
    worker* _root = create_node(*_p_data);

    // � �������� ������ ������
    while (fread(_p_data, dz, 1, _file) && paste_node(_root, create_node(*_p_data)));

    // ��������� ������
    free(_p_data);

    // ����� ��������� �� ������ ������ ������
    return _root;
}