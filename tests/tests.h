#pragma once

#include <iostream>
#include <unistd.h>
#include <csignal>
#include <sys/time.h>
#include <vector>
#include <string>
#include <string.h>
#include <cassert>

#include "core/Test.h"
#include "core/Config.h"
#include "core/color.h"

#define assert_str(s1, s2) \
    (strcmp(s1, s2) != 0 ? \
    __assert_str_rtn(s1, s2, __FILE__, __func__, __LINE__) : \
    (void)0)

int   run_tests(std::vector<Test*> tests, Config& config);
void 	__assert_str_rtn(const char *s1, const char *s2, const char *file,
                        const char *function, int line);
