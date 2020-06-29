// SPDX-License-Identifier: BSD-3-Clause
#include "mainwindow.h"
#include "graphicsscene.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scene = new GraphicsScene(this);
    ui->graphicsView->setScene(scene);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionSave_triggered()
{
   scene->save();
}

void MainWindow::on_actionLoad_triggered()
{
   scene->load();
}

void MainWindow::on_actionPrint_triggered()
{
    scene->print();
}
