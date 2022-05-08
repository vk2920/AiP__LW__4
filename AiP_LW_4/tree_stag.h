#pragma once

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#ifndef worker
#include "tree_types.h"
#endif


int get_nodes_count(worker* _root);
int get_stag_summ(worker* _root);
double get_stag_avg(worker* _root);