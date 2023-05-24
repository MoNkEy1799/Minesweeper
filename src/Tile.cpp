#include <QPushButton>
#include <QIcon>
#include <QSize>
#include <QFont>
#include <QMouseEvent>
#include <Qt>

#include <array>
#include <string>

#include "Tile.h"
#include "Board.h"

Tile::Tile(int id, int size, const std::array<std::string, 4>& styleSheets, QWidget* parent, Board* board)
	: QPushButton(parent),
	m_id(id), m_count(0), m_size(size), m_mine(false), m_board(board),
	m_state(TileState::COVERED), m_currentButton(MouseButton::LEFT), m_styles(styleSheets)
{
	std::vector<std::string> replace{ "#c0c0c0", borderSize()};
	setStyleSheet(stringFormat(m_styles[(int)m_state], replace).c_str());
	QFont font("Sans-Serif", 10);
	font.setBold(true);
	setFont(font);

	setFixedSize(size, size);
}

Tile::~Tile()
{
}

void Tile::placeMine()
{
	m_mine = true;
}

bool Tile::isMined()
{
	return m_mine;
}

TileState Tile::getState()
{
	return m_state;
}

void Tile::activate()
{
	if (m_currentButton == MouseButton::LEFT)
	{
		if (m_state == TileState::COVERED)
		{
			m_state = TileState::UNCOVERED;
			uncover();
		}

		else if (m_count and m_state == TileState::UNCOVERED and m_count == countFlaggedNeighbours())
		{
			for (Tile* tile : m_neighbours)
			{
				if (tile->getState() == TileState::COVERED)
				{
					tile->changeButton(MouseButton::LEFT);
					tile->activate();
				}
			}
		}
	}

	else if (m_currentButton == MouseButton::RIGHT)
	{
		if (m_state == TileState::UNCOVERED)
		{
			return;
		}

		if (m_state == TileState::COVERED)
		{
			m_state = TileState::FLAGGED;
			setText("\u2691");
		}

		else
		{
			m_state = TileState::COVERED;
			setText("");
		}
	}
}

void Tile::uncover(bool gameOver)
{
	if (m_mine)
	{
		setText("\u24c2");
		std::vector<std::string> replace{ "#ff0000" };
		setStyleSheet(stringFormat(m_styles[3], replace).c_str());
		m_board->gameOver(m_id);
		return;
	}

	setText(std::format("{}", m_count).c_str());
	std::vector<std::string> replace{ color() };
	setStyleSheet(stringFormat(m_styles[(int)m_state], replace).c_str());

	if (m_count == 0)
	{
		propagate();
	}
}

void Tile::addNeighbour(Tile* neighbour)
{
	m_neighbours.push_back(neighbour);
}

void Tile::checkMinedNeighbours()
{
	for (Tile* tile : m_neighbours)
	{
		if (tile->isMined())
		{
			m_count++;
		}
	}
}

int Tile::countFlaggedNeighbours()
{
	int flagCount = 0;

	for (Tile* tile : m_neighbours)
	{
		if (tile->getState() == TileState::FLAGGED)
		{
			flagCount++;
		}
	}

	return flagCount;
}

void Tile::changeButton(MouseButton button)
{
	m_currentButton = button;
}

void Tile::endGame(int id)
{
	disconnect(this, &Tile::clicked, nullptr, nullptr);

	if (m_mine and m_id != id and m_state != TileState::FLAGGED)
	{
		setText("\u24c2");
		std::vector<std::string> replace{ "#c0c0c0" };
		setStyleSheet(stringFormat(m_styles[0], replace).c_str());
	}

	else if (not m_mine and m_state == TileState::FLAGGED)
	{
		std::vector<std::string> replace{ "#ffa0a0", borderSize() };
		setStyleSheet(stringFormat(m_styles[0], replace).c_str());
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

std::string Tile::borderSize()
{
	return std::to_string(std::round(m_size / 10.0));
}

std::string Tile::color()
{
	switch (m_count)
	{
	case 0:
		return "#c0c0c0";

	case 1:
		return "#0001fd";

	case 2:
		return "#017e00";

	case 3:
		return "#fe0000";

	case 4:
		return "#010180";

	case 5:
		return "#810101";

	case 6:
		return "#008080";

	case 7:
		return "#000000";

	case 8:
		return "#808080";
	}

	return "#c0c0c0";
}

void Tile::mouseReleaseEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		m_currentButton = MouseButton::LEFT;
	}

	else if (event->button() == Qt::RightButton)
	{
		m_currentButton = MouseButton::RIGHT;
	}

	emit QPushButton::clicked();
}

void Tile::propagate()
{
	for (Tile* tile : m_neighbours)
	{
		tile->changeButton(MouseButton::LEFT);
		tile->activate();
	}
}
