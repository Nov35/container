#pragma once

#include <iterator>

template <typename Type>
class List;

template <class NodeType, typename Type>
class ListIterator
{
public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = Type;
    using pointer = Type *;
    using reference = Type &;
    using difference_type = std::ptrdiff_t;

private:
    using DeConstedNode = std::remove_const_t<NodeType>;
    using DeConstedType = std::remove_const_t<Type>;
    using DeConstedIter = ListIterator<DeConstedNode, DeConstedType> ;

public:
    ListIterator(NodeType *element) : node_(element)
    {
    }

    ListIterator(const DeConstedIter &other) : node_(other.node_)
    {
    }

    reference operator*()
    {
        return node_->value_;
    }

    ListIterator &operator++()
    {
        node_ = node_->next_;
        return *this;
    }

    ListIterator operator++(int)
    {
        ListIterator temp = *this;
        node_ = node_->next_;
        return temp;
    }

    ListIterator &operator--()
    {
        node_ = node_->prev_;
        return *this;
    }

    ListIterator operator--(int)
    {
        ListIterator temp = *this;
        node_ = node_->prev_;
        return temp;
    }

    bool operator==(const ListIterator &other) const
    {
        return node_ == other.node_;
    }

    bool operator!=(const ListIterator &other) const
    {
        return !(*this == other);
    }

private:
    DeConstedNode* get_node() const noexcept
    {
        return const_cast<DeConstedNode*>(node_);
    }

private:
    friend class List<Type>;
    friend class List<DeConstedType>;
    friend class ListIterator<const NodeType, const Type>;

private:
    NodeType *node_;
};
