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
#include "MainWindow.h"
#include "HeaderWidget.h"

MouseButton Tile::currentButton = MouseButton::NONE;
std::array<QIcon, 12> Tile::m_icons;

Tile::Tile(int id, int size, QWidget* parent, Board* board)
	: QPushButton(parent),
	m_id(id), m_count(0), m_size(size), m_mine(false), m_board(board),
	m_state(TileState::COVERED)
{
	setStyleSheet(StyleSheet::COVERED.c_str());
	setFixedSize(size, size);
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
	if (Tile::currentButton == MouseButton::LEFT)
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

	else if (Tile::currentButton == MouseButton::RIGHT)
	{
		if (m_state == TileState::UNCOVERED)
		{
			return;
		}
		if (m_state == TileState::COVERED)
		{
			m_state = TileState::FLAGGED;
			m_board->flagCount++;
			setIcon(m_icons[Icons::FLAG]);
			setIconSize(QSize(m_size, m_size));
			m_board->mainWindow->header->changeMineCount(m_board->mineCount - m_board->flagCount);
		}
		else
		{
			m_state = TileState::COVERED;
			m_board->flagCount--;
			setIcon(QIcon());
			m_board->mainWindow->header->changeMineCount(m_board->mineCount - m_board->flagCount);
		}
	}
}

void Tile::uncover()
{
	if (m_mine)
	{
		setIcon(m_icons[Icons::MINE]);
		setIconSize(QSize(m_size, m_size));
		setStyleSheet(StyleSheet::MINED.c_str());
		m_board->gameOver(m_id);
		return;
	}

	setIcon(m_icons[m_count]);
	setIconSize(QSize(m_size, m_size));
	setStyleSheet(StyleSheet::UNCOVERED.c_str());

	if (m_board->checkForWin())
	{
		m_board->gameWon();
	}

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
	Tile::currentButton = button;
}

void Tile::endGame(int id)
{
	if (m_mine and m_id != id and m_state != TileState::FLAGGED)
	{
		setIcon(m_icons[Icons::MINE]);
		setIconSize(QSize(m_size, m_size));
		setStyleSheet(StyleSheet::UNCOVERED.c_str());
	}

	else if (not m_mine and m_state == TileState::FLAGGED)
	{
		setIcon(m_icons[Icons::WRONG]);
		setIconSize(QSize(m_size, m_size));
		setStyleSheet(StyleSheet::UNCOVERED.c_str());
	}
}

void Tile::reset()
{
	m_count = 0;
	m_mine = false;
	m_state = TileState::COVERED;
	setStyleSheet(StyleSheet::COVERED.c_str());
	setIcon(m_icons[0]);
}

void Tile::loadIcons()
{
	Tile::m_icons =
	{
		QIcon(),
		QIcon("resources/tiles/one.png"),
		QIcon("resources/tiles/two.png"),
		QIcon("resources/tiles/three.png"),
		QIcon("resources/tiles/four.png"),
		QIcon("resources/tiles/five.png"),
		QIcon("resources/tiles/six.png"),
		QIcon("resources/tiles/seven.png"),
		QIcon("resources/tiles/eight.png"),
		QIcon("resources/tiles/flag.png"),
		QIcon("resources/tiles/mine.png"),
		QIcon("resources/tiles/wrong.png")
	};
}

void Tile::mouseReleaseEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		Tile::currentButton = MouseButton::LEFT;
	}
	else if (event->button() == Qt::RightButton)
	{
		Tile::currentButton = MouseButton::RIGHT;
		emit QPushButton::clicked();
	}
	return QPushButton::mouseReleaseEvent(event);
}

void Tile::propagate()
{
	for (Tile* tile : m_neighbours)
	{
		tile->changeButton(MouseButton::LEFT);
		tile->activate();
	}
}
