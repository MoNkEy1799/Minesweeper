#include <QPushButton>
#include <QIcon>
#include <QSize>

#include <array>

#include "Tile.h"

bool Tile::m_firstMove = true;

Tile::Tile(int id, int size, const std::array<std::string, 3>& styleSheets, QWidget* parent)
	: QPushButton(parent),
	m_id(id), m_count(0), m_size(size), m_mine(false),
	m_state(TileState::COVERED), m_styles(styleSheets)
{
	std::vector<std::string> replace{ std::to_string(int(m_size / 10.0)) };
	setStyleSheet(stringFormat(styleSheets[(int)m_state], replace).c_str());

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
		setStyleSheet((m_styles[(int)m_state]).c_str());
	}
}

std::string stringFormat(std::string input, std::vector<std::string>& replace)
{
	size_t position = 0;

	for (int i = 0; i < replace.size(); i++)
	{
		std::string find = "{" + std::to_string(i) + "}";
		while ((position = input.find(find, position)) != std::string::npos)
		{
			input.replace(position, find.length(), replace[i]);
		}
	}

	return input;
}
