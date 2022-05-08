#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#ifndef FILE
#include <stdio.h>
#endif

#ifndef worker
#include "tree_types.h"
#endif

#ifndef time_t
#include <time.h>
#endif

#include "tree_prints.h"

/// <summary>
/// ����������� �������, ���������� ��������� �������� ��� �������� ������
/// </summary>
/// <param name="_root">��������� �� ������ ������ / ���������</param>
/// <param name="_is_root">0, ���� ������� ����, ��� 1, ���� ������ ������</param>
/// <param name="year">���, ����� �������� ������ ����������, ���������� � ������� ���� �������</param>
/// <returns></returns>
int print_older_workers_internal(worker* _root, int _is_root, int year) {
    if (!_root) return 0;
    int c = 0;
    if (_root->l) c += print_older_workers_internal(_root->l, 0, year);
    if (_root->d.year < year) {
        printf("���������:\n   ���: %s\n   ���������: %s\n   ��� ��������: %d\n   ���������� �����: %d\n\n",
            _root->d.fio, _root->d.job, _root->d.year, _root->d.id),
            c++;
    }
    if (_root->r) c += print_older_workers_internal(_root->r, 0, year);
    return c;
}

/// <summary>
/// ������� ��� ������ ���������� � �����������, ��� ���� ��������� �������� �������������
/// </summary>
/// <param name="_root">��������� �� ������ ��������� ������</param>
/// <returns>����� �����������, ���������� � ������� ���� ��������</returns>
int print_older_workers(worker* _root) {
    if (!_root) {
        printf("��� ������, �������...\n\n");
        return 0;
    }

    time_t now = time(0);
    struct tm* t = localtime(&now);
    int year = 1900 + t->tm_year;
    int stag;
    printf("������� ���: %d\n������� ��������� ���� ������: ", year);
    if (!scanf("%d", &stag)) {
        printf("������ ������ � ����������... �� ������.\n");
        return 0;
    }

    int c = print_older_workers_internal(_root, 1, year - stag);
    if (!c) printf("�� ������� ����� ����������� �� ������ ����� %d ���.\n\n", stag);
    return c;
}

/// <summary>
/// ������� ��� ������ � ������� ���������� � ���������� � �������� ������ ���� ������ / ���������
/// </summary>
/// <param name="_root">��������� �� ������ ������ / ���������</param>
/// <returns>0 � ������ ������ ��� 1 � ������ ������</returns>
int print_right_node(worker* _root) {
    if (!_root) {
        printf("��� ������, �������...\n\n");
        return 0;
    }
    if (_root->r) return print_right_node(_root->r);

    printf("���������:\n   ���: %s\n   ���������: %s\n   ��� ��������: %d\n   ���������� �����: %d\n\n",
        _root->d.fio, _root->d.job, _root->d.year, _root->d.id);
    return 1;
}