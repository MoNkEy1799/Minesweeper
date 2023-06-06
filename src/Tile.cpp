#include <QPushButton>
#include <QIcon>
#include <QSize>
#include <QFont>
#include <QMouseEvent>
#include <Qt>

#include <array>
#include <string>
#include <iostream>

#include "Tile.h"
#include "Board.h"

Tile::Tile(int id, int size, QWidget* parent, Board* board)
	: QPushButton(parent),
	m_id(id), m_count(0), m_size(size), m_mine(false), m_board(board),
	m_state(TileState::COVERED), m_currentButton(MouseButton::LEFT)
{
	setStyleSheet(StyleSheet::COVERED.c_str());
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
			setIcon(QIcon("resources/tiles/flag.png"));
			setIconSize(QSize(m_size, m_size));
		}

		else
		{
			m_state = TileState::COVERED;
			setIcon(QIcon());
		}
	}
}

void Tile::uncover()
{
	if (m_mine)
	{
		setIcon(QIcon("resources/tiles/mine.png"));
		setIconSize(QSize(m_size, m_size));
		setStyleSheet(StyleSheet::MINED.c_str());
		m_board->gameOver(m_id);
		return;
	}


	setIcon(QIcon(countToFilepath()));
	setIconSize(QSize(m_size, m_size));
	setStyleSheet(StyleSheet::UNCOVERED.c_str());

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
	disconnect(this, &QPushButton::clicked, nullptr, nullptr);

	if (m_mine and m_id != id and m_state != TileState::FLAGGED)
	{
		setIcon(QIcon("resources/tiles/mine.png"));
		setIconSize(QSize(m_size, m_size));
		setStyleSheet(StyleSheet::UNCOVERED.c_str());
	}

	else if (not m_mine and m_state == TileState::FLAGGED)
	{
		setIcon(QIcon("resources/tiles/wrong.png"));
		setIconSize(QSize(m_size, m_size));
		setStyleSheet(StyleSheet::UNCOVERED.c_str());
	}
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

QString Tile::countToFilepath()
{
	std::string path = "resources/tiles/";
	switch (m_count)
	{
	case 1:
		return QString((path + "one.png").c_str());
	case 2:
		return QString((path + "two.png").c_str());
	case 3:
		return QString((path + "three.png").c_str());
	case 4:
		return QString((path + "four.png").c_str());
	case 5:
		return QString((path + "five.png").c_str());
	case 6:
		return QString((path + "six.png").c_str());
	case 7:
		return QString((path + "seven.png").c_str());
	case 8:
		return QString((path + "eight.png").c_str());
	}
	// For m_count == 0, the QIcon load will silently fail. This is probably not nice but I don't care.
	return QString();
}

void Tile::propagate()
{
	for (Tile* tile : m_neighbours)
	{
		tile->changeButton(MouseButton::LEFT);
		tile->activate();
	}
}
