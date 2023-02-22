#include <QGridLayout>
#include <QMetaObject>

#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <memory>
#include <cstdlib>
#include <set>
#include <iostream>

#include "Board.h"
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
			std::unique_ptr<Tile> tile = std::make_unique<Tile>(id, m_tileSize, m_styleSheets, this);
			tile->connect(tile.get(), &Tile::clicked, this, [this, id]() { activateMines(id); });
			m_tiles.push_back(std::move(tile));

			layout->addWidget(m_tiles[id].get(), i, j);
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

void Board::loadStyleSheets(const std::string& dirPath)
{
	std::ifstream infile;
	std::stringstream stream[3];
	std::string files[3] = { "covered.txt", "uncovered.txt", "flagged.txt" };

	for (int i = 0; i < 3; i++)
	{
		infile.open(dirPath + files[i]);
		stream[i] << infile.rdbuf();
		m_styleSheets[i] = stream[i].str();
		infile.close();
	}
}

void Board::activateMines(int activator)
{
	m_tiles[activator]->uncover();

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
	}
}
