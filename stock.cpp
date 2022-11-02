/*
  INF3105 - Structures de données et algorithmes
  UQAM / Département d'informatique
  
  Squelette de départ. Vous pouvez modifier ce fichier à votre guise.
  Vous n'êtes pas oubligés de l'utiliser.
*/

#include "stock.h"
#include <vector>
#include <map>
#include "pile.h"


std::istream& operator >> (std::istream& is, Recette& recette){
  int quantite;
  std::string nom;
  for(;;){ 
    is >> nom ;
    if(nom == ";")
      break;
    is >> quantite;
    if (recette.ingredients.count(nom) > 0){
      recette.ingredients[nom] = recette.ingredients[nom] + quantite;
    }else {
      recette.ingredients.insert(std::pair<std::string, int>(nom, quantite)); 
    }     
  }
  return is;
}

void Stock::ajout(std::string nomingredient, std::string date, int nombre){
  if(ingredients.find(nomingredient)==ingredients.end()){
    Ingredient ing(nomingredient);
    ing.dateExpiration.insert(std::pair<std::string, int>(date, nombre));
    ingredients.insert(std::pair<std::string, std::map<std::string, int>>(nomingredient,ing.dateExpiration));
  }else {
    if(ingredients.find(nomingredient)->second.find(date) != ingredients.find(nomingredient)->second.end()){
      ingredients.find(nomingredient)->second.find(date)->second = ingredients.find(nomingredient)->second.find(date)->second + nombre;
    }else ingredients.find(nomingredient)->second.insert(std::pair<std::string, int>(date, nombre));
  }
}

void Stock::retrait(std::string nomingredient, int nombre){
  std::map<std::string, int>::iterator iter = ingredients.find(nomingredient)->second.begin();
  while (nombre !=0){
      if(iter->second<nombre){
        nombre = nombre - iter->second;
        ingredients.find(nomingredient)->second.erase(iter->first);
      } else if(iter->second>nombre){
        iter->second = iter->second - nombre;
        nombre = 0;
      } else if(iter->second==nombre){
        ingredients.find(nomingredient)->second.erase(iter->first);
        nombre = 0;
      }
      if(ingredients.find(nomingredient)->second.empty()){
        ingredients.erase(nomingredient);
        break;
      }
      ++iter;
  }
}

void Stock::recommendation(std::map<std::string, std::map<std::string, int>> list)
{
  if(!ingredients.empty()){

    Pile<std::string> pile;
    std::string firstPeremptionDate;
    int quantite;
    unsigned int compteur = 0;
    unsigned int compteur2 = 0;
    std::string s2 = "99999999999999999999999999"; //infini
    std::string nom_ing_double;

    while(compteur>=0){ 
      std::string s = "99999999999999999999999999"; //infini
      for(std::map<std::string, std::map<std::string, int>>::iterator iter3=ingredients.begin();iter3!=ingredients.end();++iter3){ //Liste des ingrédients
        for(std::map<std::string, int>::iterator iter1=ingredients.find(iter3->first)->second.begin();iter1!=ingredients.find(iter3->first)->second.end();++iter1){ //Liste des dates et quantité de chaque ingrédients
          if(compteur > compteur2 && iter1->first!=s2 ){
            if(iter1->first<s)
            {
              s=iter1->first;
              firstPeremptionDate = iter3->first;
              quantite = iter1->second;
              nom_ing_double = iter3->first;
            }else if(iter1->first==s && nom_ing_double!=iter3->first)
            {
              s=iter1->first;
              firstPeremptionDate = iter3->first;
              quantite = iter1->second;
            }
          }else if(compteur == compteur2){
            if(iter1->first<=s)
            {
              s=iter1->first;
              firstPeremptionDate = iter3->first;
              quantite = iter1->second;
              nom_ing_double = iter3->first;
            }
          }  
        }
      }
      for(std::map<std::string, std::map<std::string, int>>::iterator iter2=list.begin();iter2!=list.end();++iter2){ //Liste de la recette
        for(std::map<std::string, int>::iterator iter3=iter2->second.begin();iter3!=iter2->second.end();++iter3){ //Liste des ingrédients de la recette
          if(iter3->first==firstPeremptionDate && iter3->second <= quantite && realisable(iter2->first)){
            pile.empiler(iter2->first);
          }
        }
      }
      compteur2 = compteur;
      compteur++;
      s2=s;
      while(!pile.vide())
      {
        std::cout << pile.getSommet() << " ";
        pile.depiler();
        compteur = -1;
      }
      if (compteur>=ingredients.size()){
        break;
      }  
    }
    std::cout << ";\n";
  } 
}

void Stock::affichage(){
    for(std::map<std::string, std::map<std::string, int>>::iterator iter=ingredients.begin();iter!=ingredients.end();++iter){
      std::cout << iter->first << " " ; 
      int quantite = 0;
      for(std::map<std::string, int>::iterator iter2=ingredients.find(iter->first)->second.begin();iter2!=ingredients.find(iter->first)->second.end();++iter2){
        quantite += iter2->second;
      }
      std::cout << quantite << std::endl;
    }
    std::cout << ";" << std::endl;
}

bool Stock::realisable(std::string nomrecette){
  bool realisation = 1;
  if(recettes.count(nomrecette) > 0){
    for(std::map<std::string, int>::iterator iter=recettes.find(nomrecette)->second.begin();iter!=recettes.find(nomrecette)->second.end();++iter){ //Les ingredients qu'il faut pour la recette
      int nombre = iter->second;
      if(ingredients.count(iter->first)>0){
          for(std::map<std::string, int>::iterator iter3=ingredients.find(iter->first)->second.begin();iter3!=ingredients.find(iter->first)->second.end();++iter3){ //Les ingrédients par date du stock
            if(iter3->second < nombre && iter3->second != 0){
              nombre = nombre - iter3->second;
            } else if(iter3->second > nombre){
              nombre = 0;
              realisation = 1;
            } else if(iter3->second == nombre){
              nombre = 0;
              realisation = 1;
            }else ;
          }
          if(nombre != 0){
            realisation = 0;
            return realisation;
          }
      } else return 0;
    }
  }else realisation = 0;
  return realisation;
}

bool Stock::utilisation(std::string nomrecette){
  bool resultat = 1;
  if(realisable(nomrecette)){
    for (std::map < std::string, int > ::iterator iter = recettes.find(nomrecette) -> second.begin(); iter != recettes.find(nomrecette) -> second.end(); ++iter) { //Les ingrédients et les quantités d'une recette
      retrait(iter->first,iter->second);
    }
  } else resultat = 0;
  return resultat;
}

