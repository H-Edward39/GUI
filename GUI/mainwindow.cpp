#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <sstream>
#include <iomanip>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , registerModel(new QStringListModel(this))
    , memoryModel(new QStringListModel(this))
    , programHalted(false)  // Initialize programHalted
{
    ui->setupUi(this);

    // Set initial models
    ui->listView->setModel(registerModel);
    ui->listView_2->setModel(memoryModel);

    // Connect buttons to slots
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::on_addInstructionButton_clicked);  // Add Instruction button
    connect(ui->pushButton_3, &QPushButton::clicked, this, &MainWindow::on_showRegisters_clicked);      // Show Registers button
    connect(ui->pushButton_5, &QPushButton::clicked, this, &MainWindow::on_showMemory_clicked);         // Show Memory button
    connect(ui->pushButton_2, &QPushButton::clicked, this, &MainWindow::on_halt_clicked);               // Halt button
    connect(ui->pushButton_4, &QPushButton::clicked, this, &MainWindow::on_clear_clicked);              // Clear button

    // Initialize the displays
    updateRegistersView();
    updateMemoryView();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_addInstructionButton_clicked()
{
    QString instruction = ui->textEdit->toPlainText().trimmed();
    if (instruction.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter a valid instruction.");
        return;
    }

    try {
        machine.loadProgramFromText(instruction.toStdString());
        ui->textEdit->clear();
        QMessageBox::information(this, "Success", "Instruction loaded successfully.");
        programHalted = false;  // Reset halted state if new instructions are added
    } catch (const exception& e) {
        QMessageBox::critical(this, "Error", QString("Failed to load instruction: %1").arg(e.what()));
    }
}

void MainWindow::on_showRegisters_clicked()
{
    if (programHalted) {
        updateRegistersView();
    } else {
        QMessageBox::information(this, "Program Not Halted", "Please halt the program before viewing registers.");
    }
}

void MainWindow::on_showMemory_clicked()
{
    if (programHalted) {
        updateMemoryView();
    } else {
        QMessageBox::information(this, "Program Not Halted", "Please halt the program before viewing memory.");
    }
}

void MainWindow::on_halt_clicked()
{
    try {
        machine.executeProgram();
        programHalted = true;  // Set program as halted after execution
        QMessageBox::information(this, "Success", "Program executed successfully.");
    } catch (const exception& e) {
        QMessageBox::critical(this, "Error", QString("Execution failed: %1").arg(e.what()));
    }
}

void MainWindow::on_clear_clicked()
{
    resetMemoryAndRegisters();

    // Clear the list views on screen
    registerModel->setStringList(QStringList());
    memoryModel->setStringList(QStringList());

    // Clear the text edit
    ui->textEdit->clear();
    programHalted = false;  // Reset halted state after clear
}

void MainWindow::updateRegistersView()
{
    QStringList registerList;
    const Register& registers = machine.getRegisterBank();

    for (int i = 0; i < 16; i++) {
        QString regValue = QString("R%1: 0x%2")
        .arg(i, 1, 16)
            .arg(registers.getCell(i), 4, 16, QChar('0')).toUpper();
        registerList << regValue;
    }

    registerModel->setStringList(registerList);
}

void MainWindow::updateMemoryView()
{
    QStringList memoryList;
    memoryList << "Memory Contents:";
    memoryList << "Addr\t+0\t+1\t+2\t+3\t+4\t+5\t+6\t+7";

    for (int i = 0; i < 256; i += 8) {
        QString line = QString("%1\t").arg(i, 3, 16, QChar('0')).toUpper();
        for (int j = 0; j < 8 && (i + j) < 256; ++j) {
            line += machine.getMemoryCell(i + j) + "\t";
        }
        memoryList << line;
    }

    memoryModel->setStringList(memoryList);
}

void MainWindow::resetMemoryAndRegisters()
{
    // Reset memory and registers in the machine object
    machine.resetMemory();  // Assuming resetMemory sets all cells to 0000
    machine.resetRegisters();  // Assuming resetRegisters sets all registers to 0x0000
}
