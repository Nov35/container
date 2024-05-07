#include <lifetime_helper/lifetime_helper.h>

int LifetimeHelper::created_count = 0;
int LifetimeHelper::alive_count = 0;

// #define PRINT_LIFETIME

LifetimeHelper::LifetimeHelper()
{
    number_ = ++created_count;
    ++alive_count;

#ifdef PRINT_LIFETIME
    std::cout << "Default contructed: " << number_ << '\n';
#endif
};

LifetimeHelper::LifetimeHelper(int n)
{
    number_ = n;
    ++alive_count;

#ifdef PRINT_LIFETIME
    std::cout << "Value contructed: " << number_ << '\n';
#endif
};

LifetimeHelper::LifetimeHelper(const LifetimeHelper &oth)
{
    number_ = ++created_count;
    ++alive_count;

#ifdef PRINT_LIFETIME
    std::cout << "Copy-constructed: " << number_ << " from: " << oth.number_ << '\n';
#endif
}

LifetimeHelper::LifetimeHelper(LifetimeHelper &&oth) noexcept
{
    number_ = ++created_count;
    ++alive_count;

#ifdef PRINT_LIFETIME
    std::cout << "Move-constructed: " << number_ << " from: " << oth.number_ << '\n';
#endif
}

LifetimeHelper &LifetimeHelper::operator=(LifetimeHelper &oth)
{
    number_ = oth.number_;
    oth.number_ = -number_;

#ifdef PRINT_LIFETIME
    std::cout << "Copy-assigned: " << number_ << '\n';
#endif

    return *this;
}

LifetimeHelper &LifetimeHelper::operator=(LifetimeHelper &&oth) noexcept
{
    number_ = oth.number_;
    oth.number_ = -number_;

#ifdef PRINT_LIFETIME
    std::cout << "Move-assigned: " << number_ << '\n';
#endif

    return *this;
}

LifetimeHelper::~LifetimeHelper()
{
    --alive_count;

#ifdef PRINT_LIFETIME
    std::cout << "Destructed: " << number_ << '\n';
#endif
}

int LifetimeHelper::get_object_number() const
{
    return number_;
}

int LifetimeHelper::get_alive_count()
{
    return alive_count;
}

std::ostream &operator<<(std::ostream &out, const LifetimeHelper &obj)
{
    out << "Printing the Liftime object #: " << obj.get_object_number() << '\n';
    return out;
}
