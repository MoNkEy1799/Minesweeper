#include <QWidget>
#include <QPushButton>
#include <QIcon>
#include <QPixmap>
#include <QGridLayout>
#include <QTimer>
#include <QLabel>
#include <Qt>

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>

#include "MainWindow.h"
#include "HeaderWidget.h"
#include "Board.h"

Header::Header(int size, QWidget* parent, MainWindow* main)
	: QWidget(parent), m_mainWindow(main), m_counter(this), m_timer(this), m_qtimer(nullptr), m_passedTime(-1), m_highscores()
{
	setFixedSize(size, 46);
	setStyleSheet(StyleSheet::HEADER.c_str());

	m_qtimer = new QTimer(this);
	connect(m_qtimer, &QTimer::timeout, this, [this] { updateTime(); });

	m_restart = new QPushButton(this);
	m_restart->setIcon(QIcon("resources/tiles/smiley.png"));
	m_restart->setIconSize(QSize(30, 30));
	m_restart->setStyleSheet(StyleSheet::COVERED.c_str());
	m_restart->setFixedSize(30, 30);
	connect(m_restart, &QPushButton::clicked, main, &MainWindow::restartGame);

	QGridLayout* layout = new QGridLayout(this);
	layout->addWidget(&m_counter, 0, 0, Qt::AlignLeft);
	layout->addWidget(m_restart, 0, 1, Qt::AlignCenter);
	layout->addWidget(&m_timer, 0, 2, Qt::AlignRight);
	setLayout(layout);
}

void Header::startTimer()
{
	m_qtimer->start(1000);
}

void Header::resetTimer()
{
	m_qtimer->stop();
	m_passedTime = -1;
	updateTime();
}

void Header::stopTimer()
{
	m_qtimer->stop();
}

void Header::changeMineCount(int count)
{
	int sign = 1;
	if (count >= 0)
	{
		m_counter.set(2, count / 100);
	}
	else
	{
		sign = -1;
		m_counter.set(2, 10);
	}
	m_counter.set(0, sign * count % 10);
	m_counter.set(1, sign * count / 10 % 10);
}

void Header::changeSmiley(bool win, bool reset)
{
	if (reset)
	{
		m_restart->setIcon(QIcon("resources/tiles/smiley.png"));
		return;
	}
	if (win)
	{
		m_restart->setIcon(QIcon("resources/tiles/win.png"));
	}
	else
	{
		m_restart->setIcon(QIcon("resources/tiles/lose.png"));
	}
}

void Header::updateTime()
{
	m_passedTime++;
	m_timer.set(0, m_passedTime % 10);
	m_timer.set(1, m_passedTime / 10 % 10);
	m_timer.set(2, m_passedTime / 100);
}

Display::Display(QWidget* parent)
	: QWidget(parent)
{
	setObjectName("Display");
	QGridLayout* layout = new QGridLayout(this);
	layout->setSpacing(0);
	layout->setContentsMargins(0, 0, 0, 0);
	setLayout(layout);
	setFixedSize(45, 24);

	for (int digit = 0; digit < 3; digit++)
	{
		for (int number = 0; number < 11; number++)
		{
			QLabel* label = new QLabel(this);
			m_display[digit][number] = label;
			layout->addWidget(label, 0, 2 - digit, Qt::AlignCenter);
			if (number < 10)
			{
				label->setPixmap(QPixmap(("resources/timer/t_" + std::to_string(number) + ".png").c_str()));
			}
			else
			{
				label->setPixmap(QPixmap("resources/timer/minus.png"));
			}
			label->setFixedSize(15, 24);
			label->setAlignment(Qt::AlignCenter);
			label->setStyleSheet(StyleSheet::HEADER.c_str());
		}
	}
	reset();
}

void Display::reset()
{
	hideAll(-1);
	for (int digit = 0; digit < 3; digit++)
	{
		m_display[digit][0]->show();
	}
}

void Display::set(int digit, int number)
{
	hideAll(digit);
	m_display[digit][number]->show();
}

void Display::hideAll(int digit)
{
	if (digit == -1)
	{
		hideAll(0);
		hideAll(1);
		hideAll(2);
		return;
	}

	for (int number = 0; number < 11; number++)
	{
		m_display[digit][number]->hide();
	}
}

Highscores::Highscores()
{
	loadData();
	addScore(Difficulty::BEGINNER, 1);
	addScore(Difficulty::INTERMEDIATE, 200);
	addScore(Difficulty::EXPERT, 4532);
}

void Highscores::addScore(Difficulty diff, uint16_t seconds)
{
	for (int i = 0; i < 10; i++)
	{
		if (seconds < m_scoreData[diff][i].time)
		{
			shiftData(diff, i);
			Score s = newScore();
			s.time = seconds;
			m_scoreData[diff][i] = s;
			break;
		}
	}
	saveData();
}

void Highscores::shiftData(Difficulty diff, int index)
{
	for (int i = index; i < 9; i++)
	{
		m_scoreData[diff][i] = m_scoreData[diff][i + 1];
	}
}

void Highscores::loadData()
{
	std::array<std::stringstream, 4> filedata;
	std::ifstream file;
	file.open("saves/highscores.txt");

	Difficulty diff = Difficulty::BEGINNER;
	std::string line;
	while (std::getline(file, line))
	{
		if (line.find("#BEGINNER") != std::string::npos)
		{
			diff = Difficulty::BEGINNER;
			continue;
		}
		else if (line.find("#INTERMEDIATE") != std::string::npos)
		{
			diff = Difficulty::INTERMEDIATE;
			continue;
		}
		else if (line.find("#EXPERT") != std::string::npos)
		{
			diff = Difficulty::EXPERT;
			continue;
		}
		else if (line.find("#STATS") != std::string::npos)
		{
			diff = Difficulty::STATS;
			continue;
		}
		filedata[diff] << line;
	}

	for (int diff = 0; diff < 3; diff++)
	{
		for (int place = 0; place < 10; place++)
		{
			Score s;
			std::string temp;
			filedata[diff] >> temp;

			if (temp != "-")
			{
				s.year = std::stoi(temp.substr(0, 4));
				s.month = std::stoi(temp.substr(4, 2));
				s.day = std::stoi(temp.substr(6, 2));
				s.hour = std::stoi(temp.substr(10, 2));
				s.minute = std::stoi(temp.substr(12, 2));
				s.second = std::stoi(temp.substr(14, 2));
				s.time = (uint16_t)std::stoi(temp.substr(18, 4));
			}
			m_scoreData[(Difficulty)diff][place] = s;
		}
	}
	file.close();
}

void Highscores::saveData()
{
	std::ofstream file;
	file.open("saves/highscores.txt");
	for (int diff = 0; diff < 3; diff++)
	{
		if (diff == 0)
		{
			file << "#BEGINNER\n";
		}
		else if (diff == 1)
		{
			file << "#INTERMEDIATE\n";
		}
		else if (diff == 2)
		{
			file << "#EXPERT\n";
		}
		for (int place = 0; place < 10; place++)
		{
			Score score = m_scoreData[(Difficulty)diff][place];
			if (score.year == 0)
			{
				file << "- \n";
				continue;
			}
			std::ostringstream osy;
			osy << std::setw(4) << std::setfill('0') << std::to_string(score.year);
			file << osy.str();

			std::ostringstream osm;
			osm << std::setw(2) << std::setfill('0') << std::to_string(score.month);
			file << osm.str();

			std::ostringstream osd;
			osd << std::setw(2) << std::setfill('0') << std::to_string(score.day);
			file << osd.str();
			file << "--";

			std::ostringstream osh;
			osh << std::setw(2) << std::setfill('0') << std::to_string(score.hour);
			file << osh.str();

			std::ostringstream osn;
			osn << std::setw(2) << std::setfill('0') << std::to_string(score.minute);
			file << osn.str();
			
			std::ostringstream oss;
			oss << std::setw(2) << std::setfill('0') << std::to_string(score.second);
			file << oss.str();
			file << "--";
			
			std::ostringstream ost;
			ost << std::setw(4) << std::setfill('0') << std::to_string(score.time);
			file << ost.str();
			file << " \n";
		}
		file << "\n";
	}
	file.close();
}

Score Highscores::newScore()
{
	std::stringstream now;
	now << std::chrono::system_clock::now();
	std::string ymd, hms;
	now >> ymd;
	now >> hms;
	Score retScore = Score();
	retScore.year = std::stoi(ymd.substr(0, 4));
	retScore.month = std::stoi(ymd.substr(5, 2));
	retScore.day = std::stoi(ymd.substr(8, 2));
	retScore.hour = std::stoi(hms.substr(0, 2));
	retScore.minute = std::stoi(hms.substr(3, 2));
	retScore.second = (int)std::round(std::stof(hms.substr(6, 4)));
	return retScore;
}
