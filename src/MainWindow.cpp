#include <QLabel>
#include <QFont>
#include <QWidget>
#include <QGridlayout>
#include <QPushButton>

#include "MainWindow.h"
#include "Board.h"

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
{
	Board* board = new Board(10, 10, this);

	m_mainWidget = new QWidget(this);

	QGridLayout* layout = new QGridLayout();
	layout->addWidget(board);
	m_mainWidget->setLayout(layout);

	setCentralWidget(m_mainWidget);
}

MainWindow::~MainWindow()
{
}
