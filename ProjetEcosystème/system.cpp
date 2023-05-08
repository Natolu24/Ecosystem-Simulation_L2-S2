#include "system.hpp"

// CLASS CASE

Case::Case(int width, int height, int x, int y) : width(width), height(height), x(x), y(y) {}

// Getter "grass" (bool)
bool Case::getGrass() {
    return grass;
}

// Getter "ent" (Entity)
Entity* Case::getEntity() {
    return ent;
}

// Setter "ent" (Entity)
void Case::setEntity(Entity* e) {
    ent = e;
}

// Ajoutte 1 sel sur la case
void Case::addSel() {
    sel++;
}

// Mange l'herbe
void Case::eatGrass() {
    grass = false;
    cooldown = 30;
}

// Si pas d'herbe sur la case, décrémente le compteur naturellement a chaque tour, ou gros boost si sel
void Case::growGrass() {
    if (!grass) {
        if (sel && cooldown != 1)
            cooldown -= 29, sel--; // utilise 1 sel pour réduire quasi entièrement le cooldown
        else
            cooldown--; // cooldown -1 naturellement sans sel
        if (cooldown <= 0)
            cooldown = 0, grass = true;
    }
}

// Retourne les cases autour
std::vector<Case*> Case::getAllCaseAround() {
    std::vector<Case*> vec;
    if (y) // up
        vec.push_back(&plateau->at(y-1)[x]);
    if (x != (width-1)) // right
        vec.push_back(&plateau->at(y)[x+1]);
    if (y != (height-1)) // down
        vec.push_back(&plateau->at(y+1)[x]);
    if (x) // left
        vec.push_back(&plateau->at(y)[x-1]);
    return vec;
}

// Retourne la première entité de même type de l'autre sexe autour de la case
Entity* Case::getOneSameEntityFromOtherGender(std::string type, bool gender) {
    std::vector<Case*> others = getAllCaseAround();
    int i;
    for (i = 0; i < (int)others.size(); i++) {
        if (others[i]->getEntity() != nullptr)
            if ((others[i]->getEntity()->getName() == type) && (others[i]->getEntity()->getGender() != gender))
                return others[i]->getEntity();
    }
    return nullptr;
}

// Retourne une case autour ayant de l'herbe (en comptant celle-ci)
Case* Case::getGrassAround() {
    if (grass) {
        return this;
    }
    std::vector<Case*> others = getAllCaseAround();
    int i;
    for (i = 0; i < (int)others.size(); i++) {
        if (others[i]->getGrass())
            return others[i];
    }
    return nullptr;
}

// Retourne une case aléatoire autour étant vide
Case* Case::getRandomEmptyCaseAround() {
    std::vector<Case*> others = getAllCaseAround();
    int i, r = rand()%(int)others.size(), size = (int)others.size();
    for (i = 0; i < size; i++) {
        if (others[(i+r)%size]->getEntity() == nullptr)
            return others[(i+r)%size];
    }
    return nullptr;
}

// Retourne forcément une case aléatoire vide
Case* Case::getRandomEmptyCase() {
    if (!ent)
        return this;
    else {
        if (x == width-1 && y == height-1)
            return plateau->at(0)[0].getRandomEmptyCase();
        else if (x == width-1)
            return plateau->at(y+1)[0].getRandomEmptyCase();
        else
            return plateau->at(y)[x+1].getRandomEmptyCase();
    }
}

// Retourne le "Charactère" de la case
char Case::getEntityChar() {
    if (!ent) {
        if (sel)
            return 's'; // Sel, aucune entité
        else
            return ' '; // Aucun sel, aucune entité
    }
    else {
        if (ent->getName() == "mouton")
            return 'M'; // Mouton
        else
            return 'L'; // Loup
    }
}

// Retourne Un mouton autour aléatoirement
Case* Case::getMoutonAround() {
    std::vector<Case*> others = getAllCaseAround();
    int i;
    for (i = 0; i < (int)others.size(); i++) {
        if (others[i]->getEntity() != nullptr)
            if (others[i]->getEntity()->getName() == "mouton")
                return others[i];
    }
    return nullptr;
}

// Bouge l'entité présente dans la direction ou il se dirige
void Case::move(int dirX, int dirY) {
    plateau->at(y+dirY)[x+dirX].setEntity(ent);
    ent = nullptr;
}

// Bouge l'entité présente dans la direction ou il se dirige
void Case::move(Case* location) {
    location->setEntity(ent);
    ent = nullptr;
}

void Case::coordPrint() {
    std::cout << "x:" << x << " y:" << y << std::endl;
}
std::vector<std::vector<Case>>* Case::plateau = nullptr;


// CLASS ECOSYSTEME

Ecosysteme::Ecosysteme(int settings[5]) : width(settings[0]), height(settings[1]), loups(settings[2]), moutons(settings[3]), delai(settings[4]) {}

// Arrondi les nombres de loups/moutons, si jamais il y en a plus qu'il y a de cases
void Ecosysteme::checkEntitysNmbs() {
    if (loups+moutons <= width*height)
        return;
    if (loups <= moutons) {
        float m = moutons, coef = moutons/loups;
        while (loups+m >= width*height)
            loups--, m -= coef;
        moutons = int(m);
    }
    else if (moutons < loups) {
        float l = loups, coef = loups/moutons;
        while (l+moutons >= width*height)
            moutons--, l -= coef;
        loups = int(l);
    }
}

// Prépare la simulation, puis la démarre
void Ecosysteme::start() {
    int i, j;
    // Crée le plateau et ses cases
    for (i = 0; i < height; i++) {
        std::vector<Case> tmpV;
        for (j = 0; j < width; j++)
            tmpV.push_back(Case(width, height, j, i));
        plateau.push_back(tmpV);
    }
    Case::plateau = &plateau;
    checkEntitysNmbs();
    srand(time(0));
    // Ajoutte les loups
    for (i = 0; i < loups; i++) {
        Case* tmpC = plateau[rand()%height][rand()%width].getRandomEmptyCase();
        Loup* tmpL = new Loup(tmpC);
        tmpC->setEntity(tmpL);
    }
    // Ajoutte les moutons
    for (i = 0; i < moutons; i++) {
        Case* tmpC = plateau[rand()%height][rand()%width].getRandomEmptyCase();
        Mouton* tmpM = new Mouton(tmpC);
        tmpC->setEntity(tmpM);
    }
    // Commencement de la simulation
    mainLoop();
}

// La boucle principale de la simulation
void Ecosysteme::mainLoop() {
    int i, j;
    std::vector<Entity*> l, m; // Vector prenant en compte tout les loups et mouton restant sur le plateau
    print(); // Affichage du début
    while (loups || moutons) { // Tant que le plateau contient toujours au moins 1 entité, on continue
        loups = 0, moutons = 0, turn++;
        for (i = 0; i < height; i++) {
            for (j = 0; j < width; j++) {
                if (plateau[i][j].getEntity() != nullptr) { // On compte et ajoutte tout les entités dans les vectors
                    if (plateau[i][j].getEntity()->getName() == "mouton") {
                        m.push_back(plateau[i][j].getEntity());
                        moutons++;
                    } else {
                        l.push_back(plateau[i][j].getEntity());
                        loups++;
                    }
                }
                plateau[i][j].growGrass(); // Chaque tour, l'herbe pousse un peu si sur chaque case
            }
        }
        // Mouvement de tout les moutons
        for (i = 0; i < moutons; i++)
            m[i]->move();
        // Mouvement de tout les loups
        for (i = 0; i < loups; i++)
            l[i]->move();
        // Action de tout les moutons
        for (i = 0; i < moutons; i++)
            m[i]->action();
        // Action de tout les loups
        for (i = 0; i < loups; i++)
            l[i]->action();
        // On reset les vectors
        l.clear();
        m.clear();
        // Tour suivant (delai d'attente)
        nextTurn();
        // Affichage de la simulation
        print();
    }
    // On sort de la boucle, donc il n'y a plus d'entité
    std::cout << "...FIN DE LA SIMULATION..." << std::endl;
}

// Passe au tour suivant après un delai d'un certain temps (fonction un peu optionelle)
void Ecosysteme::nextTurn() {
    std::this_thread::sleep_for(std::chrono::milliseconds(delai));
}

// Affiche la situation actuelle de la simulation
void Ecosysteme::print() {
    int i, j;
    // Ligne : +---+---+---+---+ ...
    std::cout << "+";
    for (i = 0; i < width; i++)
        std::cout << "---+";
    std::cout << std::endl;

    for (i = 0; i < height; i++) {
        // Ligne : | M | L | S |   | ...
        std::cout << "|";
        for (j = 0; j < width; j++) {
            if (plateau[i][j].getGrass())
                std::cout << '\'' << plateau[i][j].getEntityChar() << "\'|";
            else
                std::cout << ' ' << plateau[i][j].getEntityChar() << " |";
        }
        std::cout << std::endl;
        // Ligne : +---+---+---+---+ ...
        std::cout << "+";
        for (j = 0; j < width; j++)
            std::cout << "---+";
        std::cout << std::endl;
    }
    std::cout << "Tour : " << turn << " | Moutons : " << moutons << " | Loups : " << loups << " | Delai : " << delai << "ms" << std::endl;
}

// Prend en compte les paramètres donné à l'execution, et renvoit les paramètres de la simulation
void init(int nmb, char** args, int settings[5]) {
    if (nmb >= 7) // Personnalisé délai
        settings[4] = atoi(args[6]);
    else // Default délai
        settings[4] = 1000;
    if (nmb >= 6) {
        if (atoi(args[5])) { // Personnalisé nmb loup/mouton (personnalisé : proportion)
            settings[3] = int(((atoi(args[1])*atoi(args[2]))/100)*atoi(args[4]));
            settings[2] = int(((atoi(args[1])*atoi(args[2]))/100)*atoi(args[3]));
        } else { // Personnalisé nmb loup/mouton (personnalisé : pas de proportion)
            settings[3] = atoi(args[4]);
            settings[2] = atoi(args[3]);
        }
    }
    else if (nmb >= 5) { // Personnalisé nmb loup/mouton (default : pas de proportion)
        settings[3] = atoi(args[4]);
        settings[2] = atoi(args[3]);
    }
    else if (nmb >= 4) { // Semi-personnalisé nmb loup/mouton (default : pas de proportion)
        settings[3] = 16;
        settings[2] = atoi(args[3]);
    }
    else { // Default nmb loup/mouton (default : pas de proportion)
        settings[3] = 16;
        settings[2] = 8;
    }
    if (nmb >= 2) { // Personnalisé width/height
        settings[1] = atoi(args[2]);
        settings[0] = atoi(args[1]);
    }
    else if (nmb >= 2) { // Semi-personnalisé width/height
        settings[1] = 8;
        settings[0] = atoi(args[1]);
    }
    else { // Default width/height
        settings[1] = 8;
        settings[0] = 8;
    }
}