#pragma once

#include <QWidget>
#include <QPushButton>
#include <QLabel>

#include <vector>

class MainWindow;

class Header : public QWidget
{
public:
	Header(int size, QWidget* parent = nullptr, MainWindow* main = nullptr);
	~Header();

private:
	MainWindow* m_mainWindow;
	QPushButton* m_restart;
	std::array<QLabel*, 10> m_numbers;
};