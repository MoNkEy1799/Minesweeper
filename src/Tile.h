#pragma once
#include <QPushButton>
#include <array>
#include <QIcon>

enum class TileState {COVERED, UNCOVERED, FLAGGED};

class Tile : public QPushButton
{
public:
	Tile(int id, int rowCount, int columnCount, QWidget* partent = nullptr);
	~Tile();

	bool uncover();
	void checkNeighbours();

private:
	int m_id, m_row, m_column, m_count;
	int m_neighbours[8];
	bool m_mine;
	TileState m_state;

	std::array<int, 2> id2RowColumn();
};