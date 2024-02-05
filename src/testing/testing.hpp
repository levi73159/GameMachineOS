#pragma once

#define Assert(condition, message) \
    if (!condition)                \
    {                              \
        exit(....);                    \
    }

#define AssertEquals(expected, actual, message)
#define AssertNotEquals(expected, actual, message)

#define TEST(testFunction)

int testFunction() {
    // if asserts fails then return -1

    return -1;
}

int otherTestFunction() {
    if(testFunction() < 0)
        return -1;
}