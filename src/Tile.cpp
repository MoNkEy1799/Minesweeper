#include <QPushButton>
#include <QIcon>
#include <QSize>

#include <array>
#include <string>

#include "Tile.h"

Tile::Tile(int id, int size, const std::array<std::string, 3>& styleSheets, QWidget* parent)
	: QPushButton(parent),
	m_id(id), m_count(0), m_size(size), m_mine(false),
	m_state(TileState::COVERED), m_styles(styleSheets)
{
	std::vector<std::string> replace{ std::to_string(std::round(m_size / 10.0)) };
	setStyleSheet(stringFormat(m_styles[(int)m_state], replace).c_str());

	setFixedSize(size, size);
}

Tile::~Tile()
{
}

void Tile::placeMine()
{
	m_mine = true;
}

void Tile::activate()
{
	if (m_state == TileState::COVERED)
	{
		m_state = TileState::UNCOVERED;
		uncover();
	}
}

void Tile::uncover()
{
	std::vector<std::string> replace{ std::to_string(std::round(m_size / 10.0)) };
	setStyleSheet(stringFormat(m_styles[(int)m_state], replace).c_str());

	if (m_mine)
	{
		setText("M");
		setStyleSheet("color: red");
	}
}

std::string Tile::stringFormat(std::string input, std::vector<std::string>& replace)
{
	for (int i = 0; i < replace.size(); i++)
	{
		size_t position = 0;

		std::string find = "{" + std::to_string(i) + "}";
		while ((position = input.find(find, position)) != std::string::npos)
		{
			input.replace(position, find.length(), replace[i]);
		}
	}
	return input;
}
