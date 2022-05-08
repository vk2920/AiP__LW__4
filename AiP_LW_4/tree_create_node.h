#pragma once

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#ifndef worker
#include "tree_types.h"
#endif

worker_data enter_node_data(const char* _d);
worker* create_node(worker_data _data);