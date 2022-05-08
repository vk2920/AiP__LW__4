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

#include "tree_stag.h"

/// <summary>
/// ������� ��� ���������� ���������� ����� � ������ / ���������
/// </summary>
/// <param name="_root">��������� �� ������ ������ / ���������</param>
/// <returns>���������� ����� � ������ / ���������</returns>
int get_nodes_count(worker* _root) {
    if (!_root) return 0;
    int c = 0;
    if (_root->l) c += get_nodes_count(_root->l);
    if (_root->r) c += get_nodes_count(_root->r);
    return ++c;
}

/// <summary>
/// ������� ��� ���������� ����� ����� ���� ����������� � ������ / ���������
/// </summary>
/// <param name="_root">��������� �� ������ ������ / ���������</param>
/// <returns>����� ����� ���� ����������� � ������ / ���������</returns>
int get_stag_summ(worker* _root) {
    if (!_root) return 0;

    time_t now = time(0);
    struct tm* t = localtime(&now);
    int year = 1900 + t->tm_year;

    int stag_summ = 0;

    if (_root->l) stag_summ += get_stag_summ(_root->l);
    stag_summ += (year - _root->d.year);
    if (_root->r) stag_summ += get_stag_summ(_root->r);
    return stag_summ;
}

/// <summary>
/// ������� ��� ���������� �������� ��������������� ����� ���� ����������� � ������
/// </summary>
/// <param name="_root">��������� �� ������ ������ / ���������</param>
/// <returns>������� �������������� �� ����� ���� �����������</returns>
double get_stag_avg(worker* _root) {
    int summ = get_stag_summ(_root);
    printf("����� �����: %d\n", summ);
    int c = get_nodes_count(_root);
    printf("����� �����������: %d\n\n", c);
    if (!summ || !c) return 0;
    return (double)summ / c;
}