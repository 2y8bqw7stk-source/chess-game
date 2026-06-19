#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDialog>
#include <QMenuBar>
#include <QMessageBox>
#include <QStatusBar>
#include <QTimer>
#include <QKeyEvent>
#include "ChessBoard.h"

class PromotionDialog : public QDialog {
    Q_OBJECT
public:
    explicit PromotionDialog(int joueur, QWidget *parent = nullptr);
    int pieceChoisie() const { return _piece; }
private:
    int _piece;
};

class StartDialog : public QDialog {
    Q_OBJECT
public:
    explicit StartDialog(QWidget *parent = nullptr);
    int getMode()       const;
    int getDifficulte() const;
    int getCouleur()    const;
    int getTimerMode()  const;
private:
    QComboBox  *cbMode, *cbDiff, *cbCouleur, *cbTimer;
    QPushButton *btnStart;
    QGroupBox  *gbDiff, *gbCouleur;
};

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

protected:
    // BUG FIX: ajout keyPressEvent pour F11
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void startNewGame();
    void onStatusChanged(const QString &msg);
    void onGameOver(const QString &result);
    void onPromotionRequired(int la, int ca);
    void showAbout();
    void tickTimer();
    void toggleFullscreen();

private:
    ChessBoard  *board;
    QLabel      *lblStatus;
    QLabel      *lblTour;
    QLabel      *lblTimerWhite;
    QLabel      *lblTimerBlack;
    QGroupBox   *gbTimer;
    QPushButton *btnNew;
    QPushButton *btnAbout;
    QPushButton *btnFullscreen;
    QTimer      *gameTimer;
    int          timerWhiteSecs;
    int          timerBlackSecs;
    int          timerInitSecs;
    bool         timerEnabled;

    void setupUI();
    void applyStyle();
    void updateTimerDisplay();
};

#endif // MAINWINDOW_H
