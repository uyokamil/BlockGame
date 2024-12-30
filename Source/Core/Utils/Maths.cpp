//
// Created by physi on 27/12/2024.
//

#include <bits/stdc++.h>
#include "Maths.h"

std::string Maths::ToLower(const std::string &str)
{
    std::string result = str;
    std::ranges::transform(result.begin(), result.end(), result.begin(),
                           ::tolower);
    return result;
}