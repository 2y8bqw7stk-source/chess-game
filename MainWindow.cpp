#include "MainWindow.h"
#include <QApplication>
#include <QScreen>
#include <QFont>
#include <QFrame>
#include <QGraphicsDropShadowEffect>


PromotionDialog::PromotionDialog(int joueur, QWidget *parent)
    : QDialog(parent), _piece(5)
{
    setWindowTitle("Promotion du pion");
    setModal(true);
    setFixedSize(380, 160);


    setWindowFlags(windowFlags() & ~Qt::WindowCloseButtonHint);

    setStyleSheet(R"(
        QDialog { background: #1e1a16; }
        QLabel  { color: #f0d9b5; font-size: 14px; }
        QPushButton {
            font-size: 36px;
            background: #3a2e24;
            color: white;
            border: 2px solid #b58863;
            border-radius: 10px;
            min-width: 70px; min-height: 70px;
        }
        QPushButton:hover { background: #b58863; border-color: #f0d9b5; }
    )");

    QVBoxLayout *main = new QVBoxLayout(this);
    QLabel *lbl = new QLabel("Choisissez la pièce de promotion :");
    lbl->setAlignment(Qt::AlignCenter);
    main->addWidget(lbl);

    QHBoxLayout *row = new QHBoxLayout();

    struct PieceInfo { int code; QString sym; };
    QVector<PieceInfo> pieces = {
        {5, joueur>0 ? "\u2655" : "\u265B"},
        {2, joueur>0 ? "\u2656" : "\u265C"},
        {3, joueur>0 ? "\u2658" : "\u265E"},
        {4, joueur>0 ? "\u2657" : "\u265D"}
    };

    for (const PieceInfo &pc : pieces) {
        QPushButton *btn = new QPushButton(pc.sym);
        int code = pc.code;
        connect(btn, &QPushButton::clicked, [this, code](){
            _piece = code;
            accept();
        });
        row->addWidget(btn);
    }
    main->addLayout(row);
}


StartDialog::StartDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle("Nouvelle Partie");
    setModal(true);
    setFixedWidth(400);

    setStyleSheet(R"(
        QDialog {
            background: qlineargradient(x1:0,y1:0,x2:0,y2:1,
                        stop:0 #1e1a16, stop:1 #141210);
        }
        QLabel#titleLabel {
            color: #f0d9b5;
            font-size: 20px;
            font-weight: bold;
            letter-spacing: 4px;
        }
        QLabel#subtitleLabel {
            color: #b58863;
            font-size: 11px;
            letter-spacing: 2px;
        }
        QGroupBox {
            color: #b58863;
            font-size: 12px;
            font-weight: bold;
            border: 1px solid #3a2e24;
            border-radius: 8px;
            margin-top: 10px;
            padding: 10px 8px 8px 8px;
            background: rgba(42,30,18,0.4);
        }
        QGroupBox::title {
            subcontrol-origin: margin;
            left: 12px;
            padding: 0 6px;
            color: #d4956a;
        }
        QComboBox {
            background: #2c2116;
            color: #f0d9b5;
            border: 1px solid #4a3728;
            border-radius: 6px;
            padding: 6px 10px;
            font-size: 12px;
            min-height: 28px;
        }
        QComboBox:hover { border-color: #b58863; }
        QComboBox::drop-down { border: none; width: 24px; }
        QComboBox QAbstractItemView {
            background: #2c2116;
            color: #f0d9b5;
            selection-background-color: #4a3728;
            border: 1px solid #b58863;
        }
        QPushButton#btnStart {
            background: qlineargradient(x1:0,y1:0,x2:0,y2:1,
                        stop:0 #c8956a, stop:1 #8b6340);
            color: #1e1a16;
            border: none;
            border-radius: 10px;
            padding: 12px;
            font-size: 15px;
            font-weight: bold;
            letter-spacing: 2px;
        }
        QPushButton#btnStart:hover {
            background: qlineargradient(x1:0,y1:0,x2:0,y2:1,
                        stop:0 #e0a870, stop:1 #a07048);
        }
        QPushButton#btnStart:pressed { background: #5c3d1e; color: white; }
    )");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    mainLayout->setSizeConstraint(QLayout::SetMinimumSize);
    mainLayout->setSpacing(8);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    QHBoxLayout *headerRow = new QHBoxLayout();
    QLabel *icon = new QLabel("\u265E");
    QFont iconF; iconF.setPointSize(32);
    icon->setFont(iconF);
    icon->setStyleSheet("color:#f0d9b5;");
    headerRow->addWidget(icon);

    QVBoxLayout *titleCol = new QVBoxLayout();
    QLabel *title = new QLabel("JEU D'\u00C9CHECS");
    title->setObjectName("titleLabel");
    QLabel *subtitle = new QLabel("NOUVELLE PARTIE");
    subtitle->setObjectName("subtitleLabel");
    titleCol->addWidget(title);
    titleCol->addWidget(subtitle);
    headerRow->addLayout(titleCol);
    headerRow->addStretch();
    mainLayout->addLayout(headerRow);

    QFrame *sep = new QFrame();
    sep->setFrameShape(QFrame::HLine);
    sep->setStyleSheet("color:#3a2e24;");
    mainLayout->addWidget(sep);
    mainLayout->addSpacing(4);

    QGroupBox *gbMode = new QGroupBox("Mode de jeu");
    QVBoxLayout *gL = new QVBoxLayout(gbMode);
    gL->setContentsMargins(8,4,8,4);
    cbMode = new QComboBox();
    cbMode->addItem("\u2694  Humain vs IA",         2);
    cbMode->addItem("\u265F\u2659  Humain vs Humain", 1);
    gL->addWidget(cbMode);
    mainLayout->addWidget(gbMode);

    gbDiff = new QGroupBox("Difficulté (IA)");
    QVBoxLayout *dL = new QVBoxLayout(gbDiff);
    dL->setContentsMargins(8,4,8,4);
    cbDiff = new QComboBox();
    cbDiff->addItem("Facile",        1);
    cbDiff->addItem("Moyen",         2);
    cbDiff->addItem("Difficile",     3);
    cbDiff->addItem("Extrême",       4);
    cbDiff->setCurrentIndex(1);
    dL->addWidget(cbDiff);
    mainLayout->addWidget(gbDiff);

    gbCouleur = new QGroupBox("Votre couleur");
    QVBoxLayout *cL = new QVBoxLayout(gbCouleur);
    cL->setContentsMargins(8,4,8,4);
    cbCouleur = new QComboBox();
    cbCouleur->addItem("Blancs \u2659", 1);
    cbCouleur->addItem("Noirs \u265F",  2);
    cL->addWidget(cbCouleur);
    mainLayout->addWidget(gbCouleur);

    QGroupBox *gbTimer = new QGroupBox("Minuterie par joueur");
    QVBoxLayout *tL = new QVBoxLayout(gbTimer);
    tL->setContentsMargins(8,4,8,4);
    cbTimer = new QComboBox();
    cbTimer->addItem("\u23F1  Sans minuterie",  0);
    cbTimer->addItem("\u23F1  1 minute",        1);
    cbTimer->addItem("\u23F1  3 minutes",       3);
    cbTimer->addItem("\u23F1  5 minutes",       5);
    cbTimer->addItem("\u23F1  10 minutes",     10);
    cbTimer->addItem("\u23F1  15 minutes",     15);
    cbTimer->addItem("\u23F1  30 minutes",     30);
    tL->addWidget(cbTimer);
    mainLayout->addWidget(gbTimer);

    auto updateVisibility = [this]() {
        int m = cbMode->currentData().toInt();
        bool hvIA = (m == 2);
        gbDiff->setVisible(hvIA);
        gbCouleur->setVisible(hvIA);
        adjustSize();
    };
    connect(cbMode, QOverload<int>::of(&QComboBox::currentIndexChanged),
            [updateVisibility](int){ updateVisibility(); });
    updateVisibility();

    mainLayout->addStretch();

    btnStart = new QPushButton("\u25B6  DÉMARRER");
    btnStart->setObjectName("btnStart");
    btnStart->setMinimumHeight(46);
    connect(btnStart, &QPushButton::clicked, this, &QDialog::accept);
    mainLayout->addWidget(btnStart);
}

int StartDialog::getMode()       const { return cbMode->currentData().toInt(); }
int StartDialog::getDifficulte() const { return cbDiff->currentData().toInt(); }
int StartDialog::getCouleur()    const { return cbCouleur->currentData().toInt(); }
int StartDialog::getTimerMode()  const { return cbTimer->currentData().toInt(); }

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , timerWhiteSecs(0), timerBlackSecs(0), timerInitSecs(0), timerEnabled(false)
{
    setWindowTitle("Jeu d'\u00C9checs");
    setMinimumSize(780, 640);

    gameTimer = new QTimer(this);
    gameTimer->setInterval(1000);
    connect(gameTimer, &QTimer::timeout, this, &MainWindow::tickTimer);

    setupUI();
    applyStyle();


    QScreen *screen = QApplication::primaryScreen();
    if (screen) {
        QRect sg = screen->availableGeometry();
        resize(sg.width() * 3/4, sg.height() * 3/4);
        move(sg.center() - rect().center());
    }


    QTimer::singleShot(200, this, &MainWindow::startNewGame);
}

void MainWindow::setupUI() {
    QWidget *central = new QWidget(this);
    QHBoxLayout *hMain = new QHBoxLayout(central);
    hMain->setSpacing(0);
    hMain->setContentsMargins(0,0,0,0);

    board = new ChessBoard(this);
    hMain->addWidget(board, 1);

    connect(board, &ChessBoard::statusChanged,     this, &MainWindow::onStatusChanged);
    connect(board, &ChessBoard::gameOver,          this, &MainWindow::onGameOver);
    connect(board, &ChessBoard::promotionRequired, this, &MainWindow::onPromotionRequired);

    QWidget *side = new QWidget(this);
    side->setFixedWidth(210);
    side->setObjectName("sidePanel");

    QVBoxLayout *sL = new QVBoxLayout(side);
    sL->setSpacing(10);
    sL->setContentsMargins(14, 20, 14, 20);

    QLabel *logo = new QLabel("\u265E");
    logo->setAlignment(Qt::AlignCenter);
    QFont lf; lf.setPointSize(44);
    logo->setFont(lf);
    logo->setObjectName("logoLabel");
    sL->addWidget(logo);

    QLabel *title = new QLabel("JEU D'\u00C9CHECS");
    title->setAlignment(Qt::AlignCenter);
    title->setObjectName("titleLabel");
    sL->addWidget(title);

    sL->addSpacing(6);

    QGroupBox *gbStatus = new QGroupBox("\u00C9tat du jeu");
    QVBoxLayout *stL = new QVBoxLayout(gbStatus);
    stL->setSpacing(6);

    lblTour = new QLabel("Tour 1");
    lblTour->setAlignment(Qt::AlignCenter);
    lblTour->setObjectName("tourLabel");
    stL->addWidget(lblTour);

    lblStatus = new QLabel("En attente\u2026");
    lblStatus->setAlignment(Qt::AlignCenter);
    lblStatus->setWordWrap(true);
    lblStatus->setObjectName("statusLabel");
    stL->addWidget(lblStatus);

    sL->addWidget(gbStatus);

    gbTimer = new QGroupBox("\u23F1  Minuterie");
    QVBoxLayout *tmL = new QVBoxLayout(gbTimer);
    tmL->setSpacing(6);

    QHBoxLayout *rowW = new QHBoxLayout();
    QLabel *iconW = new QLabel("\u2659");
    iconW->setStyleSheet("color:#f0d9b5; font-size:18px;");
    rowW->addWidget(iconW);
    lblTimerWhite = new QLabel("--:--");
    lblTimerWhite->setObjectName("timerWhite");
    lblTimerWhite->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    rowW->addWidget(lblTimerWhite);
    tmL->addLayout(rowW);

    QHBoxLayout *rowB = new QHBoxLayout();
    QLabel *iconB = new QLabel("\u265F");
    iconB->setStyleSheet("color:#888; font-size:18px;");
    rowB->addWidget(iconB);
    lblTimerBlack = new QLabel("--:--");
    lblTimerBlack->setObjectName("timerBlack");
    lblTimerBlack->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    rowB->addWidget(lblTimerBlack);
    tmL->addLayout(rowB);

    gbTimer->setVisible(false);
    sL->addWidget(gbTimer);

    sL->addStretch();

    // BUG FIX: Bouton plein écran / fenêtré
    btnFullscreen = new QPushButton("\u26F6  Plein écran");
    btnFullscreen->setObjectName("btnSecondary");
    connect(btnFullscreen, &QPushButton::clicked, this, &MainWindow::toggleFullscreen);
    sL->addWidget(btnFullscreen);

    btnNew = new QPushButton("\U0001F195  Nouvelle Partie");
    btnNew->setObjectName("btnPrimary");
    connect(btnNew, &QPushButton::clicked, this, &MainWindow::startNewGame);
    sL->addWidget(btnNew);

    btnAbout = new QPushButton("\u2139  À propos");
    btnAbout->setObjectName("btnSecondary");
    connect(btnAbout, &QPushButton::clicked, this, &MainWindow::showAbout);
    sL->addWidget(btnAbout);

    hMain->addWidget(side);
    setCentralWidget(central);

    statusBar()->showMessage("Bienvenue ! Démarrez une partie.");
}

void MainWindow::applyStyle() {
    setStyleSheet(R"(
        QMainWindow { background: #1e1a16; }

        #sidePanel {
            background: qlineargradient(x1:0,y1:0,x2:1,y2:1,
                         stop:0 #252015, stop:1 #161210);
            border-left: 2px solid #3a2e24;
        }

        #logoLabel  { color: #f0d9b5; }
        #titleLabel {
            color: #b58863;
            font-size: 12px;
            font-weight: bold;
            letter-spacing: 4px;
        }

        QGroupBox {
            color: #b58863;
            font-size: 11px;
            font-weight: bold;
            border: 1px solid #3a2e24;
            border-radius: 8px;
            margin-top: 8px;
            padding: 8px;
            background: rgba(30,20,12,0.5);
        }
        QGroupBox::title {
            subcontrol-origin: margin;
            left: 8px; padding: 0 4px;
        }

        #tourLabel   { color: #666; font-size: 10px; }
        #statusLabel { color: #f0d9b5; font-size: 12px; font-weight: bold; padding: 4px; }

        #timerWhite {
            color: #f0d9b5;
            font-size: 20px;
            font-weight: bold;
            font-family: monospace;
        }
        #timerBlack {
            color: #aaa;
            font-size: 20px;
            font-weight: bold;
            font-family: monospace;
        }

        #btnPrimary {
            background: qlineargradient(x1:0,y1:0,x2:0,y2:1,
                         stop:0 #c49a6c, stop:1 #7d5a3c);
            color: #1a1208;
            border: none;
            border-radius: 8px;
            padding: 10px;
            font-size: 13px;
            font-weight: bold;
        }
        #btnPrimary:hover   { background: #d4a574; color:#1a1208; }
        #btnPrimary:pressed { background: #5c3d1e; color:white; }

        #btnSecondary {
            background: transparent;
            color: #6a5040;
            border: 1px solid #3a2e24;
            border-radius: 8px;
            padding: 8px;
            font-size: 12px;
        }
        #btnSecondary:hover { background: #2c2116; color:#b58863; border-color:#b58863; }

        QStatusBar { background: #0e0c0a; color: #555; font-size: 11px; }
    )");
}


void MainWindow::toggleFullscreen() {
    if (isMaximized() || isFullScreen()) {
        showNormal();
        btnFullscreen->setText("\u26F6  Plein écran");
    } else {
        showMaximized();
        btnFullscreen->setText("\u29C9  Fenêtré");
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_F11) {
        toggleFullscreen();
    } else {
        QMainWindow::keyPressEvent(event);
    }
}

// ─── Timer ────────────────────────────────────────────────────────────────────

void MainWindow::updateTimerDisplay() {
    if (!timerEnabled) return;
    auto fmt = [](int s) -> QString {
        int m = s / 60, sec = s % 60;
        return QString("%1:%2").arg(m,2,10,QChar('0')).arg(sec,2,10,QChar('0'));
    };
    lblTimerWhite->setText(fmt(timerWhiteSecs));
    lblTimerBlack->setText(fmt(timerBlackSecs));

    lblTimerWhite->setStyleSheet(timerWhiteSecs < 30
                                     ? "color:#ff4444; font-size:20px; font-weight:bold; font-family:monospace;"
                                     : "color:#f0d9b5; font-size:20px; font-weight:bold; font-family:monospace;");
    lblTimerBlack->setStyleSheet(timerBlackSecs < 30
                                     ? "color:#ff4444; font-size:20px; font-weight:bold; font-family:monospace;"
                                     : "color:#aaa; font-size:20px; font-weight:bold; font-family:monospace;");
}

void MainWindow::tickTimer() {
    if (!timerEnabled || board->isGameOver()) return;

    int joueur = board->getJoueurActuel();
    if (joueur == 1) {
        timerWhiteSecs--;
        if (timerWhiteSecs <= 0) {
            timerWhiteSecs = 0;
            gameTimer->stop();
            updateTimerDisplay();
            onGameOver("Temps écoulé ! Les Noirs gagnent !");
            return;
        }
    } else {
        timerBlackSecs--;
        if (timerBlackSecs <= 0) {
            timerBlackSecs = 0;
            gameTimer->stop();
            updateTimerDisplay();
            onGameOver("Temps écoulé ! Les Blancs gagnent !");
            return;
        }
    }
    updateTimerDisplay();
}

// ─── Slots ────────────────────────────────────────────────────────────────────

void MainWindow::startNewGame() {
    bool wasVisible = this->isVisible();
    if (wasVisible) this->hide();

    StartDialog dlg(this);
    if (dlg.exec() != QDialog::Accepted) {
        if (!wasVisible) {
            qApp->quit();
        } else {
            this->show();
        }
        return;
    }

    gameTimer->stop();

    int mode = dlg.getMode();
    board->setGameMode(mode);
    board->setDifficulty(dlg.getDifficulte());
    board->setPlayerColor(dlg.getCouleur());
    board->resetGame();

    int mins = dlg.getTimerMode();
    timerEnabled = (mins > 0);
    timerInitSecs = mins * 60;
    timerWhiteSecs = timerInitSecs;
    timerBlackSecs = timerInitSecs;

    if (timerEnabled) {
        gbTimer->setVisible(true);
        updateTimerDisplay();
        gameTimer->start();
    } else {
        gbTimer->setVisible(false);
        lblTimerWhite->setText("--:--");
        lblTimerBlack->setText("--:--");
    }

    QString modeStr = (mode == 1) ? "Humain vs Humain" : "Humain vs IA";
    lblTour->setText("Tour 1  \u2022  " + modeStr);
    lblStatus->setText("En cours\u2026");
    statusBar()->showMessage("Nouvelle partie démarrée ! — " + modeStr);


    if (isMaximized() || isFullScreen()) {
        this->show();
    } else {
        this->show();
        this->update();
    }
}

void MainWindow::onStatusChanged(const QString &msg) {
    lblStatus->setText(msg);

    int joueur = board->getJoueurActuel();
    QString joueurStr = (joueur == 1) ? "Blancs \u2659" : "Noirs \u265F";
    lblTour->setText("Au tour des " + joueurStr);
    statusBar()->showMessage(msg);
}

void MainWindow::onGameOver(const QString &result) {
    gameTimer->stop();
    lblStatus->setText(result);
    statusBar()->showMessage(result);

    QMessageBox *mb = new QMessageBox(this);
    mb->setWindowTitle("Fin de partie");
    mb->setText("<b style='font-size:16px;'>" + result + "</b>");
    mb->setInformativeText("Voulez-vous rejouer ?");
    mb->addButton("Rejouer", QMessageBox::YesRole);
    mb->addButton("Quitter", QMessageBox::NoRole);
    mb->setIcon(QMessageBox::NoIcon);
    mb->setStyleSheet(R"(
        QMessageBox { background:#1e1a16; }
        QLabel { color:#f0d9b5; font-size:13px; }
        QPushButton { background:#b58863; color:#1a1208; border:none;
                      border-radius:6px; padding:8px 20px; font-size:13px; font-weight:bold; }
        QPushButton:hover { background:#d4a574; }
    )");

    int ret = mb->exec();
    if (ret == 0) startNewGame();
    else qApp->quit();
}

void MainWindow::onPromotionRequired(int /*la*/, int /*ca*/) {
    // BUG FIX #7: utiliser getPromotingPlayer() pour obtenir la couleur correcte
    // sans dépendre du sens du tour (qui a déjà été incrémenté)
    int joueur = board->getPromotingPlayer();
    PromotionDialog dlg(joueur, this);
    dlg.exec();
    board->applyPromotion(dlg.pieceChoisie());
}

void MainWindow::showAbout() {
    QMessageBox::about(this, "À propos",
                       "<b>Jeu d'Échecs Qt</b><br>"
                       "Version 1.2<br><br>"
                       "Modes de jeu :<br>"
                       "\u2022 Humain vs IA (4 niveaux)<br>"
                       "\u2022 Humain vs Humain<br>"
                       "\u2022 Minuterie par joueur (1 à 30 min)<br><br>"
                       "Règles implémentées :<br>"
                       "\u2022 Roque (petit et grand)<br>"
                       "\u2022 Prise en passant<br>"
                       "\u2022 Promotion au choix<br>"
                       "\u2022 Détection pat / échec et mat<br>"
                       "\u2022 Nulle par répétition (3x)<br>"
                       "\u2022 Règle des 50 coups<br><br>"
                       "Raccourci : F11 = Plein écran");
}
