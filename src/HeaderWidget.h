#pragma once

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QTimer>

#include <vector>

class MainWindow;

class Display : public QWidget
{
public:
	Display(QWidget* partent = nullptr);
	
	void reset();
	void set(int digit, int number);

private:
	std::array<std::array<QLabel*, 11>, 3> m_display;

	void hideAll(int digit);
};

class Header : public QWidget
{
public:
	Header(int size, QWidget* parent = nullptr, MainWindow* main = nullptr);

	void startTimer();
	void resetTimer();
	void stopTimer();
	void changeMineCount(int count);
	void changeSmiley(bool win, bool reset = false);

private:
	MainWindow* m_mainWindow;
	Display m_timer, m_counter;
	QPushButton* m_restart;
	QTimer* m_qtimer;
	int m_passedTime;

	void updateTime();
};
