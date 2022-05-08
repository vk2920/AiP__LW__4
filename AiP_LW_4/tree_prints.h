#pragma once

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#ifndef worker
#include "tree_types.h"
#endif

int print_older_workers_internal(worker* _root, int _is_root, int year);
int print_older_workers(worker* _root, int _is_root);
int print_right_node(worker* _root);