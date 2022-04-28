#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <time.h>
#include "menu.h"

#define menu_items_count 14  // Это используется в двух (или более точках), но при изменении проще менять одно значение

typedef struct { int id, year; char fio[64], job[64]; } worker_data;  // Информационное поле
struct worker { worker_data d; struct worker* l, * r, * p; };         // Узел дерева
typedef struct worker worker;

// Для минимизации кода
#define dz sizeof(worker_data)
#define nz sizeof(worker)

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

FILE* open_file(char* fname);
int paste_node(worker* _root, worker* _node);
worker* remove_node(worker* _root, worker* _node);
worker_data enter_node_data(const char* _d);
worker* create_node(worker_data _data);

worker* find_left(worker* _root);
worker* find_node(worker* _root, int _id);
int remove_tree(worker* _root);
int save_tree_to_file(worker* _root, FILE* _file);
worker* read_tree_from_file(FILE* _file);

void print_tree_table(worker* _root, int _is_root);
void print_tree_struct(worker* _root, int _pos);
worker* organize_tree();
int print_older_workers_internal(worker* _root, int _is_root, int year);
int print_older_workers(worker* _root, int _is_root);

int print_right_node(worker* _root);

int get_nodes_count(worker* _root);
int get_stag_summ(worker* _root);
double get_stag_avg(worker* _root);

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
/// Функция, помещающая новый узел в бинарное дерево, не нарушая его структуру
/// </summary>
/// <param name="root">Указатель на корень дерева/поддерева</param>
/// <param name="node">Указатель на узел дерева/поддерева</param>
/// <returns>1, если всё прошло успешно, или 0 в случае ошибки</returns>
int paste_node(worker* _root, worker* _node) {
    // Если корень дерева / поддерева или узел есть NULL или ID'шники равны
    if (!_root || !_node || (_root->d.id == _node->d.id)) return 0;

    if (_node->d.id < _root->d.id) {  // Если надо уходить влево
        if (_root->l) return paste_node(_root->l, _node);
        _node->p = _root, _root->l = _node;
    }
    else {  // Если надо уходить вправо
        if (_root->r) return paste_node(_root->r, _node);
        _node->p = _root, _root->r = _node;
    }
    return 1;
}

/// <summary>
/// Функция для удаления узла в дереве и реорганизации поддеревьев этого узла
/// </summary>
/// <param name="_node">Указаель на удаляемый узел</param>
/// <returns>1 в случае успеха, или 0 в случае ошибки</returns>
worker* remove_node(worker* _root, worker* _node) {
    if (!_node) return NULL;

    // Если узел является листком, просто удалим его, переназначив указатель у родителя
    if (!_node->l && !_node->r) {
        worker* _p = _node->p;
        if (_p && _p->l == _node) _p->l = NULL;
        else if (_p && _p->r == _node) _p->r = NULL;
        else return NULL;
        free(_node);
        return _root;
    }

    // Если имеется лишь один потомок (только правый), то просто поставим его на место удаляемого
    else if (!_node->l && _node->r) {
        worker* _p = _node->p;
        if (_p && _p->l == _node) _p->l = _node->r;
        else if (_p && _p->r == _node) _p->r = _node->r;
        else return NULL;
        _node->r->p = _p;
        free(_node);
        return _root;
    }

    // Если имеется лишь один потомок (только левый), то просто поставим его на место удаляемого
    else if (_node->l && !_node->r) {
        worker* _p = _node->p;
        if (_p && _p->l == _node) _p->l = _node->l;
        else if (_p && _p->r == _node) _p->r = _node->l;
        else return NULL;
        _node->l->p = _p;
        free(_node);
        return _root;
    }

    // Если есть оба узла
    else if (_node->l && _node->r) {
        // Найдём самый левый элемент правого поддерева
        worker* _tmp = find_left(_node->r);
        if (!_tmp) return NULL;

        // Скопируем информационное поле на место удаляемого
        _node->d = _tmp->d;

        remove_node(_root, _tmp);

        return _root;
    }
    else return NULL;
}

/// <summary>
/// Функция для создания информационного поля для элемента списка
/// Данные вводятся с клавиатуры
/// </summary>
/// <param name="d">Си-строка, содержащая сообщения для пользователя</param>
worker_data enter_node_data(const char* _d) {
    worker_data data;
    system("cls");
    if (_d) printf("%s", _d);
    printf("+------+----------------------------------+----------------------------------+------+\n|  ID  |          Фамилия  И. О.          |            Должность             | Год  |\n+------+----------------------------------+----------------------------------+------+\n");
    printf("| "); scanf("%d", &(data.id));
    while (getchar() != '\n'); system("cls");
    if (_d) printf("%s", _d);
    printf("+------+----------------------------------+----------------------------------+------+\n|  ID  |          Фамилия  И. О.          |            Должность             | Год  |\n+------+----------------------------------+----------------------------------+------+\n");
    printf("| %-4d | ", data.id); gets_s(data.fio, 64); system("cls");
    if (_d) printf("%s", _d);
    printf("+------+----------------------------------+----------------------------------+------+\n|  ID  |          Фамилия  И. О.          |            Должность             | Год  |\n+------+----------------------------------+----------------------------------+------+\n");
    printf("| %-4d | %-32s | ", data.id, data.fio); gets_s(data.job, 64); system("cls");
    if (_d) printf("%s", _d);
    printf("+------+----------------------------------+----------------------------------+------+\n|  ID  |          Фамилия  И. О.          |            Должность             | Год  |\n+------+----------------------------------+----------------------------------+------+\n");
    printf("| %-4d | %-32s | %-32s | ", data.id, data.fio, data.job); scanf("%d", &(data.year));
    while (getchar() != '\n'); system("cls");
    if (_d) printf("%s", _d);
    printf("+------+----------------------------------+----------------------------------+------+\n|  ID  |          Фамилия  И. О.          |            Должность             | Год  |\n+------+----------------------------------+----------------------------------+------+\n");
    printf("| %-4d | %-32s | %-32s | %4d |\n", data.id, data.fio, data.job, data.year);
    printf("+------+----------------------------------+----------------------------------+------+\n"
        "Проверьте правильность введённых данных и нажмите ENTER / RETURN (в случае ошибки -- ESC) ");
    while (1) {
        int k = _getch();
        if (k == 13) return data;
        else if (k == 27) {
            system("cls");
            return enter_node_data("Вводим заново\n\n");
        }
    }
}

/// <summary>
/// Создаёт элемент списка
/// </summary>
/// <param name="data">Информационное поле для элемента</param>
/// <returns>Указатель на элемент списка или NULL, если элемент не создан</returns>
worker* create_node(worker_data _data) {
    // Создадим новый элемент
    worker* _node = (worker*)malloc(sizeof(worker));

    // Если не получилось выделить память, вернём нулевой указатель
    if (!_node) return NULL;

    // Если память выделена, то загоним информационное поле элемента
    _node->d = _data;

    // И установим указатели на соседние элементы
    _node->p = NULL;
    _node->l = NULL;
    _node->r = NULL;

    // Вернём указатель на созданный элемент списка
    return _node;
}

/// <summary>
/// Функция для поиска указателя на самый левый узел дерева / поддерева
/// </summary>
/// <param name="_root">Указатель на корень дерева / поддерева</param>
/// <returns>Указатель на самый левый узел дерева / поддерева</returns>
worker* find_left(worker* _root) {
    // Если самого корня нет или он является самым левым, просто вернём его
    if (!_root || !_root->l) return _root;
    // Найдём самый левый элемент и вернйм указатель на него
    for (worker* _node = _root; _node->l; _node = _node->l) if (!_node->l) return _node;
    // Вернём NULL в случае ошибки
    return NULL;
}

/// <summary>
/// Функция для поиска узла дерева с заданным ID элемента
/// </summary>
/// <param name="_root">Корень дерева / поддерева, в котором выполняется поиск</param>
/// <param name="_id">ID узла, который надо найти</param>
/// <returns>Указатель на искомы узел дерева / поддерева</returns>
worker* find_node(worker* _root, int _id) {
    // Вернём NULL, если нет дерева
    if (!_root) return NULL;

    if (_id > _root->d.id) {
        // Если требуемый ID больше, чем ID корня дерева / поддерева, то уйдём в правое поддерево
        if (_root->r) return find_node(_root->r, _id);
        else return NULL;  // Если нет поддерева, то просто вернём NULL
    } else if (_id < _root->d.id) {
        // Если требуемый ID больше, чем ID корня дерева / поддерева, то уйдём в правое поддерево
        if (_root->l) return find_node(_root->l, _id);
        else return NULL;  // Если нет поддерева, то просто вернём NULL
    } else return _root;  // Оба варианта неравенства рассмотрены выше,
                          // указатель на текущий узел вернётся только в случае раверства
}

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
    return (double) summ / c;
}

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
            print_older_workers(tree_root);
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
