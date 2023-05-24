#pragma once
#include <QWidget>
#include <QMetaObject>

#include <vector>
#include <memory>

class Tile;

class Board : public QWidget
{
public:
	Board(int rows, int columns, int mineCount, int tileSize = 20, QWidget* parent = nullptr);
	~Board();

	void gameOver(int id);

private:
	int m_rows, m_columns, m_tileSize, m_mineCount;
	std::array<std::string, 4> m_styleSheets;
	std::vector<std::unique_ptr<Tile>> m_tiles;

	void loadStyleSheets(const std::string& dirPath);
	void activateMines(int activator);
	std::vector<int> calculateNeighbours(int id);
};