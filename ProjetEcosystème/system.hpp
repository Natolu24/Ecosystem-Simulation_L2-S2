#pragma once

#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <ctime>
#include <chrono>
#include <thread>
#include "entity.hpp"

class Entity;

class Case {
    int width, height, x, y, sel = 0, cooldown = 0;
    bool grass = true;
    Entity* ent = nullptr;
    public:
    static std::vector<std::vector<Case>>* plateau;
    Case(int width, int height, int x, int y);
    bool getGrass();
    Entity* getEntity();
    void setEntity(Entity* e);
    void addSel();
    void eatGrass();
    void growGrass();
    std::vector<Case*> getAllCaseAround();
    Entity* getOneSameEntityFromOtherGender(std::string type, bool gender);
    Case* getGrassAround();
    Case* getRandomEmptyCaseAround();
    Case* getRandomEmptyCase();
    char getEntityChar();
    Case* getMoutonAround();
    void move(int dirX, int dirY);
    void move(Case* location);
    void coordPrint();
};
class Ecosysteme {
    int width, height, turn, loups, moutons, delai;
    std::vector<std::vector<Case>> plateau;
    public:
    Ecosysteme(int settings[5]);
    void checkEntitysNmbs();
    void start();
    void mainLoop();
    void nextTurn();
    void print();
};
void init(int x, char** args, int settings[5]);