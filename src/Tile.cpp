#include <QPushButton>
#include <QIcon>
#include <QSize>
#include <QFont>
#include <QMouseEvent>
#include <Qt>

#include <array>
#include <string>
#include <iostream>
#include <unordered_map>

#include "Tile.h"
#include "Board.h"
#include "MainWindow.h"
#include "HeaderWidget.h"

MouseButton Tile::currentButton = MouseButton::NONE;

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
			setIcon(*m_icons["flag"]);
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
		setIcon(*m_icons["mine"]);
		setIconSize(QSize(m_size, m_size));
		setStyleSheet(StyleSheet::MINED.c_str());
		m_board->gameOver(m_id);
		return;
	}

	if (m_count != 0)
	{
		setIcon(*m_icons[std::to_string(m_count)]);
		setIconSize(QSize(m_size, m_size));
	}
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
		setIcon(*m_icons["mine"]);
		setIconSize(QSize(m_size, m_size));
		setStyleSheet(StyleSheet::UNCOVERED.c_str());
	}

	else if (not m_mine and m_state == TileState::FLAGGED)
	{
		setIcon(*m_icons["wrong"]);
		setIconSize(QSize(m_size, m_size));
		setStyleSheet(StyleSheet::UNCOVERED.c_str());
	}
}

void Tile::loadIcons()
{
	Tile::m_icons["flag"] = new QIcon("resources/tiles/flag.png");
	Tile::m_icons["mine"] = new QIcon("resources/tiles/mine.png");
	Tile::m_icons["wrong"] = new QIcon("resources/tiles/wrong.png");
	Tile::m_icons["1"] = new QIcon("resources/tiles/one.png");
	Tile::m_icons["2"] = new QIcon("resources/tiles/two.png");
	Tile::m_icons["3"] = new QIcon("resources/tiles/three.png");
	Tile::m_icons["4"] = new QIcon("resources/tiles/four.png");
	Tile::m_icons["5"] = new QIcon("resources/tiles/five.png");
	Tile::m_icons["6"] = new QIcon("resources/tiles/six.png");
	Tile::m_icons["7"] = new QIcon("resources/tiles/seven.png");
	Tile::m_icons["8"] = new QIcon("resources/tiles/eight.png");
	Tile::m_icons["9"] = new QIcon("resources/tiles/nine.png");
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
