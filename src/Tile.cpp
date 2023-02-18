#include "Tile.h"
#include <QPushButton>
#include <QIcon>
#include <QSize>

Tile::Tile(int id, int rowCount, int columnCount, QWidget* parent)
	: QPushButton(parent), m_id(id), m_row(rowCount), m_column(columnCount),
	m_count(0), m_mine(false), m_neighbours{0, 0, 0, 0, 0, 0, 0, 0}, m_state(TileState::COVERED)
{
	setStyleSheet("");
	QPushButton::connect(this, &QPushButton::clicked, this, &Tile::uncover);
	setFixedSize(20, 20);
}

Tile::~Tile()
{
}

bool Tile::uncover()
{
	if (m_state == TileState::COVERED)
	{
		setIcon(QIcon("resources/TileUncovered.png"));
		m_state = TileState::UNCOVERED;
	}
	return false;
}
