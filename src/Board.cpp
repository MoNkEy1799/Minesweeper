#include "Board.h"
#include "Tile.h"
#include <QGridLayout>

Board::Board(int rows, int columns, QWidget* parent)
{
	QGridLayout* layout = new QGridLayout(this);

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			int id = i * rows + j;
			Tile* tile = new Tile(id, rows, columns, this);

			layout->addWidget(tile, i, j);
		}	
	}

	setLayout(layout);
}

Board::~Board()
{
}
