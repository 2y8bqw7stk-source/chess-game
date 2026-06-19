#include "Plateau.h"

plateau::plateau() {
    initPlateau();
    tour = 1;
    roiBlancBouge = roiNoirBouge = false;
    tourB_G_Bouge = tourB_D_Bouge = false;
    tourN_G_Bouge = tourN_D_Bouge = false;
    dernierLd = dernierCd = dernierLa = dernierCa = -1;
    compteur50Coups = 0;
    _promotionEnAttente = false;
    _promLa = _promCa = -1;
    historiquePositions[serialiser()]++;
}

plateau::~plateau() {}

plateau plateau::copie() const { return *this; }



void plateau::initPlateau() {
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            plat[i][j] = 0;

    for (int j = 0; j < 8; j++) { plat[1][j] = -1; plat[6][j] = 1; }

    plat[0][0]=-2; plat[0][1]=-3; plat[0][2]=-4; plat[0][3]=-5;
    plat[0][4]=-6; plat[0][5]=-4; plat[0][6]=-3; plat[0][7]=-2;

    plat[7][0]=2;  plat[7][1]=3;  plat[7][2]=4;  plat[7][3]=5;
    plat[7][4]=6;  plat[7][5]=4;  plat[7][6]=3;  plat[7][7]=2;
}



void plateau::afficher() {
    cout << "\n    0  1  2  3  4  5  6  7\n  -------------------------\n";
    for (int i = 0; i < 8; i++) {
        cout << i << " ";
        for (int j = 0; j < 8; j++) {
            cout << "|";
            switch (plat[i][j]) {
            case  0: cout << "  "; break;
            case  1: cout << "PB"; break; case -1: cout << "PN"; break;
            case  2: cout << "TB"; break; case -2: cout << "TN"; break;
            case  3: cout << "CB"; break; case -3: cout << "CN"; break;
            case  4: cout << "FB"; break; case -4: cout << "FN"; break;
            case  5: cout << "DB"; break; case -5: cout << "DN"; break;
            case  6: cout << "RB"; break; case -6: cout << "RN"; break;
            }
        }
        cout << "|\n  -------------------------\n";
    }
}

int plateau::estValide(int l, int c) const { return l>=0 && l<8 && c>=0 && c<8; }
int plateau::getPiece(int l, int c)  const { return plat[l][c]; }
int plateau::getTour()               const { return tour; }

string plateau::serialiser() const {
    string s;
    s.reserve(70);
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            s += (char)(plat[i][j] + 10);
    s += (char)((roiBlancBouge ? 1:0)|(roiNoirBouge?2:0)|(tourB_G_Bouge?4:0)|
                 (tourB_D_Bouge?8:0)|(tourN_G_Bouge?16:0)|(tourN_D_Bouge?32:0));
    s += (char)(tour % 2);
    return s;
}



int plateau::eval(int ld, int cd, int la, int ca) {
    if (!estValide(ld,cd) || !estValide(la,ca)) return 0;
    if (plat[ld][cd] == 0) return 0;
    if (plat[ld][cd] * plat[la][ca] > 0) return 0;

    switch (abs(plat[ld][cd])) {
    case 1: return evalPion(ld,cd,la,ca);
    case 2: return evalTour(ld,cd,la,ca);
    case 3: return evalCav(ld,cd,la,ca);
    case 4: return evalFou(ld,cd,la,ca);
    case 5: return evalDame(ld,cd,la,ca);
    case 6: return evalRoi(ld,cd,la,ca);
    }
    return 0;
}

int plateau::evalPion(int ld, int cd, int la, int ca) const {
    int dir = (plat[ld][cd] > 0) ? -1 : 1;
    if (cd==ca && la==ld+dir && plat[la][ca]==0) return 1;
    if (cd==ca && la==ld+2*dir &&
        ((ld==6 && dir==-1)||(ld==1 && dir==1)) &&
        plat[la][ca]==0 && plat[ld+dir][ca]==0) return 1;
    if (abs(cd-ca)==1 && la==ld+dir && plat[la][ca]!=0) return 1;

    if (abs(cd-ca)==1 && la==ld+dir && plat[la][ca]==0) {
        if (abs(plat[ld][ca])==1 &&
            dernierLa==ld && dernierCa==ca &&
            abs(dernierLd-dernierLa)==2) return 1;
    }
    return 0;
}

int plateau::evalTour(int ld, int cd, int la, int ca) const {
    if (ld!=la && cd!=ca) return 0;
    int sL=(la==ld)?0:(la>ld?1:-1);
    int sC=(ca==cd)?0:(ca>cd?1:-1);
    for (int i=ld+sL,j=cd+sC; i!=la||j!=ca; i+=sL,j+=sC)
        if (plat[i][j]!=0) return 0;
    return 1;
}

int plateau::evalCav(int ld, int cd, int la, int ca) const {
    return (abs(ld-la)==2&&abs(cd-ca)==1)||(abs(ld-la)==1&&abs(cd-ca)==2);
}

int plateau::evalFou(int ld, int cd, int la, int ca) const {
    if (abs(ld-la)!=abs(cd-ca)) return 0;
    int sL=(la>ld)?1:-1, sC=(ca>cd)?1:-1;
    for (int i=ld+sL,j=cd+sC; i!=la; i+=sL,j+=sC)
        if (plat[i][j]!=0) return 0;
    return 1;
}

int plateau::evalDame(int ld, int cd, int la, int ca) const {
    return evalTour(ld,cd,la,ca) || evalFou(ld,cd,la,ca);
}

int plateau::roquePossible(int ld, int cd, int la, int ca) {
    if (abs(plat[ld][cd])!=6) return 0;
    int joueur = (plat[ld][cd]>0) ? 1 : -1;
    if (estEnEchec(joueur)) return 0;

    if (joueur==1 && !roiBlancBouge) {
        if (la==7 && ca==6 && !tourB_D_Bouge && plat[7][5]==0 && plat[7][6]==0) {
            plateau s=*this; s.plat[7][5]=6; s.plat[7][4]=0;
            if (!s.estEnEchec(1)) return 1;
        }
        if (la==7 && ca==2 && !tourB_G_Bouge && plat[7][1]==0 && plat[7][2]==0 && plat[7][3]==0) {
            plateau s=*this; s.plat[7][3]=6; s.plat[7][4]=0;
            if (!s.estEnEchec(1)) return 1;
        }
    }
    if (joueur==-1 && !roiNoirBouge) {
        if (la==0 && ca==6 && !tourN_D_Bouge && plat[0][5]==0 && plat[0][6]==0) {
            plateau s=*this; s.plat[0][5]=-6; s.plat[0][4]=0;
            if (!s.estEnEchec(-1)) return 1;
        }
        if (la==0 && ca==2 && !tourN_G_Bouge && plat[0][1]==0 && plat[0][2]==0 && plat[0][3]==0) {
            plateau s=*this; s.plat[0][3]=-6; s.plat[0][4]=0;
            if (!s.estEnEchec(-1)) return 1;
        }
    }
    return 0;
}

int plateau::evalRoi(int ld, int cd, int la, int ca) {
    if (abs(ld-la)<=1 && abs(cd-ca)<=1) return 1;
    if (abs(cd-ca)==2) return roquePossible(ld,cd,la,ca);
    return 0;
}



void plateau::deplacerPiece(int ld, int cd, int la, int ca, int promotionPiece) {
    bool estCapture   = (plat[la][ca] != 0);
    bool estPionBouge = (abs(plat[ld][cd]) == 1);


    if (abs(plat[ld][cd])==6 && abs(cd-ca)==2) {
        if (ca==6) { plat[ld][5]=plat[ld][7]; plat[ld][7]=0; }
        if (ca==2) { plat[ld][3]=plat[ld][0]; plat[ld][0]=0; }
    }


    if (abs(plat[ld][cd])==1 && cd!=ca && plat[la][ca]==0) {
        plat[ld][ca]=0;
        estCapture = true;
    }


    if (ld==7&&cd==4) roiBlancBouge=true;
    if (ld==0&&cd==4) roiNoirBouge=true;
    if (ld==7&&cd==0) tourB_G_Bouge=true;
    if (ld==7&&cd==7) tourB_D_Bouge=true;
    if (ld==0&&cd==0) tourN_G_Bouge=true;
    if (ld==0&&cd==7) tourN_D_Bouge=true;

    plat[la][ca] = plat[ld][cd];
    plat[ld][cd] = 0;

    dernierLd=ld; dernierCd=cd; dernierLa=la; dernierCa=ca;


    if (abs(plat[la][ca])==1 && (la==0 || la==7)) {
        if (promotionPiece>=2 && promotionPiece<=5) {
            plat[la][ca] = (plat[la][ca]>0) ? promotionPiece : -promotionPiece;
            _promotionEnAttente = false;
        } else {
            _promotionEnAttente = true;
            _promLa = la; _promCa = ca;
        }
    }

    if (estCapture || estPionBouge) compteur50Coups = 0;
    else compteur50Coups++;

    tour++;

    if (!_promotionEnAttente)
        historiquePositions[serialiser()]++;
}

void plateau::finaliserPromotion(int piece) {
    if (!_promotionEnAttente) return;
    int couleur = (plat[_promLa][_promCa] > 0) ? 1 : -1;
    plat[_promLa][_promCa] = couleur * piece;
    _promotionEnAttente = false;
    historiquePositions[serialiser()]++;
}



int plateau::estEnEchec(int joueur) const {
    int rl=-1, rc=-1;
    int roiCode = (joueur==1) ? 6 : -6;
    for (int i=0; i<8 && rl==-1; i++)
        for (int j=0; j<8; j++)
            if (plat[i][j]==roiCode) { rl=i; rc=j; break; }
    if (rl==-1) return 0;

    for (int i=0; i<8; i++) {
        for (int j=0; j<8; j++) {
            int p = plat[i][j];
            if ((joueur==1 && p<0) || (joueur==-1 && p>0)) {
                if (abs(p)==6) {
                    if (abs(i-rl)<=1 && abs(j-rc)<=1) return 1;
                } else {

                    plateau tmp = *this;
                    if (tmp.eval(i,j,rl,rc)) return 1;
                }
            }
        }
    }
    return 0;
}

bool plateau::coupLaisseEnEchec(int ld, int cd, int la, int ca, int joueur) {
    plateau s = *this;
    s.deplacerPiece(ld,cd,la,ca,5);
    return s.estEnEchec(joueur);
}

int plateau::estEchecMat(int joueur) {

    if (!estEnEchec(joueur)) return 0;
    return estPat(joueur) ? 1 : 0;
}

bool plateau::estPat(int joueur) {
    for (int ld=0; ld<8; ld++) {
        for (int cd=0; cd<8; cd++) {
            if ((joueur==1&&plat[ld][cd]>0)||(joueur==-1&&plat[ld][cd]<0)) {
                for (int la=0; la<8; la++) {
                    for (int ca=0; ca<8; ca++) {
                        if (eval(ld,cd,la,ca)) {
                            if (!coupLaisseEnEchec(ld,cd,la,ca,joueur))
                                return false;
                        }
                    }
                }
            }
        }
    }
    return true;
}



FinPartieType plateau::etatPartie() {
    if (compteur50Coups >= 100) return NULLE_50_COUPS;

    for (auto& kv : historiquePositions)
        if (kv.second >= 3) return NULLE_REPETITION;

    int joueurActuel = (tour%2!=0) ? 1 : -1;

    if (estPat(joueurActuel)) {
        if (estEnEchec(joueurActuel))

            return (joueurActuel==1) ? ECHEC_MAT_BLANC : ECHEC_MAT_NOIR;
        else
            return PAT;
    }
    return EN_COURS;
}
int plateau::evaluerPosition() const {
    static const int valeurs[7] = {0, 10, 50, 32, 33, 90, 900};
    static const int bonusPion[8][8] = {
        { 0, 0, 0, 0, 0, 0, 0, 0},
        { 5, 5, 5, 5, 5, 5, 5, 5},
        { 1, 1, 2, 3, 3, 2, 1, 1},
        { 0, 0, 0, 2, 2, 0, 0, 0},
        { 0, 0, 0, 2, 2, 0, 0, 0},
        { 0,-1,-2, 0, 0,-2,-1, 0},
        { 0, 1, 1,-2,-2, 1, 1, 0},
        { 0, 0, 0, 0, 0, 0, 0, 0}
    };
    static const int bonusCav[8][8] = {
        {-5,-4,-3,-3,-3,-3,-4,-5},
        {-4,-2, 0, 0, 0, 0,-2,-4},
        {-3, 0, 1, 2, 2, 1, 0,-3},
        {-3, 0, 2, 3, 3, 2, 0,-3},
        {-3, 0, 2, 3, 3, 2, 0,-3},
        {-3, 0, 1, 2, 2, 1, 0,-3},
        {-4,-2, 0, 0, 0, 0,-2,-4},
        {-5,-4,-3,-3,-3,-3,-4,-5}
    };

    int score = 0;
    for (int i=0; i<8; i++) {
        for (int j=0; j<8; j++) {
            int p = plat[i][j];
            if (p==0) continue;
            int type = abs(p);
            int base = valeurs[type];
            int bonus = 0;
            if (type==1) bonus = bonusPion[(p>0)?(7-i):i][j];
            if (type==3) bonus = bonusCav[i][j];
            score += (p>0) ? (base+bonus) : -(base+bonus);
        }
    }
    return score;
}

bool plateau::estCaseAttaquee(int l, int c, int attaquant) const {
    plateau tmp = *this;
    for (int i=0; i<8; i++)
        for (int j=0; j<8; j++)
            if ((attaquant==1&&plat[i][j]>0)||(attaquant==-1&&plat[i][j]<0))
                if (tmp.eval(i,j,l,c)) return true;
    return false;
}
