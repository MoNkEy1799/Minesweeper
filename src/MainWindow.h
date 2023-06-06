#pragma once
#include <QMainWindow>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <iostream>
#include <QGridLayout>

class Board;
class Header;

class MainWindow : public QMainWindow
{
public: 
	MainWindow(QWidget* partent = nullptr);
	~MainWindow();

	void restartGame();

private:
	QWidget* m_mainWidget;
	QGridLayout* m_layout;
	Header* m_header;
	Board* m_board;

};
