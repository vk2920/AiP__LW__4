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

#include "tree_create_node.h"

/// <summary>
/// ������� ��� �������� ��������������� ���� ��� �������� ������
/// ������ �������� � ����������
/// </summary>
/// <param name="d">��-������, ���������� ��������� ��� ������������</param>
worker_data enter_node_data(const char* _d) {
    worker_data data;
    system("cls");
    if (_d) printf("%s", _d);
    printf("+------+----------------------------------+----------------------------------+------+\n|  ID  |          �������  �. �.          |            ���������             | ���  |\n+------+----------------------------------+----------------------------------+------+\n");
    printf("| "); scanf("%d", &(data.id));
    while (getchar() != '\n'); system("cls");
    if (_d) printf("%s", _d);
    printf("+------+----------------------------------+----------------------------------+------+\n|  ID  |          �������  �. �.          |            ���������             | ���  |\n+------+----------------------------------+----------------------------------+------+\n");
    printf("| %-4d | ", data.id); gets_s(data.fio, 64); system("cls");
    if (_d) printf("%s", _d);
    printf("+------+----------------------------------+----------------------------------+------+\n|  ID  |          �������  �. �.          |            ���������             | ���  |\n+------+----------------------------------+----------------------------------+------+\n");
    printf("| %-4d | %-32s | ", data.id, data.fio); gets_s(data.job, 64); system("cls");
    if (_d) printf("%s", _d);
    printf("+------+----------------------------------+----------------------------------+------+\n|  ID  |          �������  �. �.          |            ���������             | ���  |\n+------+----------------------------------+----------------------------------+------+\n");
    printf("| %-4d | %-32s | %-32s | ", data.id, data.fio, data.job); scanf("%d", &(data.year));
    while (getchar() != '\n'); system("cls");
    if (_d) printf("%s", _d);
    printf("+------+----------------------------------+----------------------------------+------+\n|  ID  |          �������  �. �.          |            ���������             | ���  |\n+------+----------------------------------+----------------------------------+------+\n");
    printf("| %-4d | %-32s | %-32s | %4d |\n", data.id, data.fio, data.job, data.year);
    printf("+------+----------------------------------+----------------------------------+------+\n"
        "��������� ������������ �������� ������ � ������� ENTER / RETURN (� ������ ������ -- ESC) ");
    while (1) {
        int k = _getch();
        if (k == 13) return data;
        else if (k == 27) {
            system("cls");
            return enter_node_data("������ ������\n\n");
        }
    }
}

/// <summary>
/// ������ ������� ������
/// </summary>
/// <param name="data">�������������� ���� ��� ��������</param>
/// <returns>��������� �� ������� ������ ��� NULL, ���� ������� �� ������</returns>
worker* create_node(worker_data _data) {
    // �������� ����� �������
    worker* _node = (worker*)malloc(sizeof(worker));

    // ���� �� ���������� �������� ������, ������ ������� ���������
    if (!_node) return NULL;

    // ���� ������ ��������, �� ������� �������������� ���� ��������
    _node->d = _data;

    // � ��������� ��������� �� �������� ��������
    _node->p = NULL;
    _node->l = NULL;
    _node->r = NULL;

    // ������ ��������� �� ��������� ������� ������
    return _node;
}