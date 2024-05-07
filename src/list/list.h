#pragma once

#include "list_iterator.h"

template <class NodeType, typename Type>
class ListIterator;

template <typename Type>
class List
{
private:
    struct Node
    {
        Node() : value_()
        {
        }

        Node(Type &&value) : value_(std::move(value))
        {
        }

        Node(const Type &value) : value_(value)
        {
        }

        Node *prev_ = nullptr;
        Node *next_ = nullptr;
        Type value_;
    };

    using Iter = ListIterator<Node, Type>;
    using ConstIter = ListIterator<const Node, const Type>;
    using ReverseIter = std::reverse_iterator<Iter>;
    using ConstReverseIter = std::reverse_iterator<ConstIter>;

public: // Special member functions
    List() = default;

    List(const Type &value) : first_(new Node(value)), end_(first_)
    {
    }

    List(Type &&value) : first_(new Node(std::forward<Type>(value))), end_(first_)
    {
    }

    List(std::initializer_list<Type> values)
    {
        for (const Type &val : values)
            emplace_back(val);
    }

    template <typename _InputIterator,
              typename = std::_RequireInputIter<_InputIterator>>
    List(_InputIterator first, _InputIterator last)
    {
        while (first != last)
        {
            emplace_back(*first);
            ++first;
        }
    }

    List(const List &other)
    {
        Node *element = other.first_;

        while (element != other.end_node())
        {
            emplace_back(element->value_);
            element = element->next_;
        }
    }

    List(List &&other) noexcept
    {
        swap(other);
    }

    List &operator=(const List &other)
    {
        if (this != &other)
            List (other).swap(*this);

        return *this;
    }

    List &operator=(List &&other) noexcept
    {
        if (this != &other)
            List(std::move(other)).swap(*this);

        return *this;
    }

    ~List()
    {
        clear();
    }

public: // Size-related methods
    size_t size() const noexcept
    {
        return size_;
    }

    bool empty() const noexcept
    {
        return size_ == 0;
    }

public: // Member access methods
    Type &front() noexcept
    {
        return first_->value_;
    }

    const Type &front() const noexcept
    {
        return first_->value_;
    }

    Type &back() noexcept
    {
        return end_.prev_->value_;
    }

    const Type &back() const noexcept
    {
        return end_.prev_->value_;
    }

public: // Modifying methods
    void push_front(const Type &value)
    {
        create_node(first_, value);
    }

    void push_front(Type &&value)
    {
        create_node(first_, std::move(value));
    }

    template <typename... Types>
    void emplace_front(Types &&...args)
    {
        create_node(first_, std::forward<Types>(args)...);
    }

    void push_back(const Type &value)
    {
        create_node(end_node(), value);
    }

    void push_back(Type &&value)
    {
        create_node(end_node(), std::move(value));
    }

    template <typename... Types>
    void emplace_back(Types &&...args)
    {
        create_node(end_node(), std::forward<Types>(args)...);
    }

    void pop_front() noexcept
    {
        first_ = erase_node(first_);
    }

    void pop_back() noexcept
    {
        Node *tmp = end_.prev_;
        end_.prev_ = end_.prev_->prev_;

        erase_node(tmp);
    }

    void swap(List &other) noexcept
    {
        if(empty())
            first_ = other.end_node();
        else
            end_node()->prev_->next_ = other.end_node();

        if(other.empty())
            other.first_ = end_node();
        else
            other.end_node()->prev_->next_ = end_node();

        std::swap(size_, other.size_);
        std::swap(first_, other.first_);
        std::swap(end_, other.end_);
    }

    void clear() noexcept
    {
        erase_nodes(first_, end_node()->prev_);
    }

    void resize(size_t new_size)
    {
        resize_internal(new_size);
    }

    void resize(size_t new_size, const Type &value)
    {
        resize_internal(new_size, value);
    }

    void assign(std::initializer_list<Type> values)
    {
        clear();

        for (const auto &val : values)
            emplace_back(val);
    }

    template <typename _InputIterator,
              typename = std::_RequireInputIter<_InputIterator>>
    void assign(_InputIterator first, _InputIterator last)
    {
        clear();

        while (first != last)
        {
            emplace_back(*first);
            ++first;
        }
    }

public: // Algorithms
 void sort()
{
    sort(std::greater());
}

template<typename CmpFunc>
    void sort(CmpFunc compare)
    {
        if (empty())
            return;
        int count = 0;

        for (Iter last = --end(); last != begin(); --last)
        {
            bool is_sorted = true;
            Iter current = begin();

            while (current != last)
            {
                Iter next = current;
                ++next;

                if (compare(*current, *next))
                {
                    is_sorted = false;
                    current = swap_nodes(current, next);
                    if (last == next)
                        last = current;
                }
                else
                    ++current;
            }

            if (is_sorted)
                break;
        }
    }

    void splice(ConstIter position, List &other)
    {
        splice(position, other, other.begin(), other.end());
    }

    void splice(ConstIter position, List &other, ConstIter it)
    {
        splice_internal(position.get_node(), it.get_node(), it.get_node());
    }

    void splice(ConstIter position, List &other, ConstIter begin, ConstIter end)
    {
        if(begin == end)
            return;

        Node *first = begin.get_node();
        Node *last = end.get_node()->prev_;

        splice_internal(position.get_node(), other, first, last);
    }

    void reverse()
    {
        if(first_ == end_node())
            return;

        Node* node = first_;
        while (node != end_node())
        {
            std::swap(node->prev_, node->next_);
            node = node->prev_;
        }

        std::swap(first_, end_.prev_);
        first_->prev_ = nullptr;
        end_.prev_->next_ = end_node();
    }

    void merge(List& other)
    {
        Iter pos = begin();

        Iter last = other.begin();
        Iter first = last++;

        while (true)
        {
            while(*pos < *first && pos != end())
                ++pos;

            if(pos == end())
                break;

            while (last != other.end() && *last < *pos)
                ++last;

            splice(pos, other, first, last);

            if(last == other.end())
                break;

            first = last++;
        }

        splice(pos, other, first, other.end());
    }

public: // Iterator-related methods
    Iter erase(Iter it)
    {
        return Iter(erase_node(it.get_node()));
    }

    ReverseIter erase(ReverseIter it)
    {
        return ReverseIter(erase_node((--it.base()).element_));
    }

    Iter insert(Iter it, Type val)
    {
        return Iter(create_node(it.get_node(), std::move(val)));
    }

    ReverseIter insert(const ReverseIter it)
    {
        return ReverseIter(create_node((--it.base()).element_));
    }

public: // Fabric methods
    Iter begin() noexcept
    {
        return Iter(first_);
    }

    Iter end() noexcept
    {
        return Iter(end_node());
    }

    ConstIter begin() const noexcept
    {
        return ConstIter(first_);
    }

    ConstIter end() const noexcept
    {
        return ConstIter(end_node());
    }

    ConstIter cbegin() const noexcept
    {
        return ConstIter(first_);
    }

    ConstIter cend() const noexcept
    {
        return ConstIter(end_node());
    }

    ReverseIter rbegin() noexcept
    {
        return ReverseIter(end());
    }

    ReverseIter rend() noexcept
    {
        return ReverseIter(begin());
    }

    ConstReverseIter crbegin() const noexcept
    {
        return ConstReverseIter(cend());
    }

    ConstReverseIter crend() const noexcept
    {
        return ConstReverseIter(cbegin());
    }

private: // Internal logic
    template <typename... Types>
    Node *create_node(Node *position, Types &&...args)
    {
        Node *new_element = new Node(std::forward<Types>(args)...);

        insert_nodes(position, new_element, new_element);

        return new_element;
    }

    void insert_nodes(Node *position, Node *first, Node *last) noexcept
    {
        size_ += count_nodes(first, last);
        emplace_nodes(position, first, last);
    }

    void emplace_nodes(Node *position, Node *first, Node *last) noexcept
    {
        check_for_front_placement(position, first, last);

        first->prev_ = position->prev_;
        if (position->prev_ != nullptr)
            position->prev_->next_ = first;

        last->next_ = position;
        position->prev_ = last;
    }

    void check_for_front_placement(Node *position, Node *first, Node *last) noexcept
    {
        if (position == first_)
            first_ = first;
        else if (first_ == end_node())
        {
            first_ = first;
            end_node()->prev_ = last;
            last->next_ = end_node();
        }
    }

    inline size_t count_nodes(Node *first, Node *last) noexcept
    {
        int count = 1;

        while (first != last)
        {
            ++count;
            first = first->next_;
        }

        return count;
    }

    inline Node *erase_node(Node *node) noexcept
    {
        return erase_nodes(node, node);
    }

    Node *erase_nodes(Node *first, Node *last) noexcept
    {
        if (first == nullptr || first == end_node())
            return first;

        remove_nodes(first, last);

        Node *end = last->next_;

        while (first != end)
        {
            Node *tmp = first;
            first = first->next_;
            delete tmp;
        }

        return end;
    }

    void remove_nodes(Node *first, Node *last) noexcept
    {
        size_ -= count_nodes(first, last);
        extract_nodes(first, last);
    }

    void extract_nodes(Node *first, Node *last) noexcept
    {
        last->next_->prev_ = first->prev_;

        if (first->prev_ != nullptr)
            first->prev_->next_ = last->next_;

        if (first_ == end_node())
            end_.prev_ = nullptr;

        if (end_.prev_ == nullptr)
            first_ = end_node();
    }

    template <typename... Types>
    void resize_internal(size_t new_size, Types &&...args)
    {
        while (new_size > size_)
            emplace_back(std::forward<Types>(args)...);

        while (new_size < size_)
            pop_back();
    }

    Iter swap_nodes(Iter lhs, Iter rhs)
    {
        if constexpr (std::is_trivially_copyable_v<Type> || std::is_move_constructible_v<Type>)
        {
            std::swap(*lhs, *rhs);
            return rhs;
        }

        extract_nodes(rhs.get_node(), rhs.get_node());
        emplace_nodes(lhs.get_node(), rhs.get_node(), rhs.get_node());

        return lhs;
    }

    void splice_internal(Node *position, List &other, Node *first, Node *last)
    {
        if (this == &other)
        {
            other.extract_nodes(first, last);
            emplace_nodes(position, first, last);
        }
        else
        {
            other.remove_nodes(first, last);
            insert_nodes(position, first, last);
        }
    }

    inline Node *end_node() noexcept
    {
        return reinterpret_cast<Node *>(&end_);
    }

    inline const Node *end_node() const noexcept
    {
        return reinterpret_cast<const Node *>(&end_);
    }

private:
    struct EndNode
    {
        Node *prev_ = nullptr;
    };

private:
    size_t size_ = 0;
    Node *first_ = end_node();
    EndNode end_;
};
