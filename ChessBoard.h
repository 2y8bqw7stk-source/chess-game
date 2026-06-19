#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QTimer>
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QVector>
#include "Plateau.h"

struct Move {
    int ld, cd, la, ca;
    Move(int a,int b,int c,int d):ld(a),cd(b),la(c),ca(d){}
};

class ChessBoard : public QWidget {
    Q_OBJECT
public:
    explicit ChessBoard(QWidget *parent = nullptr);

    void setGameMode(int mode);
    void setDifficulty(int niv);
    void setPlayerColor(int couleur);
    void resetGame();

    int  getJoueurActuel() const;
    bool isGameOver() const;
    // BUG FIX #7: retourne la couleur de la pièce en cours de promotion (1=blanc, -1=noir)
    int  getPromotingPlayer() const;

signals:
    void statusChanged(const QString &msg);
    void gameOver(const QString &result);
    void promotionRequired(int la, int ca);

public slots:
    void doIAMove();
    void applyPromotion(int piece);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    plateau jeu;
    int     mode;
    int     niveauIA;
    int     couleurIA;

    int     selectedL, selectedC;
    bool    pieceSelected;

    int     cellSize;
    int     boardOffsetX, boardOffsetY;

    bool    gameOverFlag;
    bool    promotionPending;

    QVector<Move> legalMoves;
    QVector<Move> highlightedMoves;

    static const QColor COLOR_LIGHT;
    static const QColor COLOR_DARK;
    static const QColor COLOR_SELECT;
    static const QColor COLOR_MOVE;
    static const QColor COLOR_CAPTURE;
    static const QColor COLOR_CHECK;

    void drawBoard(QPainter &p);
    void drawPieces(QPainter &p);
    void drawPiece(QPainter &p, int piece, int l, int c);
    void drawHighlights(QPainter &p);
    QString pieceUnicode(int piece) const;

    QVector<Move> getLegalMoves(int l, int c);
    bool isMoveInList(int la, int ca, const QVector<Move> &list) const;
    void tryMove(int la, int ca);
    void checkGameState();
    void updateStatus();

    struct Coup {
        int ld,cd,la,ca,score;
        bool valid;
        Coup():ld(0),cd(0),la(0),ca(0),score(0),valid(false){}
        Coup(int a,int b,int c,int d,int s):ld(a),cd(b),la(c),ca(d),score(s),valid(true){}
    };
    QVector<Coup> listerCoupsLegaux(plateau &p, int joueur);
    int  minimax(plateau p, int prof, int alpha, int beta, bool estMax);
    Coup choisirCoupIA();

    QPoint cellToPixel(int l, int c) const;
    bool   pixelToCell(QPoint pos, int &l, int &c) const;

    int    visualToBoard(int v) const;
    int    boardToVisual(int b) const;
};

#endif
