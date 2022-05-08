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
/// Рекурсивная функция для вывода содержимого дерева в таблице (обход слева направо)
/// </summary>
/// <param name="_root">Указатель на корень дерева</param>
/// <param name="_is_root">Всегда единица</param>
void print_tree_table(worker* _root, int _is_root) {
    // Если нет дерева / поддерева, просто выйдем
    if (!_root) return;

    if (_is_root) printf("+------+----------------------------------+----------------------------------+------+\n|  ID  |          Фамилия  И. О.          |            Должность             | Год  |\n+------+----------------------------------+----------------------------------+------+\n");

    // Если есть левое поддерево, выведем его, используя рекурсию
    if (_root->l) print_tree_table(_root->l, 0);

    // Выведем текущие элемент
    printf("| %4d | %32s | %32s | %4d |\n", _root->d.id, _root->d.fio, _root->d.job, _root->d.year);

    // Если есть правое поддерево, выведем его, используя рекрсию
    if (_root->r) print_tree_table(_root->r, 0);

    if (_is_root) printf("+------+----------------------------------+----------------------------------+------+\n");
}

/// <summary>
/// Рекурсивная функция для вывода структуры дерева (выводит ID и ФИО сотрудников)
/// </summary>
/// <param name="_root">Указатель на корень дерева</param>
/// <param name="_pos">Отступ от левого края экрана</param>
void print_tree_struct(worker* _root, int _pos) {
    // Если нет дерева / поддерева, просто выйдем
    if (!_root) return;

    // Если есть правое поддерево, выведем его, используя рекрсию
    if (_root->r) print_tree_struct(_root->r, _pos + 1);

    // Выведем отступ для текущего элемента
    for (int _i = 0; _i < _pos; _i++) printf("    ");

    // Выведем текущие элемент
    printf("%d. %s\n", _root->d.id, _root->d.fio);

    // Если есть левое поддерево, выведем его, используя рекурсию
    if (_root->l) print_tree_struct(_root->l, _pos + 1);
}