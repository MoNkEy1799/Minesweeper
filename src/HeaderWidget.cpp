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

#include "MainWindow.h"
#include "HeaderWidget.h"
#include "Board.h"

Header::Header(int size, QWidget* parent, MainWindow* main)
	: QWidget(parent), m_mainWindow(main), m_counter(this), m_timer(this), m_qtimer(nullptr), m_passedTime(-1)
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
