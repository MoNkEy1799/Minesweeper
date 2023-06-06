#include <QWidget>
#include <QPushButton>
#include <QIcon>
#include <QPixmap>
#include <QGridLayout>

#include <string>

#include "MainWindow.h"
#include "HeaderWidget.h"
#include "Board.h"

Header::Header(int size, QWidget* parent, MainWindow* main)
	: QWidget(parent), m_mainWindow(main)
{
	setFixedSize(size, 50);

	for (int i = 0; i < 10; i++)
	{
		m_numbers[i] = new QLabel(this);
		m_numbers[i]->setPixmap(QPixmap(("resources/timer/t_" + std::to_string(i) + ".png").c_str()));
	}

	m_restart = new QPushButton(this);
	m_restart->setIcon(QIcon("resources/tiles/smiley.png"));
	m_restart->setIconSize(QSize(25, 25));
	m_restart->setStyleSheet(StyleSheet::COVERED.c_str());
	m_restart->setFixedSize(25, 25);
	connect(m_restart, &QPushButton::clicked, main, &MainWindow::restartGame);

	QGridLayout* layout = new QGridLayout();
	layout->addWidget(m_restart);
	setLayout(layout);
}

Header::~Header()
{
	for (int i = 0; i < 10; i++)
	{
		delete m_numbers[i];
	}
}
