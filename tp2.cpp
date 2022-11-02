/*
  INF3105 - Structures de données et algorithmes
  UQAM / Département d'informatique
  
  Squelette de départ. Vous pouvez modifier ce fichier à votre guise.
  Vous n'êtes pas oubligés de l'utiliser.
*/

#include <fstream>
#include "stock.h"


int main(int argc, const char** argv) {
    std::istream* entree;
    std::ifstream* entree_fichier = NULL;
    if(argc>1){
         entree = entree_fichier = new std::ifstream(argv[1]);
         if(entree->fail())
          std::cerr << "Erreur d'ouverture du fichier '" << argv[1] << "'" << std::endl;
    } else {
         entree = &std::cin;
    }

  Stock stock;
    while(*entree){
        std::string commande;
        *entree >> commande;
        if(!(*entree)) break;
        if(commande=="recette"){
            std::string nomrecette;
            *entree >> nomrecette;
            if (stock.recettes.count(nomrecette) > 0){
              stock.recettes.erase(nomrecette); // On enlève l'ancienne recette pour rajouter la nouvelle
            }
            Recette recette(nomrecette);
            
            *entree >> recette;
            stock.recettes[recette.nomrecette] = recette.ingredients;

        } else if(commande=="ajout") { 
            std::string nomingredient;
            *entree >> nomingredient;
            int nombre = 0;
            *entree >> nombre;
            if( nombre <= 0) {
                std::cerr << "Erreur sur le nombre d'ingredient." << std::endl;
                return -1;
            }
            std::string date;
            *entree >> date;
            stock.ajout(nomingredient, date, nombre);
            
        } else if(commande=="retrait") { 

            std::string nomingredient; 
            *entree >> nomingredient;
            std::cerr << "retrait " << nomingredient << std::endl;
            int nombre = 0;
            *entree >> nombre;
            if( nombre <= 0) {
                std::cerr << "Erreur sur le nombre d'ingredient." << std::endl;
                return -1;
            }
            stock.retrait(nomingredient, nombre);

        } else if(commande=="recommendation") { 
            
            stock.recommendation(stock.recettes);

        } else if(commande=="utilisation") { 
            std::string nomrecette;
            *entree >> nomrecette;
            std::cout << nomrecette << " : " << stock.utilisation(nomrecette) << std::endl;

        } else if(commande=="affichage") {
          /*for(std::map<std::string, std::map<std::string, int>>::iterator iter=stock.ingredients.begin();iter!=stock.ingredients.end();++iter){
            if(iter->first == "citron"){
                for(std::map<std::string, int>::iterator iter2=stock.ingredients.find(iter->first)->second.begin();iter2!=stock.ingredients.find(iter->first)->second.end();++iter2){
                  std::cout << iter2->first << " : " << iter2->second << std::endl; 
            }
            }
          }*/
          stock.affichage();
        } else {
            std::cerr << "commande \"" << commande << "\" non reconnue !" << std::endl; 
        }
    }
    delete entree_fichier;


    return 0;
}
