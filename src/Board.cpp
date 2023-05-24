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

Board::Board(int rows, int columns, int mineCount, int tileSize, QWidget* parent)
	: QWidget(parent),
	m_rows(rows), m_columns(columns), m_tileSize(tileSize), m_mineCount(mineCount)
{
	loadStyleSheets("resources/stylesheets/");

	QGridLayout* layout = new QGridLayout(this);

	for (int i = 0; i < m_rows; i++)
	{
		for (int j = 0; j < m_columns; j++)
		{
			int id = i * m_rows + j;
			std::unique_ptr<Tile> tile = std::make_unique<Tile>(id, m_tileSize, m_styleSheets, this, this);
			tile->connect(tile.get(), &Tile::clicked, this, [this, id]() { activateMines(id); });
			m_tiles.push_back(std::move(tile));

			layout->addWidget(m_tiles[id].get(), i, j);
		}
	}

	for (int i = 0; i < m_rows * m_columns; i++)
	{
		std::vector<int> neighbours = calculateNeighbours(i);
		for (int n : neighbours)
		{
			m_tiles[i]->addNeighbour(m_tiles[n].get());
		}
	}

	layout->setSpacing(0);
	layout->setContentsMargins(0, 0, 0, 0);
	setLayout(layout);
	setFixedSize(m_columns * m_tileSize, m_rows * m_tileSize);
	setLayout(layout);
}

Board::~Board()
{
	m_tiles.clear();
}

void Board::gameOver(int id)
{
	for (std::unique_ptr<Tile>& tile : m_tiles)
	{
		tile->endGame(id);
	}
}

void Board::loadStyleSheets(const std::string& dirPath)
{
	std::ifstream infile;
	std::stringstream stream[4];
	std::string files[4] = { "covered.txt", "uncovered.txt", "flagged.txt", "mined.txt" };

	for (int i = 0; i < 4; i++)
	{
		infile.open(dirPath + files[i]);
		stream[i] << infile.rdbuf();
		m_styleSheets[i] = stream[i].str();
		infile.close();
	}
}

void Board::activateMines(int activator)
{
	std::set<int> setMines;
	Random r;

	while (setMines.size() < m_mineCount)
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
	
	for (std::unique_ptr<Tile>& tile : m_tiles)
	{
		tile->disconnect(tile.get(), &Tile::clicked, nullptr, nullptr);
		tile->connect(tile.get(), &Tile::clicked, tile.get(), &Tile::activate);
		tile->checkMinedNeighbours();
	}

	m_tiles[activator]->activate();
}

std::vector<int> Board::calculateNeighbours(int id)
{
	std::vector<int> neighbours;
	int x = id % m_columns;
	int y = id / m_rows;

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
