#include <QGridLayout>
#include <QMetaObject>

#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <memory>
#include <set>
#include <iostream>

#include "Board.h"
#include "Tile.h"
#include "Random.h"
#include "MainWindow.h"
#include "HeaderWidget.h"

const std::string StyleSheet::COVERED =
	"QPushButton{"
		"background: #c0c0c0;"
		"border-top: 2px solid #ffffff;"
		"border-left: 2px solid #ffffff;"
		"border-bottom: 2px solid #808080;"
		"border-right: 2px solid #808080;}"

	"QPushButton::pressed{"
		"background: #c0c0c0;"
		"border: 1px solid #808080;}";

const std::string StyleSheet::UNCOVERED =
	"QPushButton{"
		"background: #c0c0c0;"
		"border: 1px solid #808080;}";

const std::string StyleSheet::MINED =
	"QPushButton{"
		"background: #ff0000;"
		"border: 1px solid #808080;}";

const std::string StyleSheet::HEADER =
	"QWidget{"
		"background: #c0c0c0;"
		"border-top: 2px solid #808080;"
		"border-left: 2px solid #808080;"
		"border-bottom: 2px solid #ffffff;"
		"border-right: 2px solid #ffffff;}"

	"QLabel{"
		"background: #000000;"
		"border: 0px solid;}";

const std::string StyleSheet::OUTER =
	"QWidget{"
		"background: #c0c0c0;"
		"border-top: 2px solid #ffffff;"
		"border-left: 2px solid #ffffff;"
		"border-bottom: 2px solid #808080;"
		"border-right: 2px solid #808080;}";

Board::Board(int rows, int columns, int mineCount, int tileSize, QWidget* parent, MainWindow* main)
	: QWidget(parent), mainWindow(main), flagCount(0),
	m_rows(rows), m_columns(columns), m_tileSize(tileSize), mineCount(mineCount)
{
	QGridLayout* layout = new QGridLayout(this);

	for (int i = 0; i < m_rows; i++)
	{
		for (int j = 0; j < m_columns; j++)
		{
			int id = i * m_columns + j;
			Tile* tile = new Tile(id, m_tileSize, this, this);
			connect(tile, &QPushButton::clicked, this, [this, id]() { activateMines(id); });
			m_tiles.push_back(tile);
			layout->addWidget(m_tiles[id], i, j);
		}
	}

	for (int i = 0; i < m_rows * m_columns; i++)
	{
		std::vector<int> neighbours = calculateNeighbours(i);
		for (int n : neighbours)
		{
			m_tiles[i]->addNeighbour(m_tiles[n]);
		}
	}

	layout->setSpacing(0);
	layout->setContentsMargins(2, 2, 2, 2);
	setLayout(layout);
	setFixedSize(m_columns * m_tileSize + 4, m_rows * m_tileSize + 4);
	setStyleSheet(StyleSheet::HEADER.c_str());
	setLayout(layout);
	mainWindow->header->resetTimer();
}

bool Board::checkForWin()
{
	int coveredCount = 0;
	for (int i = 0; i < m_rows * m_columns; i++)
	{
		if (m_tiles[i]->getState() == TileState::COVERED || m_tiles[i]->getState() == TileState::FLAGGED)
		{
			coveredCount++;
		}
	}
	if (coveredCount == mineCount)
	{
		return true;
	}
	return false;
}

// copy pasta
void Board::gameWon()
{
	mainWindow->header->stopTimer();
	mainWindow->header->changeSmiley(true);
	mainWindow->header->changeMineCount(0);
	for (Tile* tile : m_tiles)
	{
		if (tile->getState() == TileState::COVERED)
		{
			tile->setIcon(QIcon("resources/tiles/flag.png"));
		}
		disconnect(tile, &QPushButton::clicked, nullptr, nullptr);
	}
}

void Board::gameOver(int id)
{
	mainWindow->header->stopTimer();
	mainWindow->header->changeSmiley(false);
	for (Tile* tile : m_tiles)
	{
		disconnect(tile, &QPushButton::clicked, nullptr, nullptr);
		tile->endGame(id);
	}
}

void Board::activateMines(int activator)
{
	if (Tile::currentButton == MouseButton::RIGHT)
	{
		m_tiles[activator]->activate();
		return;
	}
	if (m_tiles[activator]->getState() == TileState::FLAGGED || Tile::currentButton != MouseButton::LEFT)
	{
		return;
	}

	mainWindow->header->startTimer();
	std::set<int> setMines;
	Random r;

	while (setMines.size() < mineCount)
	{
		int max = (m_rows * m_columns) - 1;
		int mine = r.randInt(0, max);
		
		if (mine == activator)
		{
			continue;
		}

		if (setMines.find(mine) == setMines.end())
		{
			m_tiles[mine]->placeMine();
			setMines.insert(mine);
		}
	}
	
	for (Tile* tile : m_tiles)
	{
		disconnect(tile, &QPushButton::clicked, nullptr, nullptr);
		connect(tile, &QPushButton::clicked, tile, &Tile::activate);
		tile->checkMinedNeighbours();
	}

	m_tiles[activator]->activate();
}

std::vector<int> Board::calculateNeighbours(int id)
{
	std::vector<int> neighbours;
	int x = id % m_columns;
	int y = id / m_columns;

	if (x == 0 and y == 0)
	{
		neighbours.push_back(id + 1);
		neighbours.push_back(id + m_columns);
		neighbours.push_back(id + m_columns + 1);
	}

	else if (x == m_columns - 1 and y == 0)
	{
		neighbours.push_back(id - 1);
		neighbours.push_back(id + m_columns - 1);
		neighbours.push_back(id + m_columns);
	}

	else if (x == 0 and y == m_rows - 1)
	{
		neighbours.push_back(id - m_columns);
		neighbours.push_back(id - m_columns + 1);
		neighbours.push_back(id + 1);
	}

	else if (x == m_columns - 1 and y == m_rows - 1)
	{
		neighbours.push_back(id - 1);
		neighbours.push_back(id - m_columns - 1);
		neighbours.push_back(id - m_columns);
	}

	else if (y == 0)
	{
		neighbours.push_back(id - 1);
		neighbours.push_back(id + 1);
		neighbours.push_back(id + m_columns - 1);
		neighbours.push_back(id + m_columns);
		neighbours.push_back(id + m_columns + 1);
	}

	else if (y == m_rows - 1)
	{
		neighbours.push_back(id - 1);
		neighbours.push_back(id + 1);
		neighbours.push_back(id - m_columns - 1);
		neighbours.push_back(id - m_columns);
		neighbours.push_back(id - m_columns + 1);
	}

	else if (x == 0)
	{
		neighbours.push_back(id - m_columns);
		neighbours.push_back(id - m_columns + 1);
		neighbours.push_back(id + 1);
		neighbours.push_back(id + m_columns);
		neighbours.push_back(id + m_columns + 1);
	}

	else if (x == m_columns - 1)
	{
		neighbours.push_back(id - m_columns);
		neighbours.push_back(id - m_columns - 1);
		neighbours.push_back(id - 1);
		neighbours.push_back(id + m_columns);
		neighbours.push_back(id + m_columns - 1);
	}

	else
	{
		neighbours.push_back(id - m_columns - 1);
		neighbours.push_back(id - m_columns);
		neighbours.push_back(id - m_columns + 1);
		neighbours.push_back(id - 1);
		neighbours.push_back(id + 1);
		neighbours.push_back(id + m_columns - 1);
		neighbours.push_back(id + m_columns);
		neighbours.push_back(id + m_columns + 1);
	}

	return neighbours;
}
