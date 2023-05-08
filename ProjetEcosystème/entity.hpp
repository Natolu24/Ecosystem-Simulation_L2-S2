#pragma once

#include <string>
#include "system.hpp"

class Case;

class Entity {
    protected:
    std::string name;
    int life, hunger, cooldown = 5;
    const int eatValue;
    const bool gender = rand()%2; // male = false, female = true
    bool canMove = true;
    Case* location;
    public:
    Entity(std::string name, int life, int eat, Case* loc);
    virtual ~Entity();
    std::string getName();
    bool getGender();
    void reproduction(const Entity& other);
    void move();
    virtual void manger() = 0;
    virtual void action() = 0;
};
class Loup : public Entity{
    public:
    Loup(Case* loc);
    ~Loup() override;
    void manger() override;
    void action() override;
};
class Mouton : public Entity{
    public:
    Mouton(Case* loc);
    ~Mouton() override;
    void manger() override;
    void action() override;
};