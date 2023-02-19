#include <QGridLayout>

#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <memory>

#include "Board.h"

Board::Board(int rows, int columns, int tileSize, QWidget* parent)
	: QWidget(parent),
	m_rows(rows), m_columns(columns), m_tileSize(tileSize)
{
	loadStyleSheets("resources/stylesheets/");

	std::array<const std::string*, 3> styleSheets = { &m_coveredStyle, &m_uncoveredStyle, &m_flaggedStyle };

	QGridLayout* layout = new QGridLayout(this);

	for (int i = 0; i < m_rows; i++)
	{
		for (int j = 0; j < m_columns; j++)
		{
			int id = i * m_rows + j;
			Tile* tile = new Tile(id, m_tileSize, styleSheets, this);
			m_tiles.push_back(tile);

			layout->addWidget(tile, i, j);
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
}

void Board::loadStyleSheets(const std::string dirPath)
{
	std::ifstream infile;
	std::stringstream stream[3];
	std::string files[3] = { "covered.txt", "uncovered.txt", "flagged.txt" };

	for (int i = 0; i < 3; i++)
	{
		infile.open(dirPath + files[i]);
		stream[i] << infile.rdbuf();
		infile.close();
	}

	m_coveredStyle = stream[0].str();
	m_uncoveredStyle = stream[1].str();
	m_flaggedStyle = stream[2].str();
}
