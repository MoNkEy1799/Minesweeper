#pragma once
#include <QPushButton>
#include <QIcon>
#include <QMouseEvent>

#include <string>
#include <vector>
#include <array>

enum class TileState {COVERED, UNCOVERED, FLAGGED};

enum class MouseButton{LEFT, RIGHT};

class Board;

class Tile : public QPushButton
{
public:
	Tile(int id, int size, const std::array<std::string, 4>& styleSheets, QWidget* partent = nullptr, Board* board = nullptr);
	~Tile();

	void placeMine();
	bool isMined();
	TileState getState();
	void activate();
	void uncover(bool gameOver = false);
	void addNeighbour(Tile* neighbour);
	void checkMinedNeighbours();
	int countFlaggedNeighbours();
	void changeButton(MouseButton button);
	void endGame(int id);

private:
	int m_id, m_count, m_size;
	std::vector<Tile*> m_neighbours;
	bool m_mine;
	TileState m_state;
	const std::array<std::string, 4>& m_styles;
	MouseButton m_currentButton;
	Board* m_board;

	std::string stringFormat(std::string input, std::vector<std::string>& replace);
	std::string borderSize();
	std::string color();

	void propagate();

protected:
	void mouseReleaseEvent(QMouseEvent* event) override;
};