#ifndef PREPARATION_INGAME_H
#define PREPARATION_INGAME_H

#include <QDialog>
#include"User.h"
#include"League.h"
#include "GameEngine/gameengine.h"

namespace Ui {
class Preparation_inGame;
}

class Preparation_inGame : public QDialog
{
    Q_OBJECT

public:
    explicit Preparation_inGame(QWidget *parent = nullptr);
    explicit Preparation_inGame(GameEngine* eng, User& theuser,League& league, QWidget *parent = nullptr);
    User* myuser;
    League* myleague;
    GameEngine* engine;
    ~Preparation_inGame();

//signals:
    //void ContinueClicked(const GameEngine*, const User&, const League&);

public slots:
    void refresh(const User&);

private slots:
    void on_pushButton_14_clicked();

    void on_pushButton_13_clicked();

    void on_p1_clicked();

    void on_p2_clicked();

    void on_p3_clicked();

    void on_p4_clicked();

    void on_p5_clicked();

    void on_p6_clicked();

    void on_p7_clicked();

    void on_p8_clicked();

    void on_p9_clicked();

    void on_p10_clicked();

    void on_p11_clicked();

    void on_p12_clicked();

private:
    Ui::Preparation_inGame *ui;
    int chosen_1;
    int chosen_2;
    void Substitution();
};

#endif // PREPARATION_INGAME_H
