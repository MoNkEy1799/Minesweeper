#pragma once
#include <QPushButton>
#include <QIcon>
#include <QMouseEvent>

#include <string>
#include <vector>
#include <array>
#include <unordered_map>

enum class TileState { COVERED, UNCOVERED, FLAGGED };
enum class MouseButton{ NONE, LEFT, RIGHT };

class Board;

class Tile : public QPushButton
{
public:
	Tile(int id, int size, QWidget* partent = nullptr, Board* board = nullptr);

	void placeMine();
	bool isMined();
	TileState getState();
	void activate();
	void uncover();
	void addNeighbour(Tile* neighbour);
	void checkMinedNeighbours();
	int countFlaggedNeighbours();
	void changeButton(MouseButton button);
	void endGame(int id);

	static MouseButton currentButton;
	static void loadIcons();

private:
	int m_id, m_count, m_size;
	std::vector<Tile*> m_neighbours;
	bool m_mine;
	TileState m_state;
	Board* m_board;
	static std::unordered_map<std::string, QIcon*> m_icons;

	void propagate();

protected:
	void mouseReleaseEvent(QMouseEvent* event) override;
};