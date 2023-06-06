#pragma once
#include <QWidget>
#include <QMetaObject>

#include <vector>
#include <memory>

struct StyleSheet
{
	const static std::string COVERED, UNCOVERED, MINED;
};

class Tile;
class MainWindow;

class Board : public QWidget
{
public:
	Board(int rows, int columns, int mineCount, int tileSize = 20, QWidget* parent = nullptr, MainWindow* main = nullptr);
	~Board();

	void gameOver(int id);

private:
	MainWindow* m_mainWindow;
	int m_rows, m_columns, m_tileSize, m_mineCount;
	std::vector<Tile*> m_tiles;

	void activateMines(int activator);
	std::vector<int> calculateNeighbours(int id);
};