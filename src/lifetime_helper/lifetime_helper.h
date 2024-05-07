#pragma once

#include <iostream>

class LifetimeHelper
{
public:
    LifetimeHelper();
    LifetimeHelper(int n);
    LifetimeHelper(const LifetimeHelper &oth);
    LifetimeHelper(LifetimeHelper &&oth) noexcept;
    LifetimeHelper &operator=(LifetimeHelper &oth);
    LifetimeHelper &operator=(LifetimeHelper &&oth) noexcept;
    ~LifetimeHelper();

    int get_object_number() const;
    static int get_alive_count();

private:
    int number_;
    static int created_count;
    static int alive_count;
};

std::ostream &operator<<(std::ostream &out, const LifetimeHelper &obj);
