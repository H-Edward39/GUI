#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringListModel>
#include "machine.h"

using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_addInstructionButton_clicked();
    void on_showRegisters_clicked();
    void on_showMemory_clicked();
    void on_halt_clicked();
    void on_clear_clicked();

private:
    Ui::MainWindow *ui;
    Machine machine;
    bool programHalted;  // Tracks if program has halted
    void updateRegistersView();
    void updateMemoryView();
    void resetMemoryAndRegisters();  // New function to reset memory and registers
    QStringListModel* registerModel;
    QStringListModel* memoryModel;
};

#endif // MAINWINDOW_H
