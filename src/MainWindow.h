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

	void restartGame();
	void makeMenu();
	void makeSettings();
	void makeHighscores();

	Board* board;
	Header* header;

private:
	QWidget* m_mainWidget;
	QWidget* m_customSettings;
	QWidget* m_highscores;
	QGridLayout* m_layout;
	int m_rowSetting, m_colSetting, m_mineSetting;
};
