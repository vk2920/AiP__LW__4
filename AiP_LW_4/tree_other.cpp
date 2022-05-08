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
/// Функция для удаления всего дерева (используется при выходе из программы)
/// </summary>
/// <param name="_root">Указатель на корень дерева / поддерева</param>
/// <returns>Число удалённых узлов дерева / поддерева</returns>
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
/// Функция для организации дерева на основе вводимых пользователем данных
/// </summary>
/// <returns>Указатель на корень нового дерева</returns>
worker* organize_tree() {
    // Сохраним введённые данные в переменную и проверим условие отмены
    worker_data _d = enter_node_data("Первый ввод. Для отмены инициализации введите запись с ID < 0\n");
    if (_d.id < 0) return NULL;

    // Если не вышли, создаём корневой узел дерева
    worker* _root = create_node(_d);

    // Помещаем новые данные в дерево о тех пор, пока пользователь не захочет остановиться
    // Или не словим OOM (но это уже другая история)
    while (1) {
        // Спросим новые данные
        _d = enter_node_data("Для завершения организации дерева введите ID < 0\n");
        // Выйдем, если того хочет пользователь
        if (_d.id < 0) break;
        // Создадим и установим в дерево новый узел
        paste_node(_root, create_node(_d));
        // Сообщим об этом пользователю
        system("cls"), printf("Узел дерева установлен\n\n"), _getch();
    }

    // Вернём указатель на корень дерева
    return _root;
}