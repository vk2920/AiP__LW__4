#pragma once

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#ifndef FILE
#include <stdio.h>
#endif

FILE* open_file(char* fname);
int save_tree_to_file(worker* _root, FILE* _file);
worker* read_tree_from_file(FILE* _file);