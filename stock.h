/*
  INF3105 - Structures de données et algorithmes
  UQAM / Département d'informatique
  
  Squelette de départ. Vous pouvez modifier ce fichier à votre guise.
  Vous n'êtes pas oubligés de l'utiliser.
*/
#ifndef STOCK_H
#define STOCK_H
#include <iostream>
#include <map>

class Recette {
  public:
    std::string nomrecette;
    std::map<std::string, int> ingredients;

    Recette(std::string nom){
      nomrecette = nom;
    }

    Recette(std::string nom,std::map<std::string, int> listIngredients){
      nomrecette = nom;
      ingredients = listIngredients;
    }

    
  friend std::istream& operator >> (std::istream& is, Recette& recette);
};

//Création de la classe ingrédient pour l'ajout
class Ingredient {
  public:
    std::string nomingredient;
    std::map<std::string, int> dateExpiration;

    Ingredient(std::string nom){
      nomingredient = nom;
    }

    Ingredient(std::string nom,std::map<std::string, int> listdateExpiration){
      nomingredient = nom;
      dateExpiration = listdateExpiration;
    }
};


class Stock
{
  
  public:
    std::map<std::string, std::map<std::string, int>> recettes;
    std::map<std::string,std::map<std::string, int>> ingredients;

    void ajout(std::string nomingredient, std::string date, int nombre);

    void retrait(std::string nomingredient, int nombre);

    void recommendation(std::map<std::string,std::map<std::string, int>> list);

    void affichage();

    bool realisable(std::string nomrecette);
    
    bool utilisation(std::string nomrecette);


 
};

#endif // STOCK_H