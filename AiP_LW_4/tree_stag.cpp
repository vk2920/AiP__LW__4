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
/// Функция для вычисления количества узлов в дереве / поддереве
/// </summary>
/// <param name="_root">Указатель на корень дерева / поддерева</param>
/// <returns>Количество узлов в дереве / поддереве</returns>
int get_nodes_count(worker* _root) {
    if (!_root) return 0;
    int c = 0;
    if (_root->l) c += get_nodes_count(_root->l);
    if (_root->r) c += get_nodes_count(_root->r);
    return ++c;
}

/// <summary>
/// Функция для вычисления суммы стажа всех сотрудников в дереве / поддереве
/// </summary>
/// <param name="_root">Указатель на корень дерева / поддерева</param>
/// <returns>Сумма стажа всех сотрудников в дереве / поддереве</returns>
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
/// Функция для вычисления среднего арифметического стажа всех сотрудников в дереве
/// </summary>
/// <param name="_root">Указатель на корень дерева / поддерева</param>
/// <returns>Среднее арифметическое по стажу всех сотрудников</returns>
double get_stag_avg(worker* _root) {
    int summ = get_stag_summ(_root);
    printf("Сумма стажа: %d\n", summ);
    int c = get_nodes_count(_root);
    printf("Число сотрудников: %d\n\n", c);
    if (!summ || !c) return 0;
    return (double)summ / c;
}