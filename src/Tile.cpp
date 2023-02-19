#include <QPushButton>
#include <QIcon>
#include <QSize>

#include <array>

#include "Tile.h"

bool Tile::m_firstMove = true;

Tile::Tile(int id, int size, std::array<const std::string*, 3> styleSheets, QWidget* parent)
	: QPushButton(parent),
	m_id(id), m_count(0), m_size(size), m_mine(false),
	m_state(TileState::COVERED), m_styles(styleSheets)
{
	setStyleSheet((*styleSheets[(int)m_state]).c_str());

	QPushButton::connect(this, &QPushButton::clicked, this, &Tile::uncover);
	setFixedSize(size, size);
}

Tile::~Tile()
{
}

void Tile::uncover()
{
	if (m_firstMove)
	{
		m_firstMove = false;
	}

	if (m_state == TileState::COVERED)
	{
		m_state = TileState::UNCOVERED;
		setStyleSheet((*m_styles[(int)m_state]).c_str());
	}
}
