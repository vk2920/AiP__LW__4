#pragma once

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#ifndef FILE
#include <stdio.h>
#endif

#ifndef worker
#include "tree_types.h"
#endif

void print_tree_table(worker* _root, int _is_root);
void print_tree_struct(worker* _root, int _pos);
