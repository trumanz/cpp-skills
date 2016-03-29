#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "gtest/gtest.h"
#include <map>
#include <string>
#include "BaseTemplateWithStatic.h"

template<> int BaseTemplate<double>::static_value  = 0;
template<> int BaseTemplate<int>::static_value  = 0;


TEST(tempalte, static_in_base_template) {

    BaseTemplate<int> x;
    BaseTemplate<double> y;

    x.static_value++;
    y.static_value++;

    ASSERT_EQ(1, x.static_value);
    ASSERT_EQ(1, y.static_value);
}
