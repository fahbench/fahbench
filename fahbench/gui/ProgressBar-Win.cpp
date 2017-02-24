#include "ProgressBar.h"

#include <QWinTaskbarButton>

ProgressBar::ProgressBar()
	: pbar(nullptr)
	, tbpbar(nullptr)
{
	pbar = new QProgressBar;
}

void ProgressBar::initialize(QWindow * win) {
	auto tbbut = new QWinTaskbarButton();
	tbbut->setWindow(win);
	tbpbar = tbbut->progress();
}

void ProgressBar::setMaximum(int max) {
	pbar->setMaximum(max);
	tbpbar->setMaximum(max);
}

int ProgressBar::maximum(void) const {
	return pbar->maximum();
}

void ProgressBar::setMinimum(int min) {
	pbar->setMinimum(min);
	tbpbar->setMinimum(min);
}

int ProgressBar::minimum(void) const {
	return pbar->minimum();
}

void ProgressBar::setValue(int val) {
	pbar->setValue(val);
	tbpbar->setValue(val);
}

int ProgressBar::value(void) const {
	return pbar->value();
}

void ProgressBar::setTaskbarVisible(bool vis) {
	tbpbar->setVisible(vis);
}

bool ProgressBar::isTaskbarVisible(void) const {
	// Note: This is for taskbar visibility
	return tbpbar->isVisible();
}

void ProgressBar::reset(void) {
	pbar->reset();
	tbpbar->reset();
}

QWidget * ProgressBar::widget(void) const {
	return pbar;
}
