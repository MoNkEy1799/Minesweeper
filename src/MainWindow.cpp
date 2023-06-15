#include <QLabel>
#include <QSpinBox>
#include <QFont>
#include <QWidget>
#include <QGridlayout>
#include <QPushButton>
#include <QMenu>
#include <QMenuBar>
#include <QIcon>

#include <string>

#include "MainWindow.h"
#include "HeaderWidget.h"
#include "Board.h"
#include "Tile.h"

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent), board(nullptr), m_mainWidget(nullptr), header(nullptr), m_layout(nullptr),
	m_customSettings(nullptr), m_highscoreWidget(nullptr), m_statsWidget(nullptr),
	m_rowSetting(8), m_colSetting(8), m_mineSetting(10), difficulty(Difficulty::BEGINNER)
{
	header = new Header(m_colSetting * 20 + 4, this, this);
	header->changeMineCount(m_mineSetting);
	board = new Board(m_rowSetting, m_colSetting, m_mineSetting, 20, this, this);

	makeMenu();
	makeSettings();
	makeHighscores();
	makeStatsWidget();

	m_layout = new QGridLayout();
	m_layout->addWidget(header, 0, 0, Qt::AlignCenter);
	m_layout->addWidget(board, 1, 0);

	m_mainWidget = new QWidget(this);
	m_mainWidget->setLayout(m_layout);
	m_mainWidget->setStyleSheet(StyleSheet::OUTER.c_str());
	m_mainWidget->setFixedSize(m_colSetting * 20 + 24, m_rowSetting * 20 + 84);
	setCentralWidget(m_mainWidget);
	setFixedSize(m_mainWidget->size() + QSize(0, 22));
	setWindowIcon(QIcon("resources/tiles/mine.png"));
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
	QMenu* menu = menuBar()->addMenu("Game");

	QAction* highscore = menuBar()->addAction("Highscores");
	auto scoreTrig = [this]
	{
		if (header->highscores.scoreChanged)
		{
			makeHighscores();
		}
		m_highscoreWidget->show();
	};
	connect(highscore, &QAction::triggered, this, scoreTrig);

	QAction* stats = menuBar()->addAction("Stats");
	auto statTrig = [this]
	{
		if (header->highscores.statChanged)
		{
			makeStatsWidget();
		}
		m_statsWidget->show();
	};
	connect(stats, &QAction::triggered, this, statTrig);

	QAction* beginner = menu->addAction("Beginner");
	QAction* inter = menu->addAction("Intermediate");
	QAction* expert = menu->addAction("Expert");
	QAction* custom = menu->addAction("Custom");
	auto startBeg = [this] { difficulty = Difficulty::BEGINNER, m_rowSetting = 8; m_colSetting = 8;
							 m_mineSetting = 10; restartGame(); };
	auto startInt = [this] { difficulty = Difficulty::INTERMEDIATE, m_rowSetting = 16; m_colSetting = 16;
							 m_mineSetting = 40; restartGame(); };
	auto startExp = [this] { difficulty = Difficulty::EXPERT, m_rowSetting = 16; m_colSetting = 30;
							 m_mineSetting = 99; restartGame(); };
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
	m_customSettings->setMinimumSize(220, 100);
	m_customSettings->setWindowTitle("Settings");
	m_customSettings->setStyleSheet(StyleSheet::OUTER.c_str());
	m_customSettings->setWindowIcon(QIcon("resources/tiles/win.png"));

	QLabel* widthLabel = new QLabel("Width: ");
	widthLabel->setStyleSheet("color: #0100fe");
	widthLabel->setFixedSize(60, 20);
	QSpinBox* widthEdit = new QSpinBox();
	widthEdit->setValue(m_colSetting);
	widthEdit->setFixedSize(60, 20);
	widthEdit->setPrefix("00");
	widthEdit->setMaximum(40);
	widthEdit->setMinimum(7);
	
	QLabel* heightLabel = new QLabel("Height: ");
	heightLabel->setStyleSheet("color: #008001");
	heightLabel->setFixedSize(60, 20);
	QSpinBox* heightEdit = new QSpinBox();
	heightEdit->setValue(m_rowSetting);
	heightEdit->setFixedSize(60, 20);
	heightEdit->setPrefix("00");
	heightEdit->setMaximum(40);
	heightEdit->setMinimum(2);
	
	QLabel* mineLabel = new QLabel("Mines: ");
	mineLabel->setStyleSheet("color: #fe0000");
	mineLabel->setFixedSize(60, 20);
	QSpinBox* mineEdit = new QSpinBox();
	mineEdit->setValue(m_mineSetting);
	mineEdit->setFixedSize(60, 20);
	mineEdit->setPrefix("0");
	mineEdit->setMaximum(700);
	mineEdit->setMinimum(1);

	auto pre = [widthEdit, heightEdit, mineEdit]
	{
		if (widthEdit->value() > 9)
		{
			widthEdit->setPrefix("0");
		}
		else
		{
			widthEdit->setPrefix("00");
		}
		if (heightEdit->value() > 9)
		{
			heightEdit->setPrefix("0");
		}
		else
		{
			heightEdit->setPrefix("00");
		}
		if (mineEdit->value() > 9)
		{
			mineEdit->setPrefix("0");
		}
		else
		{
			mineEdit->setPrefix("00");
		}
	};
	connect(widthEdit, &QSpinBox::valueChanged, this, pre);
	connect(heightEdit, &QSpinBox::valueChanged, this, pre);
	connect(mineEdit, &QSpinBox::valueChanged, this, pre);
	
	QPushButton* restart = new QPushButton("Restart Game");
	restart->setStyleSheet("color: #008081");
	auto custom = [this, widthEdit, heightEdit, mineEdit] { m_colSetting = widthEdit->value();
				   m_rowSetting = heightEdit->value(); m_mineSetting = mineEdit->value();
				   difficulty = Difficulty::STATS; m_customSettings->hide(); restartGame(); };
	connect(restart, &QPushButton::clicked, this, custom);

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
	if (m_highscoreWidget)
	{
		m_highscoreWidget->deleteLater();
		header->highscores.scoreChanged = false;
	}
	m_highscoreWidget = new QWidget();
	QGridLayout* layout = new QGridLayout();
	m_highscoreWidget->setLayout(layout);
	m_highscoreWidget->setMinimumSize(400, 300);
	m_highscoreWidget->setWindowTitle("Highscores");
	m_highscoreWidget->setStyleSheet(StyleSheet::OUTER.c_str());
	m_highscoreWidget->setWindowIcon(QIcon("resources/tiles/flag.png"));

	QLabel* high = new QLabel("HIGHSCORES");
	high->setAlignment(Qt::AlignCenter);
	high->setStyleSheet("color: #008081");
	layout->addWidget(high, 0, 0, 1, 2);
	for (int diff = 0; diff < 3; diff++)
	{
		std::string label, style;
		if (diff == 0)
		{
			label = "Beginner:";
			style = "color: #0100fe";
		}
		else if (diff == 1)
		{
			label = "Intermediate:";
			style = "color: #008001";
		}
		else if (diff == 2)
		{
			label = "Expert:";
			style = "color: #fe0000";
		}
		QLabel* difficulty = new QLabel(label.c_str());
		difficulty->setStyleSheet(style.c_str());
		layout->addWidget(difficulty, 1 + diff * 6, 0, 1, 2);

		for (int place = 0; place < 10; place++)
		{
			std::string score = header->highscores.formattedScore((Difficulty)diff, place, false);
			QLabel* qlabel = new QLabel((std::to_string(place + 1) + ". " + score).c_str());
			if (place < 5)
			{
				layout->addWidget(qlabel, 2 + diff * 6 + (place % 5), 0, 1, 1);
			}
			else
			{
				layout->addWidget(qlabel, 2 + diff * 6 + (place % 5), 1, 1, 1);
			}
		}
	}
}

void MainWindow::makeStatsWidget()
{
	if (m_statsWidget)
	{
		m_statsWidget->deleteLater();
		header->highscores.statChanged = false;
	}
	m_statsWidget = new QWidget();
	QGridLayout* layout = new QGridLayout();
	m_statsWidget->setLayout(layout);
	m_statsWidget->setMinimumSize(240, 120);
	m_statsWidget->setWindowTitle("Game Stats");
	m_statsWidget->setStyleSheet(StyleSheet::OUTER.c_str());
	m_statsWidget->setWindowIcon(QIcon("resources/tiles/eight.png"));

	QLabel* game = new QLabel("Games played: ");
	layout->addWidget(game, 1, 0);
	QLabel* won = new QLabel("Games won: ");
	layout->addWidget(won, 2, 0);
	QLabel* lost = new QLabel("Games lost: ");
	layout->addWidget(lost, 3, 0);
	QLabel* time = new QLabel("Total time played: ");
	layout->addWidget(time, 4, 0);
	QLabel* left = new QLabel("Left clicks: ");
	layout->addWidget(left, 5, 0);
	QLabel* right = new QLabel("Right clicks: ");
	layout->addWidget(right, 6, 0);
	for (int stat = 0; stat < 6; stat++)
	{
		layout->addWidget(new QLabel(header->highscores.getStat((Stats)stat).c_str()), stat + 1, 1);
	}
}
