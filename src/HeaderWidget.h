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

enum Difficulty { BEGINNER, INTERMEDIATE, EXPERT, STATS };

struct Score
{
	uint16_t time = ~0;
	int year = 0, month = 0, day = 0, hour = 0, minute = 0, second = 0;
};

class Highscores
{
public:
	Highscores(MainWindow* main = nullptr);
	void addScore(Difficulty diff, uint16_t seconds);
	std::string formattedScore(Difficulty diff, int placing, bool save);

private:
	std::unordered_map<Difficulty, std::array<Score, 10>> m_scoreData;
	MainWindow* m_mainWindow;
	
	void shiftData(Difficulty diff, int index);
	void loadData();
	void saveData();
	Score newScore();
};

class Header : public QWidget
{
public:
	Header(int size, QWidget* parent = nullptr, MainWindow* main = nullptr);

	void startTimer();
	void resetTimer();
	int stopTimer();
	void changeMineCount(int count);
	void changeSmiley(bool win, bool reset = false);

	Highscores highscores;

private:
	MainWindow* m_mainWindow;
	Display m_timer, m_counter;
	QPushButton* m_restart;
	QTimer* m_qtimer;
	int m_passedTime;

	void updateTime();
};