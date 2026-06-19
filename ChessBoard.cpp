#include "ChessBoard.h"
#include <QTimer>
#include <QPainterPath>
#include <cstdlib>
#include <ctime>
#include <algorithm>

const QColor ChessBoard::COLOR_LIGHT  (240, 217, 181);
const QColor ChessBoard::COLOR_DARK   (181, 136,  99);
const QColor ChessBoard::COLOR_SELECT (  0, 150, 200, 180);
const QColor ChessBoard::COLOR_MOVE   (  0, 200, 100,  90);
const QColor ChessBoard::COLOR_CAPTURE(220,  50,  50, 120);
const QColor ChessBoard::COLOR_CHECK  (255,  30,  30, 160);

ChessBoard::ChessBoard(QWidget *parent)
    : QWidget(parent)
    , mode(2), niveauIA(2), couleurIA(-1)
    , selectedL(-1), selectedC(-1), pieceSelected(false)
    , cellSize(80), boardOffsetX(0), boardOffsetY(0)
    , gameOverFlag(false), promotionPending(false)
{
    srand((unsigned)time(nullptr));
    setMinimumSize(400, 400);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void ChessBoard::setGameMode(int m)    { mode = m; }
void ChessBoard::setDifficulty(int n)  { niveauIA = n; }
void ChessBoard::setPlayerColor(int c) { couleurIA = (c==1) ? -1 : 1; }

void ChessBoard::resetGame() {
    jeu = plateau();
    selectedL = selectedC = -1;
    pieceSelected = false;
    gameOverFlag = false;
    promotionPending = false;
    legalMoves.clear();
    highlightedMoves.clear();
    update();
    updateStatus();

    if (mode==2 && couleurIA==1)
        QTimer::singleShot(500, this, &ChessBoard::doIAMove);
}


int ChessBoard::visualToBoard(int v) const {

    return (mode == 2 && couleurIA == 1) ? (7 - v) : v;
}
int ChessBoard::boardToVisual(int b) const {
    return (mode == 2 && couleurIA == 1) ? (7 - b) : b;
}



void ChessBoard::resizeEvent(QResizeEvent *) {
    update();
}

void ChessBoard::paintEvent(QPaintEvent *) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    drawBoard(p);
    drawHighlights(p);
    drawPieces(p);
}

void ChessBoard::drawBoard(QPainter &p) {
    int cellW = width() / 8;
    int cellH = height() / 8;

    for (int vi=0; vi<8; vi++) {
        for (int vj=0; vj<8; vj++) {
            bool light = (vi+vj)%2==0;
            p.setBrush(light ? COLOR_LIGHT : COLOR_DARK);
            p.setPen(Qt::NoPen);
            p.drawRect(vj*cellW, vi*cellH, cellW, cellH);
        }
    }
}

void ChessBoard::drawHighlights(QPainter &p) {
    p.setPen(Qt::NoPen);
    int cellW = width() / 8;
    int cellH = height() / 8;

    if (pieceSelected) {

        int vi = boardToVisual(selectedL);
        int vj = boardToVisual(selectedC);
        p.setBrush(COLOR_SELECT);
        p.drawRect(vj*cellW, vi*cellH, cellW, cellH);
    }

    int joueur = (jeu.getTour()%2!=0) ? 1 : -1;
    if (jeu.estEnEchec(joueur)) {
        for (int i=0; i<8; i++) {
            for (int j=0; j<8; j++) {
                int code = (joueur==1) ? 6 : -6;
                if (jeu.getPiece(i,j)==code) {
                    int vi = boardToVisual(i);
                    int vj = boardToVisual(j);
                    p.setBrush(COLOR_CHECK);
                    p.drawRect(vj*cellW, vi*cellH, cellW, cellH);
                }
            }
        }
    }

    for (const Move &m : highlightedMoves) {
        bool isCapture = (jeu.getPiece(m.la, m.ca) != 0);
        p.setBrush(isCapture ? COLOR_CAPTURE : COLOR_MOVE);

        int vi = boardToVisual(m.la);
        int vj = boardToVisual(m.ca);
        if (isCapture) {
            p.drawEllipse(vj*cellW + 4, vi*cellH + 4, cellW - 8, cellH - 8);
        } else {
            int r = qMin(cellW, cellH) / 4;
            p.drawEllipse(vj*cellW + cellW/2 - r/2, vi*cellH + cellH/2 - r/2, r, r);
        }
    }
}

void ChessBoard::drawPieces(QPainter &p) {
    int cellW = width() / 8;
    int cellH = height() / 8;
    QFont f("Segoe UI Symbol", qMin(cellW, cellH)*55/100);
    f.setStyleStrategy(QFont::PreferDefault);
    p.setFont(f);

    for (int i=0; i<8; i++) {
        for (int j=0; j<8; j++) {
            int piece = jeu.getPiece(i,j);
            if (piece==0) continue;
         
            drawPiece(p, piece, i, j);
        }
    }
}

void ChessBoard::drawPiece(QPainter &p, int piece, int l, int c) {
    QString sym = pieceUnicode(piece);
    bool blanc = (piece > 0);

    int cellW = width() / 8;
    int cellH = height() / 8;

    int vl = boardToVisual(l);
    int vc = boardToVisual(c);
    int x = vc*cellW;
    int y = vl*cellH;
    QRect rect(x, y, cellW, cellH);

    QFont f("Segoe UI Symbol", qMin(cellW, cellH)*55/100);
    p.setFont(f);

    p.setPen(blanc ? QColor(40,40,40,200) : QColor(220,220,220,180));
    QRect shadow = rect.translated(1,1);
    p.drawText(shadow, Qt::AlignCenter, sym);

    p.setPen(blanc ? QColor(255,255,255) : QColor(20,20,20));
    p.drawText(rect, Qt::AlignCenter, sym);
}

QString ChessBoard::pieceUnicode(int piece) const {
    switch(piece) {
    case  6: return "\u2654"; case -6: return "\u265A";
    case  5: return "\u2655"; case -5: return "\u265B";
    case  2: return "\u2656"; case -2: return "\u265C";
    case  4: return "\u2657"; case -4: return "\u265D";
    case  3: return "\u2658"; case -3: return "\u265E";
    case  1: return "\u2659"; case -1: return "\u265F";
    }
    return " ";
}



bool ChessBoard::pixelToCell(QPoint pos, int &l, int &c) const {
    int cellW = width() / 8;
    int cellH = height() / 8;
    if (cellW <= 0 || cellH <= 0) return false;

    int vc = pos.x() / cellW;
    int vl = pos.y() / cellH;

    if (vc < 0 || vc >= 8 || vl < 0 || vl >= 8) return false;


    l = visualToBoard(vl);
    c = visualToBoard(vc);
    return true;
}

QPoint ChessBoard::cellToPixel(int l, int c) const {
    int cellW = width() / 8;
    int cellH = height() / 8;
    return QPoint(c*cellW, l*cellH);
}

void ChessBoard::mousePressEvent(QMouseEvent *event) {
    if (gameOverFlag || promotionPending) return;

    int joueur = (jeu.getTour()%2!=0) ? 1 : -1;
    if (mode==2 && joueur==couleurIA) return;

    int l, c;
    if (!pixelToCell(event->pos(), l, c)) return;

    if (!pieceSelected) {
        int piece = jeu.getPiece(l,c);
        if ((joueur==1 && piece>0) || (joueur==-1 && piece<0)) {
            selectedL = l; selectedC = c;
            pieceSelected = true;
            highlightedMoves = getLegalMoves(l,c);
        }
    } else {
        if (l==selectedL && c==selectedC) {
            pieceSelected = false;
            highlightedMoves.clear();
        } else if (isMoveInList(l, c, highlightedMoves)) {
            tryMove(l, c);
        } else {
            int piece = jeu.getPiece(l,c);
            if ((joueur==1 && piece>0) || (joueur==-1 && piece<0)) {
                selectedL=l; selectedC=c;
                highlightedMoves = getLegalMoves(l,c);
            } else {
                pieceSelected = false;
                highlightedMoves.clear();
            }
        }
    }
    update();
}


QVector<ChessBoard::Coup> ChessBoard::listerCoupsLegaux(plateau &p, int joueur) {
    QVector<Coup> liste;
    for (int ld=0; ld<8; ld++) {
        for (int cd=0; cd<8; cd++) {
            if ((joueur==1&&p.getPiece(ld,cd)>0)||(joueur==-1&&p.getPiece(ld,cd)<0)) {
                for (int la=0; la<8; la++) {
                    for (int ca=0; ca<8; ca++) {
                        if (p.eval(ld,cd,la,ca)) {
                            plateau simu = p.copie();
                            simu.deplacerPiece(ld,cd,la,ca,5);
                            if (!simu.estEnEchec(joueur)) {
                                Coup cp; cp.ld=ld; cp.cd=cd; cp.la=la; cp.ca=ca; cp.score=0; cp.valid=true;
                                liste.push_back(cp);
                            }
                        }
                    }
                }
            }
        }
    }
    return liste;
}

QVector<Move> ChessBoard::getLegalMoves(int l, int c) {
    QVector<Move> moves;
    int joueur = (jeu.getPiece(l,c)>0) ? 1 : -1;
    for (int la=0; la<8; la++) {
        for (int ca=0; ca<8; ca++) {
            if (jeu.eval(l,c,la,ca)) {
                plateau simu = jeu.copie();
                simu.deplacerPiece(l,c,la,ca,5);
                if (!simu.estEnEchec(joueur))
                    moves.push_back(Move(l,c,la,ca));
            }
        }
    }
    return moves;
}

bool ChessBoard::isMoveInList(int la, int ca, const QVector<Move> &list) const {
    for (const Move &m : list)
        if (m.la==la && m.ca==ca) return true;
    return false;
}

void ChessBoard::tryMove(int la, int ca) {
    int piece = jeu.getPiece(selectedL, selectedC);
    bool estPionPromotion = (abs(piece)==1 && (la==0||la==7));

    pieceSelected = false;
    highlightedMoves.clear();

    if (estPionPromotion) {
        jeu.deplacerPiece(selectedL, selectedC, la, ca, -1);
        promotionPending = true;
        update();
        emit promotionRequired(la, ca);
    } else {
        jeu.deplacerPiece(selectedL, selectedC, la, ca, 5);
        update();
        checkGameState();

        if (!gameOverFlag && mode==2) {
            QTimer::singleShot(300, this, &ChessBoard::doIAMove);
        }
    }
}

void ChessBoard::applyPromotion(int piece) {
    jeu.finaliserPromotion(piece);
    promotionPending = false;
    update();
    checkGameState();
    if (!gameOverFlag && mode==2)
        QTimer::singleShot(300, this, &ChessBoard::doIAMove);
}

void ChessBoard::checkGameState() {
    FinPartieType etat = jeu.etatPartie();
    if (etat == EN_COURS) {
        updateStatus();
        return;
    }
    gameOverFlag = true;
    QString msg;
    switch(etat) {
    case ECHEC_MAT_BLANC:  msg="\u2654 \u00C9chec et mat ! Les Blancs gagnent."; break;
    case ECHEC_MAT_NOIR:   msg="\u265A \u00C9chec et mat ! Les Noirs gagnent.";  break;
    case PAT:              msg="\u00BD Pat ! Match nul.";                        break;
    case NULLE_50_COUPS:   msg="\u00BD Nulle \u2014 r\u00E8gle des 50 coups.";  break;
    case NULLE_REPETITION: msg="\u00BD Nulle \u2014 triple r\u00E9p\u00E9tition."; break;
    default: break;
    }
    emit gameOver(msg);
}

void ChessBoard::updateStatus() {
    int joueur = (jeu.getTour()%2!=0) ? 1 : -1;
    QString who = (joueur==1) ? "Blancs \u2659" : "Noirs \u265F";
    QString msg = "Tour des " + who;
    if (jeu.estEnEchec(joueur)) msg += " \u2014 \u26A0 \u00C9CHEC !";
    if (mode==2 && joueur==couleurIA) msg += " (IA r\u00E9fl\u00E9chit\u2026)";
    emit statusChanged(msg);
}

int ChessBoard::getJoueurActuel() const {
    return (jeu.getTour()%2!=0) ? 1 : -1;
}

bool ChessBoard::isGameOver() const { return gameOverFlag; }

int ChessBoard::getPromotingPlayer() const {

    if (!jeu.promotionEnAttente()) return 0;
    int piece = jeu.getPiece(jeu.promotionLigne(), jeu.promotionColonne());
    return (piece > 0) ? 1 : -1;
}



int ChessBoard::minimax(plateau p, int prof, int alpha, int beta, bool estMax) {
    if (prof<=0) return p.evaluerPosition();
    if (estMax) {
        int best = -100000;
        QVector<Coup> coups = listerCoupsLegaux(p,1);
        if (coups.empty()) return p.estEnEchec(1) ? -50000 : 0;
        for (const Coup &c : coups) {
            plateau s = p.copie();
            s.deplacerPiece(c.ld,c.cd,c.la,c.ca,5);
            int val = minimax(s, prof-1, alpha, beta, false);
            best = qMax(best, val);
            alpha = qMax(alpha, val);
            if (beta<=alpha) break;
        }
        return best;
    } else {
        int best = 100000;
        QVector<Coup> coups = listerCoupsLegaux(p,-1);
        if (coups.empty()) return p.estEnEchec(-1) ? 50000 : 0;
        for (const Coup &c : coups) {
            plateau s = p.copie();
            s.deplacerPiece(c.ld,c.cd,c.la,c.ca,5);
            int val = minimax(s, prof-1, alpha, beta, true);
            best = qMin(best, val);
            beta = qMin(beta, val);
            if (beta<=alpha) break;
        }
        return best;
    }
}

ChessBoard::Coup ChessBoard::choisirCoupIA() {
    int prof = (niveauIA>=3) ? 3 : 2;
    QVector<Coup> coups = listerCoupsLegaux(jeu, couleurIA);
    if (coups.empty()) return Coup();

    QVector<Coup> meilleurs;
    int bScore = (couleurIA==1) ? -100000 : 100000;

    for (Coup &c : coups) {
        plateau simu = jeu.copie();
        simu.deplacerPiece(c.ld,c.cd,c.la,c.ca,5);

        bool nextIsMax = (couleurIA == 1) ? false : true;
        int val = minimax(simu, prof, -100000, 100000, nextIsMax);
        if ((couleurIA==1 && val>bScore) || (couleurIA==-1 && val<bScore)) {
            bScore=val; meilleurs.clear(); meilleurs.push_back(c);
        } else if (val==bScore) {
            meilleurs.push_back(c);
        }
    }
    return meilleurs[rand() % meilleurs.size()];
}

void ChessBoard::doIAMove() {
    if (gameOverFlag || promotionPending) return;
    int joueur = (jeu.getTour()%2!=0) ? 1 : -1;
    if (joueur != couleurIA) return;

    Coup c = choisirCoupIA();

    if (!c.valid) return;

    jeu.deplacerPiece(c.ld,c.cd,c.la,c.ca,5);
    update();
    checkGameState();
}
