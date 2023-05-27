#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#include <filesystem>
#include <SDL_image.h>

#include "Node.hpp"


template<typename T>
class RBTree {
public:
    struct Vector3 {
        int x;
        int y;
        int dx;
    };

    RBTree() = delete;


    explicit RBTree(SDL_Renderer *m_renderer): m_renderer(m_renderer) {
        m_root = nullptr;
        std::filesystem::remove_all("images");
        std::filesystem::create_directory("images");
    }

    ~RBTree() {
        destroyTree(m_root);
    }

    void insert(T data);


    Node<T> *getRoot();


private:
    Node<T> *m_root;
    size_t m_iteration{};
    SDL_Renderer * m_renderer;

    void destroyTree(Node<T> *node);

    Node<T> *insertNode(Node<T> *node, Node<T> *newNode);

    Node<T> *searchNode(Node<T> *node, T data);

    Node<T> *minimum(Node<T> *node);

    Node<T> *maximum(Node<T> *node);


    void fixInsert(Node<T> *node);

    void rotateLeft(Node<T> *node);

    void rotateRight(Node<T> *node);

    void deleteFix(Node<T> *node);



public:
    void remove(const T &data);

    void drawNode(Node<T> *node, int x, int y, int radius);

    Vector3 drawTree(Node<T> *node, int x, int y, int radius, int SCREEN_WIDTH);

    void saveToJpeg(const std::string& prefix);
};


template<typename T>
typename RBTree<T>::Vector3
RBTree<T>::drawTree(Node<T> *node, int x, int y, int radius, int SCREEN_WIDTH) {
    Vector3 pos = {x, y, x};
    if (node != nullptr) {
        Vector3 posLeft, posRight;
        posLeft = drawTree(node->left, pos.dx, y + 50, radius, SCREEN_WIDTH);
        pos.dx = radius + 5 + posLeft.dx;
        pos.x = pos.dx;
        SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 20);
        SDL_RenderDrawLine(m_renderer, pos.x, pos.y, posLeft.x, posLeft.y);
        posRight = drawTree(node->right, pos.dx, y + 50, radius, SCREEN_WIDTH);
        pos.dx = posRight.dx;
        SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 20);
        SDL_RenderDrawLine(m_renderer, pos.x, pos.y, posRight.x, posRight.y);
        drawNode(node, pos.x, pos.y, radius);

    }
    return pos;
}

template<typename T>
void RBTree<T>::drawNode(Node<T> *node, int x, int y, int radius) {
    TTF_Font *gFont = TTF_OpenFont("../arial.ttf", radius);

    SDL_Rect rect = {x - radius, y - radius, radius * 2, radius * 2};

    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
    if (node->color == Node<T>::BLACK) {
        SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
    } else {
        SDL_SetRenderDrawColor(m_renderer, 255, 0, 0, 255);
    }
    SDL_RenderFillRect(m_renderer, &rect);

    SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(m_renderer, &rect);

    SDL_Color textColor = {175, 175, 175};
    std::string dataString = std::to_string(node->data);
    if(gFont == nullptr){
        std::cout << "Font do not exists!" << std::endl;
        return;
    }
    SDL_Surface *textSurface = TTF_RenderText_Solid(gFont, dataString.c_str(), textColor);

    SDL_Texture *texture = SDL_CreateTextureFromSurface(m_renderer, textSurface);
    int textWidth = textSurface->w;
    int textHeight = textSurface->h;
    SDL_Rect textRect = {x - textWidth / 2, y - textHeight / 2, textWidth, textHeight};
    SDL_RenderCopy(m_renderer, texture, nullptr, &textRect);
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(texture);
}

template<typename T>
void RBTree<T>::remove(const T &data) {
    Node<T> *p;
    Node<T> *nextNode;
    p = searchNode(m_root, data);
    if (p == nullptr) {
        return;
    }
    if (p->left && p->right) {
        nextNode = minimum(p->right);

        p->data = nextNode->data;
        p = nextNode;
    }
    if (p->left) {
        Node<T> *temp = p->left;
        p->data = temp->data;
        p->left = nullptr;
        delete temp;
    } else if (p->right) {
        Node<T> *temp = p->right;
        p->data = temp->data;
        p->right = nullptr;
        delete temp;
    } else {
        deleteFix(p);
        if (p == m_root) {
            m_root = nullptr;
        } else if (p == p->parent->left) {
            p->parent->left = nullptr;
        } else if (p == p->parent->right) {
            p->parent->right = nullptr;
        }
        delete p;
    }
}

template<typename T>
void RBTree<T>::deleteFix(Node<T> *node) {

    Node<T> *parent;
    Node<T> *brother;
    if (!(node->color == Node<T>::BLACK && node->parent)) {
        node->color = Node<T>::BLACK;
        return;
    }

    parent = node->parent;
    if (node == parent->left) {
        brother = parent->right;
        if (brother->color == Node<T>::RED) {
            parent->color = Node<T>::RED;
            brother->color = Node<T>::BLACK;
            rotateLeft(parent);
            brother = parent->right;
        }
        if ((brother->left == nullptr || brother->left->color == Node<T>::BLACK)
            && (brother->right == nullptr || brother->right->color == Node<T>::BLACK)) {
            brother->color = Node<T>::RED;
            node = parent;
            return deleteFix(node);
        }
        if (brother->right == nullptr || brother->right->color == Node<T>::BLACK) {
            brother->left->color = Node<T>::BLACK;
            brother->color = Node<T>::RED;
            rotateRight(brother);
            brother = parent->right;
        }

        brother->right->color = Node<T>::BLACK;
        brother->color = parent->color;
        parent->color = Node<T>::BLACK;
        rotateLeft(parent);


    } else {
        brother = parent->left;
        if (brother->color == Node<T>::RED) {
            parent->color = Node<T>::RED;
            brother->color = Node<T>::BLACK;
            rotateRight(parent);
            brother = parent->left;
        }
        if ((brother->left == nullptr || brother->left->color == Node<T>::BLACK)
            && (brother->right == nullptr || brother->right->color == Node<T>::BLACK)) {
            brother->color = Node<T>::RED;
            node = parent;
            return deleteFix(node);
        }
        if (brother->left == nullptr || brother->left->color == Node<T>::BLACK) {
            brother->right->color = Node<T>::BLACK;
            brother->color = Node<T>::RED;
            rotateLeft(brother);
            brother = parent->left;
        }

        brother->left->color = Node<T>::BLACK;
        brother->color = parent->color;
        parent->color = Node<T>::BLACK;
        rotateRight(parent);

    }

    node->color = Node<T>::BLACK;

}

template<typename T>
void RBTree<T>::rotateRight(Node<T> *node) {
    Node<T> *left = node->left;
    node->left = left->right;
    if (left->right != nullptr) {
        left->right->parent = node;
    }
    left->parent = node->parent;
    if (node->parent == nullptr) {
        m_root = left;
    } else if (node == node->parent->right) {
        node->parent->right = left;
    } else {
        node->parent->left = left;
    }
    left->right = node;
    node->parent = left;
    SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
    SDL_RenderClear(m_renderer);
    drawTree(m_root, 15 / 2, 50, 15, 1500);
    SDL_RenderPresent(m_renderer);
    SDL_Delay(200);
    saveToJpeg("Rr");
}

template<typename T>
void RBTree<T>::rotateLeft(Node<T> *node) {
    Node<T> *right = node->right;
    node->right = right->left;
    if (right->left != nullptr) {
        right->left->parent = node;
    }
    right->parent = node->parent;
    if (node->parent == nullptr) {
        m_root = right;
    } else if (node == node->parent->left) {
        node->parent->left = right;
    } else {
        node->parent->right = right;
    }
    right->left = node;
    node->parent = right;
    SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
    SDL_RenderClear(m_renderer);
    drawTree(m_root, 15 / 2, 50, 15, 1500);
    SDL_RenderPresent(m_renderer);
    SDL_Delay(200);
    saveToJpeg("Rl");
}

template<typename T>
void RBTree<T>::fixInsert(Node<T> *node) {
    if (node->parent == nullptr) {
        node->color = Node<T>::BLACK;
        return;
    }

    if (node->parent->color == Node<T>::BLACK) {
        return;
    }

    Node<T> *grandparent = node->parent->parent;
    Node<T> *uncle;

    if (node->parent == grandparent->left) {
        uncle = grandparent->right;

        if (uncle != nullptr && uncle->color == Node<T>::RED) {
            grandparent->color = Node<T>::RED;
            node->parent->color = Node<T>::BLACK;
            uncle->color = Node<T>::BLACK;
            fixInsert(grandparent);
        } else {
            if (node == node->parent->right) {
                node = node->parent;
                rotateLeft(node);
            }

            node->parent->color = Node<T>::BLACK;
            grandparent->color = Node<T>::RED;
            rotateRight(grandparent);
        }
    } else {
        uncle = grandparent->left;

        if (uncle != nullptr && uncle->color == Node<T>::RED) {
            grandparent->color = Node<T>::RED;
            node->parent->color = Node<T>::BLACK;
            uncle->color = Node<T>::BLACK;
            fixInsert(grandparent);
        } else {
            if (node == node->parent->left) {
                node = node->parent;
                rotateRight(node);
            }

            node->parent->color = Node<T>::BLACK;
            grandparent->color = Node<T>::RED;
            rotateLeft(grandparent);
        }
    }
}

template<typename T>
Node<T> *RBTree<T>::maximum(Node<T> *node) {
    if (node->right == nullptr) {
        return node;
    }
    return maximum(node->right);
}

template<typename T>
Node<T> *RBTree<T>::minimum(Node<T> *node) {
    if (node->left == nullptr) {
        return node;
    }
    return minimum(node->left);
}

template<typename T>
Node<T> *RBTree<T>::searchNode(Node<T> *node, T data) {
    if (node == nullptr || node->data == data) {
        return node;
    }

    if (data < node->data) {
        return searchNode(node->left, data);
    } else {
        return searchNode(node->right, data);
    }
}

template<typename T>
Node<T> *RBTree<T>::insertNode(Node<T> *node, Node<T> *newNode) {
    if (node == nullptr) {
        return newNode;
    }

    if (newNode->data < node->data) {
        node->left = insertNode(node->left, newNode);
        node->left->parent = node;
    } else if (newNode->data > node->data) {
        node->right = insertNode(node->right, newNode);
        node->right->parent = node;
    }

    return node;
}

template<typename T>
void RBTree<T>::destroyTree(Node<T> *node) {
    if (node != nullptr) {
        destroyTree(node->left);
        destroyTree(node->right);
        delete node;
    }
}

template<typename T>
Node<T> *RBTree<T>::getRoot() {
    return m_root;
}

template<typename T>
void RBTree<T>::insert(T data) {
    auto *node = new Node<T>(data);
    m_root = insertNode(m_root, node);
    if (node != nullptr)
        fixInsert(node);
}

template<typename T>
void RBTree<T>::saveToJpeg(const std::string& prefix) {
    SDL_Surface* surface;
    SDL_Texture* texture;
    int width, height;

    std::string filename = "images/image_";
    filename += std::to_string(m_iteration);
    filename += "_";
    filename += prefix;
    filename += ".jpeg";
    m_iteration++;

    // Get size of window
    SDL_GetRendererOutputSize(m_renderer, &width, &height);

    // Create texture
    texture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);
    if (!texture) {
        SDL_Log("Failed to create texture: %s", SDL_GetError());
        return;
    }
    // Create surface
    surface = SDL_CreateRGBSurfaceWithFormat(0, width, height, 32, SDL_PIXELFORMAT_RGBA8888);
    if (!surface) {
        SDL_Log("Failed to create surface: %s", SDL_GetError());
        SDL_DestroyTexture(texture);
        return;
    }

    SDL_SetRenderTarget(m_renderer, texture);
    SDL_RenderCopy(m_renderer, texture, nullptr, nullptr);
    SDL_SetRenderTarget(m_renderer, nullptr);

    if (SDL_RenderReadPixels(m_renderer, nullptr, SDL_PIXELFORMAT_RGBA8888, surface->pixels, surface->pitch) != 0) {
        SDL_Log("Failed to read pixels from m_renderer: %s", SDL_GetError());
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
        return;
    }

    // save to JPEG
    if (IMG_SaveJPG(surface, filename.c_str(), 80) != 0) {
        SDL_Log("Failed to save JPEG: %s", IMG_GetError());
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
        return;
    }
}



