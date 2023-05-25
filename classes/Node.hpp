#ifndef AUTOMAT_NODE_HPP
#define AUTOMAT_NODE_HPP

template<typename T>
class Node {

public:
    enum Color{
        RED,
        BLACK
    };

    Node *right;
    Node *left;
    Node *parent;
    T data;

    Color color;

    Node();
    explicit Node(const T &data);
    ~Node();

};

template<typename T>
Node<T>::Node(const T &data): right(nullptr), left(nullptr), parent(nullptr), data(data), color(RED)  {}

template<typename T>
Node<T>::Node(): right(nullptr), left(nullptr), parent(nullptr), data(), color(RED) {}

template<typename T>
Node<T>::~Node() {
}
#endif //AUTOMAT_NODE_HPP
