#pragma once
#include <QMainWindow>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <iostream>

class MainWindow : public QMainWindow
{
public: 
	MainWindow(QWidget* partent = nullptr);
	~MainWindow();

private:
	QWidget* m_mainWidget;
};
