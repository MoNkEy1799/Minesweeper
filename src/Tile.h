#pragma once
#include <QPushButton>
#include <QIcon>

#include <string>
#include <vector>
#include <array>

enum class TileState {COVERED, UNCOVERED, FLAGGED};

class Tile : public QPushButton
{
public:
	Tile(int id, int size, std::array<const std::string*, 3> styleSheets, QWidget* partent = nullptr);
	~Tile();

	void uncover();
	void checkNeighbours();

private:
	int m_id, m_count, m_size;
	std::vector<Tile*> m_neighbours;
	bool m_mine;
	static bool m_firstMove;
	TileState m_state;
	std::array<const std::string*, 3> m_styles;

	std::array<int, 2> id2RowColumn();
};