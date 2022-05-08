#pragma once

typedef struct { int id, year; char fio[64], job[64]; } worker_data;  // Информационное поле
struct worker { worker_data d; struct worker* l, * r, * p; };         // Узел дерева
typedef struct worker worker;

#define dz sizeof(worker_data)
#define nz sizeof(worker)