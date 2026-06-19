#ifndef PLATEAU_H
#define PLATEAU_H

#include <iostream>
#include <cmath>
#include <map>
#include <string>
using namespace std;

enum FinPartieType {
    EN_COURS = 0,
    ECHEC_MAT_BLANC,
    ECHEC_MAT_NOIR,
    PAT,
    NULLE_50_COUPS,
    NULLE_REPETITION
};

class plateau {
public:
    plateau();
    ~plateau();

    void initPlateau();
    void afficher();

    int eval(int ld, int cd, int la, int ca);
    void deplacerPiece(int ld, int cd, int la, int ca, int promotionPiece = 5);

    int getPiece(int ld, int cd) const;
    int getTour() const;

    FinPartieType etatPartie();
    int estValide(int l, int c) const;

    int evalCav(int ld, int cd, int la, int ca) const;
    int evalPion(int ld, int cd, int la, int ca) const;
    int evalTour(int ld, int cd, int la, int ca) const;
    int evalFou(int ld, int cd, int la, int ca) const;
    int evalDame(int ld, int cd, int la, int ca) const;
    int evalRoi(int ld, int cd, int la, int ca);

    int roquePossible(int ld, int cd, int la, int ca);
    int estEnEchec(int joueur) const;
    int estEchecMat(int joueur);
    bool estPat(int joueur);

    bool coupLaisseEnEchec(int ld, int cd, int la, int ca, int joueur);

    int evaluerPosition() const;
    bool estCaseAttaquee(int l, int c, int attaquant) const;

    bool promotionEnAttente() const { return _promotionEnAttente; }
    int  promotionLigne()    const  { return _promLa; }
    int  promotionColonne()  const  { return _promCa; }
    void finaliserPromotion(int piece);

    plateau copie() const;

private:
    int plat[8][8];
    int tour;

    int dernierLd, dernierCd, dernierLa, dernierCa;
    bool roiBlancBouge, roiNoirBouge;
    bool tourB_G_Bouge, tourB_D_Bouge;
    bool tourN_G_Bouge, tourN_D_Bouge;

    int compteur50Coups;
    map<string, int> historiquePositions;

    bool _promotionEnAttente;
    int  _promLa, _promCa;

    string serialiser() const;
};

#endif
