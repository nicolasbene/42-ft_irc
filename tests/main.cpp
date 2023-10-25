#include "tests.h"
#include "utils.h"

void    fake_test(void)
{
    assert(true);
}

void    test_min(void)
{
    assert(utils::min(1, 2) == 1);
    assert(utils::min(2, 1) == 1);
    assert(utils::min(10, 10) == 10);
}

int	main(void)
{
    Config				config;
    std::vector<Test*>	tests;

    config.setStopOnFail(true);
    tests.push_back(new Test("fake_test", &fake_test));
    tests.push_back(new Test("utils::min", &test_min));
    return run_tests(tests, config);
}
