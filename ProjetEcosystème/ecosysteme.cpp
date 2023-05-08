// PROJET ECOSYSTÈME
// Aguesse Nathan L2-Y N°21001877
// Sinnaththurai Sriram L2-Y N°21002831

/*
CLASS :

class entity; Classe de base pour les entités du jeu
Attributs :
Espérence de vie restante
Sexe
Faim restante
Méthodes :
Reproduction

class loup; héritage entity + fonctionalité du loup
Attributs :
Méthodes :
eat - Mange mouton si a coté
move - Se déplace aléatoirement
(lorsque mouton a coté, le marque en proie pour le chasser jusqu'à besoin de manger)

class mouton; héritage entity + fonctionalité du mouton
Attributs :
Méthodes :
eat - Mange herbe si dessus
move - Se déplace aléatoirement

class case; Classe pour chaque case
Attributs :
Méthodes :
Entité sur la case
Herbe sur la case
Sel sur la case
Converti sel en herbe si aucune herbe sur la case

class ecosysteme; Classe pour l'ecosystème
Attributs :
nmb tour
nmb loup
nmb moutons
dimmensions
plateau (avec toutes les objets case)
Méthodes :
start
nextTurn - tout les entités vont bouger, puis vont manger si besoin (et herbe pousse si sel)
print

FUNCTION :

int[4] init(int nmb, char** args);
va prendre les arguments lors de l'execution pour avoir les paramètres de la simulation
1 : Longeur : x : default = 8
2 : Hauteur : y : default = 8
3 : Nmb moutons : m (si proportion, m devient le pourcentage de la map ou il faudra des moutons) : default = x+y
4 : Nmb loups : l (si proportion, l devient le pourcentage de la map ou il faudra des loups) : default = (x+y)/2
5 : Proportion activé : p : default = 0
6 : délai temps entre chaque tour : t : default = 1000


int main(int argc, char* argv[]) {
    int settings[4] = init(argc, argv);
    Ecosysteme simulation {settings};
    simulation.start();
    return 0;
}

*/

#include "entity.hpp"
#include "system.hpp"


int main(int argc, char* argv[]) {
    int settings[5];
    init(argc, argv, settings);
    Ecosysteme simulation {settings};
    simulation.start();
    return 0;
}