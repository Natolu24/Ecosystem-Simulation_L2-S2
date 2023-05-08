#include "entity.hpp"

// CLASS ENTITY

Entity::Entity(std::string name, int life, int eat, Case* loc) : name(name), life(life), hunger(eat), eatValue(eat), location(loc) {}
Entity::~Entity() {}

// Getter "name" (string)
std::string Entity::getName() {
    return name;
}

// Getter "gender" (bool)
bool Entity::getGender() {
    return gender;
}

// Crée une nouvelle entité de même type autour d'une case vide à coté de la mère
void Entity::reproduction(const Entity& other) {
    canMove = false;
    if (name == other.name && gender != other.gender){
        if (gender == 1) {
            if (location->getRandomEmptyCaseAround() != nullptr)
            {
                Case* c = location->getRandomEmptyCaseAround();
                if (name == "loup")
                    c->setEntity(new Loup(c));
                else
                    c->setEntity(new Mouton(c));
            }
        }
    }
}
// Bouge dans une direction aléatoire ou il y a de la place
void Entity::move(){
    if (!canMove)
        return;
    
    // declarer un pointeur de class Case pour stocker la location d'un vide case voisin 
    Case* c;
    c = location;

    // tester si la case voisin != nullptr si au cas ou il n'y a pas de case vide autour d'un entity 
    if (location != nullptr && location->getRandomEmptyCaseAround() != nullptr)
    {
        location = c->getRandomEmptyCaseAround();
        c->move(location);
    }
}


// CLASS LOUP : public ENTITY

Loup::Loup(Case* loc) : Entity("loup", 60, 10, loc) {}
Loup::~Loup() {}

// Se reproduit ou mange
// manger car on deja declarer un method pour l'action reproduire
// manger method retourner un pointeur sur la case ou il manger il retourner nullptr ou cas de ne mange pas
void Loup::manger() {
    Case* c;
    c = location->getMoutonAround();
    // tester si la case voisin != nullptr si au cas ou il n'y a pas de case vide autour d'un entity 
    if (location != nullptr && c != nullptr)
    {
        delete c->getEntity();
        c->setEntity(nullptr);
        hunger += eatValue;
    }
}
void Loup::action() {
    Entity* other;
    other = location->getOneSameEntityFromOtherGender(name, gender);
    Case* mouton;
    mouton = location->getMoutonAround();
    if (!canMove)
        canMove = true;
    if (hunger > 1 && other != nullptr && cooldown == 0)
    {
        reproduction(*other);
        cooldown += 5;
    }
    else if (mouton != nullptr && hunger <= eatValue)
    {
        manger();  
    }
    else if (hunger < 1 || life < 1)
    {
        location->setEntity(nullptr);
        location->addSel();
        delete this;
    }
    life--, hunger--;
    if (cooldown)
        cooldown--;
}


// CLASS MOUTON : public ENTITY

Mouton::Mouton(Case* loc) : Entity("mouton", 50, 5, loc) {}
Mouton::~Mouton() {}

// Se reproduit ou mange
// manger car on deja declarer un method pour l'action reproduire
// manger method retourner un pointeur sur la case ou il manger il retourner nullptr ou cas de ne mange pas
void Mouton::manger() {
    Case* c;
    c = location->getGrassAround();
    // tester si la case voisin != nullptr si au cas ou il n'y a pas de case vide autour d'un entity 
    if (location != nullptr && c != nullptr)
    {
        c->eatGrass();
        hunger += eatValue;
    }
}

void Mouton::action() {
    Entity* other;
    other = location->getOneSameEntityFromOtherGender(name, gender);
    Case* grass;
    grass = location->getGrassAround();
    if (!canMove)
        canMove = true;
    if (hunger > 1 && other != nullptr && cooldown == 0)
    {
        reproduction(*other);
        cooldown += 5;
    }
    else if (grass != nullptr && hunger <= eatValue)
    {
        manger();  
    }
    else if (hunger < 1 || life < 1)
    {
        location->setEntity(nullptr);
        location->addSel();
        delete this;
    }
    life--, hunger--;
    if (cooldown)
        cooldown--;
}