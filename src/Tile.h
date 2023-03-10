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
	Tile(int id, int size, const std::array<std::string, 3>& styleSheets, QWidget* partent = nullptr);
	~Tile();

	void placeMine();
	void activate();
	void uncover();
	void checkNeighbours();

private:
	int m_id, m_count, m_size;
	std::vector<Tile*> m_neighbours;
	bool m_mine;
	TileState m_state;
	const std::array<std::string, 3>& m_styles;

	std::string stringFormat(std::string input, std::vector<std::string>& replace);
	std::array<int, 2> id2RowColumn();
};