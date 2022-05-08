#define _CRT_SECURE_NO_WARNINGS

// Библиотеки от препроцессора
#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <time.h>

// Самописная библиотрека для реализации меню
#include "menu.h"

// Модули, содержащие код лабораторной работы
#include "tree_types.h"
#include "tree_files.h"
#include "tree_nodes_edit.h"
#include "tree_create_node.h"
#include "tree_find_nodes.h"
#include "tree_prints.h"
#include "tree_stag.h"
#include "tree_view.h"
#include "tree_other.h"

#define menu_items_count 14  // Это используется в двух (или более точках), но при изменении проще менять одно значение

const char* goodbye[] = {
    "\x1b[37;1m  mmm   mmmm   mmmm  mmmm   mmmmm  m     m  mmmmmm\n",
    "m\"   \" m\"  \"m m\"  \"m #   \"m #    #  \"m m\"   #      \n",
    "#   mm #    # #    # #    # #mmmm    \"#\"    #mmmmm \n",
    "#    # #    # #    # #    # #    #    #     #      \n",
    " \"mmm\"  #mm#   #mm#  #mmm\"  #mmmm     #     #mmmmm \n\x1b[0m"
};
const char* menu[menu_items_count] = {
    "Открыть файл",                          // 0
    "Закрыть файл",                          // 1
    "Открыть дерево из файла",               // 2
    "Реинициализировать дерево",             // 3
    "Добавить элемент в дерево",             // 4
    "Удалить элемент из дерева",             // 5
    "Просмотреть дерево",                    // 6
    "Просмотреть таблицу",                   // 7
    "Сохранить дерево в файл",               // 8
    "Удалить файл",                          // 9
    "Показать сотрудников с большим стажем", // 10
    "Показать наиболее правый узел дерева" , // 11
    "Найти среднеий стаж сотрудников",       // 12
    "Выйти из программы",                    // 13
};

/// <summary>
/// Главная функция в программе
/// </summary>
/// <returns>Код выхода из программы: 0, если всё нормально, или код ошибки при наличии</returns>
int main() {
    system("chcp 1251"), system("cls");
    FILE* f = NULL;
    int r = 0;
    char fname[256] = "\0";
    // worker* tree_root = organize_tree();
    worker* tree_root = NULL;

    while (1) {
        switch (select_from_menu(menu, menu_items_count, menu_items_count - 1)) {
        case 0: // Открыть файл
        {
            system("cls");
            if (f) {
                printf("Файл уже открыт. Вы хотите заменить открытый файл? [ Y / N ] ");
                int a = 0;
                while (a = _getch()) {
                    if (a == 121 || a == 237) {  // Y
                        fclose(f);
                        f = open_file(fname);
                        if (!f) {
                            system("cls");
                            printf("Не удалось открыть файл.\n");
                            _getch();
                        }
                        break;
                    }
                    else if (a == 110 || a == 242) break;  // N
                }
            }
            else {
                f = open_file(fname);
                if (!f) {
                    system("cls");
                    printf("Не удалось открыть файл.\n");
                    _getch();
                }
            }
            break;
        }

        case 1: // Закрыть файл
        {
            system("cls");
            if (f) {
                fclose(f);
                f = NULL;
                printf("Файл закрыт, но его имя осталось в памяти.\n\n");
            }
            else printf("Файл не открыт. Нечего закрывать.\n\n");
            _getch();
            break;
        }

        case 2: // Загрузить дерево из файла
        {
            system("cls");
            if (!f) {
                printf("Файл не открыт, не можем считать данные\n\n");
                _getch();
                break;
            }
            int a, flag = 0;
            if (tree_root) {
                printf("Имеется непустое дерево. Избавиться от него? [ Y / N ] ");
                while (a = _getch()) {
                    if (a == 121 || a == 237) { flag = 1; system("cls"); break; }
                    else if (a == 110 || a == 242) { flag = 0; printf("\nОтменено\n\n"); break; }
                }
            }
            else flag = 1;

            if (flag) {
                tree_root = read_tree_from_file(f);
                if (tree_root) printf("Дерево создано.\n\n");
                else printf("Ошибка создания дерева.\n\n");
            }
            else printf("Не читаем дерево.\n\n");
            _getch();
            break;
        }

        case 3: // Реинициализировать дерево
        {
            int flag;
            if (tree_root) {
                printf("Имеется непустое дерево. Избавиться от него? [ Y / N ]");
                int a;
                while (a = _getch()) {
                    if (a == 121 || a == 237) {
                        flag = 1;
                        system("cls");
                        break;
                    }
                    else if (a == 110 || a == 242) {
                        flag = 0;
                        printf("\nОтменено\n\n");
                        break;
                    }
                }
            }
            else flag = 1;

            if (flag) {
                tree_root = organize_tree();
                if (tree_root) printf("Дерево сохранено.\n\n");
                else printf("Ошибка организации дерева.\n\n");
            }
            _getch();
            break;
        }

        case 4: // Добавить элемент в дерево
        {
            system("cls");
            if (tree_root) paste_node(tree_root, create_node(enter_node_data("")));
            else {
                printf("Нет дерева, создаём корень.\n");
                Sleep(1500);
                tree_root = create_node(enter_node_data(""));
            }
            _getch();
            break;
        }

        case 5: // Удалить элемент из дерева
        {
            system("cls");
            if (tree_root) {
                int id = 0;
                printf("Введите ID удаляемого узла: "), scanf("%d", &id);
                worker* node = find_node(tree_root, id);
                if (node) {
                    worker* new_root = remove_node(tree_root, node);
                    if (new_root) {
                        tree_root = new_root;
                        printf("Узел удалён.\n\n");
                    }
                    else printf("Ошибка удаления узла.\n\n");
                }
                else printf("Узел не найден.\n\n");
            }
            else printf("Нет дерева. Нечего удалять.\n\n");
            _getch();
            break;
        }

        case 6: // Показать дерево
        {
            system("cls");
            if (tree_root) print_tree_struct(tree_root, 4);
            else printf("Нет дерева, нечего выводить.\n\n");
            _getch();
            break;
        }

        case 7: // Показать таблицу
        {
            system("cls");
            if (tree_root) print_tree_table(tree_root, 1);
            else printf("Нет дерева, нечего выводить.\n\n");
            _getch();
            break;
        }

        case 8: // Сохранить дерево в файл
        {
            system("cls");
            r = save_tree_to_file(tree_root, f);
            if (r) printf("%d узлов дерева записаны в файл.\n\n", r);
            else printf("Произошла ошибка при записи\n\n");
            _getch();
            break;
        }

        case 9: // Удалить файл
        {
            system("cls");
            if (strlen(fname)) {
                if (f) fclose(f), f = NULL, remove(fname), printf("Файл удалён.\n\n");
                else printf("Имя файла есть, но файл не открыт.\nПеред удалением откройте файл, он закроется автоматически.\n\n");
            }
            else printf("Нет файла. Даже имени файла нет.\n\n");
            _getch();
            break;
        }

        case 10: // Вывести данные о сотрудниках, чей стаж превышает заданный
        {
            system("cls");
            if (!tree_root) {
                printf("Нет дерева. Выходим...\n\n");
                _getch();
                break;
            }
            print_older_workers(tree_root, 1);
            _getch();
            break;
        }

        case 11: // Показать наиболее правый узел дерева
        {
            system("cls");
            if (!tree_root) {
                printf("Нет дерева, выходим");
                _getch();
                break;
            }
            print_right_node(tree_root);
            _getch();
            break;
        }

        case 12: // Найти среднеий стаж сотрудников
        {
            system("cls");
            if (!tree_root) {
                printf("Нет дерева, выходим...");
                _getch();
                break;
            }
            double avg = get_stag_avg(tree_root);
            if (avg) printf("Найден средний стаж: %f", avg);
            else printf("Произошла ошибка вычисления стажа");
            _getch();
            break;
        }

        case 13: // Выйти из программы
        {
            if (f) fclose(f), f = NULL;
            if (tree_root) remove_tree(tree_root);
            system("cls");
            int* size = get_size();
            int htab, vtab;
            if (size && !size[0]) htab = (get_size()[1] - 48) / 2; else htab = 0;
            if (size && !size[0]) vtab = (get_size()[2] - 5) / 2; else vtab = 0;
            free(size);
            for (int _j = 0; _j < vtab; _j++) printf("\n");
            for (int _i = 0; _i < 5; _i++) { for (int _j = 0; _j < htab; _j++) printf(" "); printf("%s", goodbye[_i]); }
            _getch();
            system("cls");
            return 0;
        }

        default:
        {
            system("cls");
            printf("Выбранное действие не поддерживается.\n\n");
            _getch();
            break;
        }
        }
    }
}
