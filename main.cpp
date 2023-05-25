#include <iostream>
#include "classes/RBTree.hpp"
#include "random"
#include "funcs.h"

const int SCREEN_WIDTH = 1500;
const int SCREEN_HEIGHT = 1000;

enum Actions {
    ADD,
    REMOVE,
    QUIT = 999
};

#define DELAY 500 // sdl delay of render in ms


SDL_Renderer *renderer;

void addValue(RBTree<int> &tree) {

    while (true) {
        clear_screen();
        std::string numbers;
        std::cout << "Enter value or 'stop': ";

        getline(std::cin, numbers);

        std::stringstream ss(numbers);
        if (numbers.find("stop") != std::string::npos) {
            return;
        }
        int num;

        while (ss >> num) {


            tree.insert(num);

            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderClear(renderer);

            tree.drawTree(tree.getRoot(), 15 / 2, 50, 15, SCREEN_WIDTH);

            SDL_RenderPresent(renderer);
            SDL_Delay(DELAY);

            tree.saveToJpeg("insert");
            ss.ignore();

        }
    }
}


void removeValue(RBTree<int> &tree) {

    while (true) {
        clear_screen();
        std::string numbers;
        std::cout << "Enter value or 'stop': ";

        getline(std::cin, numbers);

        std::stringstream ss(numbers);
        if (numbers.find("stop") != std::string::npos) {
            return;
        }
        int num;

        while (ss >> num) {


            tree.remove(num);

            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderClear(renderer);

            tree.drawTree(tree.getRoot(), 15 / 2, 50, 15, SCREEN_WIDTH);

            SDL_RenderPresent(renderer);
            SDL_Delay(DELAY);
            tree.saveToJpeg("remove");
            ss.ignore();

        }
    }
}

void menu(RBTree<int> &tree) {
    int c;
    while (true) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
        SDL_Delay(100);
        SDL_RenderClear(renderer);

        tree.drawTree(tree.getRoot(), 15 / 2, 50, 15, SCREEN_WIDTH);

        SDL_RenderPresent(renderer);
        clear_screen();
        std::cout << ADD << ". Insert value into red-black tree" << std::endl;
        std::cout << REMOVE << ". Remove value from red-black tree" << std::endl;
        std::cout << QUIT << ". Quit" << std::endl;
        std::cout << "Choose action:" << std::endl;
        c = readInt();


        switch (c) {
            case ADD: {
                addValue(tree);
                break;
            }
            case REMOVE: {
                removeValue(tree);
                break;
            }
            case QUIT: {
                return;
            }
            default: {
                break;
            }
        }
    }
}

int main() {

    // Init SDL 2
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }
    // Init TTF
    if (TTF_Init() != 0) {
        std::cerr << "TTF_Init Error: " << TTF_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }
    // Create window
    SDL_Window *window = SDL_CreateWindow("Red-Black Tree", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        TTF_Quit();
        SDL_Quit();
        return 1;
    }
    // init SDL_Renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) {
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }


    RBTree<int> tree = RBTree<int>(renderer); // tree init
    menu(tree); // enter to main menu

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
