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
/// Функция спрашивает у пользователя имя файла для открытия
/// и открывает его для чтения и записи (при необходимости создаёт его)
/// </summary>
/// <returns>Указатель на файл, открытый для чтения и записи</returns>
FILE* open_file(char* fname) {
    FILE* file;
    while (1) {
        system("cls");  // Очистим консоль
        printf("Введите название файла (без пробелов): ");
        gets_s(fname, 256);               // Ввод названия файла
        file = fopen(fname, "rb+");       // Откроем файл для чтения и записи
        if (!file) {                      // Если файл не открыт
            file = fopen(fname, "wb+");     // Создадим его и откроем для записи и чтения
            if (file) return file;          // Если файл создан, вернём указатель на него
        }
        else return file;                 // Если файл открыт, вернём указатель на него
    }
}

/// <summary>
/// Функция для записи дерева / поддерева в файл
/// Перед использованием переместите указатель в файле на нужную позицию
/// </summary>
/// <param name="root">Указаетль на корень записываемого дерева / поддерева</param>
/// <param name="file">Указатель на файл, в который надо записать</param>
/// <returns>Число записанных узлов дерева / поддерева</returns>
int save_tree_to_file(worker* _root, FILE* _file) {
    // Если дерева/поддерева не существует или файла не существует, то просто выйдем
    if (!_root || !_file) return 0;

    // Сделаем счётчик
    int c = 0;

    // Запишем в файл корень
    fwrite(&(_root->d), dz, 1, _file);
    c++;

    // Запишем левое поддерево, если оно существует
    if (_root->l) c += save_tree_to_file(_root->l, _file);

    // Запишем правое поддерево, если оно существует
    if (_root->r) c += save_tree_to_file(_root->r, _file);

    // Вернём количесво записанных узлов дерева
    return c;
}

/// <summary>
/// Функция, создающая новое дерево на основе информационных полей в файле
/// </summary>
/// <param name="file">Указтель на файл, открытый для чтения (Возможность записи не имеет значения)</param>
/// <returns>Указатель на корень нового дерева</returns>
worker* read_tree_from_file(FILE* _file) {
    // Если нет файла, просто выйдем
    if (!_file) return NULL;

    // Сделаем буфер для информационного поля
    worker_data* _p_data = (worker_data*)malloc(dz);

    // Считаем первое информационное поле и поместим в корень дерева
    if (!_p_data || !fread(_p_data, dz, 1, _file)) return NULL;

    // Создадим корень дерева
    worker* _root = create_node(*_p_data);

    // И закидаем дерево узлами
    while (fread(_p_data, dz, 1, _file) && paste_node(_root, create_node(*_p_data)));

    // Освободим память
    free(_p_data);

    // Вернём указатель на корень нового дерева
    return _root;
}