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

<<<<<<< HEAD
	layout->setSpacing(0);
	layout->setContentsMargins(0, 0, 0, 0);
	setLayout(layout);
	setFixedSize(columns * 20, rows * 20);
=======
	setLayout(layout);
>>>>>>> 11f922720a7c20aca0a81d208ec49ea72086bf6d
}

Board::~Board()
{
}
