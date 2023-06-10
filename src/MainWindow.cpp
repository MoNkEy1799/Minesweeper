#include <QLabel>
#include <QSpinBox>
#include <QFont>
#include <QWidget>
#include <QGridlayout>
#include <QPushButton>
#include <QMenu>
#include <QMenuBar>

#include <string>

#include "MainWindow.h"
#include "HeaderWidget.h"
#include "Board.h"
#include "Tile.h"

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent), board(nullptr), m_mainWidget(nullptr), header(nullptr), m_layout(nullptr),
	m_customSettings(nullptr), m_highscores(nullptr), m_rowSetting(16), m_colSetting(16), m_mineSetting(40)
{
	header = new Header(m_colSetting * 20 + 4, this, this);
	header->changeMineCount(m_mineSetting);
	board = new Board(m_rowSetting, m_colSetting, m_mineSetting, 20, this, this);

	makeMenu();
	makeSettings();
	makeHighscores();

	m_layout = new QGridLayout();
	m_layout->addWidget(header, 0, 0, Qt::AlignCenter);
	m_layout->addWidget(board, 1, 0);

	m_mainWidget = new QWidget(this);
	m_mainWidget->setLayout(m_layout);
	m_mainWidget->setStyleSheet(StyleSheet::OUTER.c_str());
	m_mainWidget->setFixedSize(m_colSetting * 20 + 24, m_rowSetting * 20 + 84);
	setCentralWidget(m_mainWidget);
}

void MainWindow::restartGame()
{
	if (QSize(m_rowSetting, m_colSetting) != board->boardSize())
	{
		board->hide();
		board->deleteLater();
		board = new Board(m_rowSetting, m_colSetting, m_mineSetting, 20, this, this);
		m_layout->addWidget(board, 1, 0);

		header->hide();
		header->deleteLater();
		header = new Header(m_colSetting * 20 + 4, this, this);
		header->changeMineCount(m_mineSetting);
		m_layout->addWidget(header, 0, 0, Qt::AlignCenter);

		m_mainWidget->setFixedSize(m_colSetting * 20 + 24, m_rowSetting * 20 + 84);
		menuBar()->setFixedWidth(m_colSetting * 20 + 24);
		setFixedSize(m_mainWidget->size() + QSize(0, menuBar()->height()));
		return;
	}
	board->mineCount = m_mineSetting;
	board->flagCount = 0;
	board->reset();
	header->changeSmiley(true, true);
	header->resetTimer();
	header->changeMineCount(m_mineSetting);
	Tile::currentButton = MouseButton::LEFT;
}

void MainWindow::makeMenu()
{
	menuBar()->setFixedWidth(m_colSetting * 20 + 24);
	menuBar()->setStyleSheet(StyleSheet::MENU.c_str());
	QMenu* menu = menuBar()->addMenu("New Game");
	QMenu* highscore = menuBar()->addMenu("Highscores");

	QAction* beginner = menu->addAction("Beginner");
	QAction* inter = menu->addAction("Intermediate");
	QAction* expert = menu->addAction("Expert");
	QAction* custom = menu->addAction("Custom");
	auto startBeg = [this] { m_rowSetting = 8; m_colSetting = 8; m_mineSetting = 10; restartGame(); };
	auto startInt = [this] { m_rowSetting = 16; m_colSetting = 16; m_mineSetting = 40; restartGame(); };
	auto startExp = [this] { m_rowSetting = 16; m_colSetting = 30; m_mineSetting = 99; restartGame(); };
	connect(beginner, &QAction::triggered, this, startBeg);
	connect(inter, &QAction::triggered, this, startInt);
	connect(expert, &QAction::triggered, this, startExp);
	connect(custom, &QAction::triggered, this, [this] { m_customSettings->show(); });
}

void MainWindow::makeSettings()
{
	m_customSettings = new QWidget();
	QGridLayout* layout = new QGridLayout();
	m_customSettings->setLayout(layout);
	m_customSettings->setMinimumSize(200, 100);
	m_customSettings->setWindowTitle("Settings");
	m_customSettings->setStyleSheet(StyleSheet::OUTER.c_str());

	QLabel* widthLabel = new QLabel("Width: ");
	QSpinBox* widthEdit = new QSpinBox();
	widthEdit->setValue(m_colSetting);
	widthEdit->setMaximum(40);
	widthEdit->setMinimum(7);
	connect(widthEdit, &QSpinBox::valueChanged, this, [this, widthEdit] { m_colSetting = widthEdit->value(); });
	
	QLabel* heightLabel = new QLabel("Height: ");
	QSpinBox* heightEdit = new QSpinBox();
	heightEdit->setValue(m_rowSetting);
	heightEdit->setMaximum(40);
	heightEdit->setMinimum(2);
	connect(heightEdit, &QSpinBox::valueChanged, this, [this, heightEdit] { m_rowSetting = heightEdit->value(); });
	
	QLabel* mineLabel = new QLabel("Mines: ");
	QSpinBox* mineEdit = new QSpinBox();
	mineEdit->setValue(m_mineSetting);
	mineEdit->setMaximum(700);
	mineEdit->setMinimum(1);
	connect(mineEdit, &QSpinBox::valueChanged, this, [this, mineEdit] { m_mineSetting = mineEdit->value(); });
	
	QPushButton* restart = new QPushButton("Restart Game");
	connect(restart, &QPushButton::clicked, this, [this] { restartGame(); });

	layout->addWidget(widthLabel, 0, 0);
	layout->addWidget(widthEdit, 0, 1);
	layout->addWidget(heightLabel, 1, 0);
	layout->addWidget(heightEdit, 1, 1);
	layout->addWidget(mineLabel, 2, 0);
	layout->addWidget(mineEdit, 2, 1);
	layout->addWidget(restart, 3, 0, 1, 2);
}

void MainWindow::makeHighscores()
{
}
