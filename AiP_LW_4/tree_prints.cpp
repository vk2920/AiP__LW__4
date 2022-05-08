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
/// Рекурсивная функция, вызываемая следующей функцией для процесса вывода
/// </summary>
/// <param name="_root">Указатель на корень дерева / поддерева</param>
/// <param name="_is_root">0, если обычный узел, или 1, если корень дерева</param>
/// <param name="year">Год, ранее которого пришли сотрудники, информацию о которых надо вывести</param>
/// <returns></returns>
int print_older_workers_internal(worker* _root, int _is_root, int year) {
    if (!_root) return 0;
    int c = 0;
    if (_root->l) c += print_older_workers_internal(_root->l, 0, year);
    if (_root->d.year < year) {
        printf("Сотрудник:\n   ФИО: %s\n   Должность: %s\n   Год принятия: %d\n   Уникальный номер: %d\n\n",
            _root->d.fio, _root->d.job, _root->d.year, _root->d.id),
            c++;
    }
    if (_root->r) c += print_older_workers_internal(_root->r, 0, year);
    return c;
}

/// <summary>
/// Функция для вывода информации о сотрудниках, чей стаж превышает заданный пользователем
/// </summary>
/// <param name="_root">Указатель на корень бинарного дерева</param>
/// <returns>Число сотрудников, информация о которых была выведена</returns>
int print_older_workers(worker* _root) {
    if (!_root) {
        printf("Нет дерева, выходим...\n\n");
        return 0;
    }

    time_t now = time(0);
    struct tm* t = localtime(&now);
    int year = 1900 + t->tm_year;
    int stag;
    printf("Текущий год: %d\nВведите требуемый стаж работы: ", year);
    if (!scanf("%d", &stag)) {
        printf("Ошибка чтения с клавиатуры... Не судьба.\n");
        return 0;
    }

    int c = print_older_workers_internal(_root, 1, year - stag);
    if (!c) printf("Не удалось найти сотрудников со стажем более %d лет.\n\n", stag);
    return c;
}

/// <summary>
/// Функция для печати в консоль информации о сотруднике в наиболее правом узле дерева / поддерева
/// </summary>
/// <param name="_root">Указатель на корень дерева / поддерева</param>
/// <returns>0 в случае ошибки или 1 в случае успеха</returns>
int print_right_node(worker* _root) {
    if (!_root) {
        printf("Нет дерева, выходим...\n\n");
        return 0;
    }
    if (_root->r) return print_right_node(_root->r);

    printf("Сотрудник:\n   ФИО: %s\n   Должность: %s\n   Год принятия: %d\n   Уникальный номер: %d\n\n",
        _root->d.fio, _root->d.job, _root->d.year, _root->d.id);
    return 1;
}