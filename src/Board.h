#pragma once
#include <QWidget>
#include <vector>
#include <memory>

struct StyleSheet
{
	const static std::string COVERED, UNCOVERED, MINED, HEADER, OUTER, MENU;
};

class Tile;
class MainWindow;

class Board : public QWidget
{
public:
	Board(int rows, int columns, int mineCount, int tileSize = 20, QWidget* parent = nullptr, MainWindow* main = nullptr);

	bool checkForWin();
	void gameWon();
	void gameOver(int id);
	void reset();
	QSize boardSize();

	MainWindow* mainWindow;
	int flagCount, mineCount;

private:
	int m_rows, m_columns, m_tileSize;
	std::vector<Tile*> m_tiles;

	void activateMines(int activator);
	std::vector<int> calculateNeighbours(int id);
};