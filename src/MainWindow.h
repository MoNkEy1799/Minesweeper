#pragma once
#include <QMainWindow>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <iostream>
#include <QGridLayout>

class MainWindow : public QMainWindow
{
public: 
	MainWindow(QWidget* partent = nullptr);
	~MainWindow();

private:
	QWidget* m_mainWidget;
	QGridLayout* m_layout;
};
