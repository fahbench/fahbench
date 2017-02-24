#include "ProgressBar.h"

ProgressBar::ProgressBar()
{
	pbar = new QProgressBar;
}

void ProgressBar::setMaximum(int max) {
	pbar->setMaximum(max);
}

int ProgressBar::maximum(void) const {
	return pbar->maximum();
}

void ProgressBar::setMinimum(int min) {
	pbar->setMinimum(min);
}

int ProgressBar::minimum(void) const {
	return pbar->minimum();
}

void ProgressBar::setValue(int val) {
	pbar->setValue(val);
}

int ProgressBar::value(void) const {
	return pbar->value();
}

void ProgressBar::setTaskbarVisible(bool vis) { }

bool ProgressBar::isTaskbarVisible(void) const { }

void ProgressBar::reset(void) {
	pbar->reset();
}

QWidget * ProgressBar::widget(void) const {
	return pbar;
}

void ProgressBar::initialize(QWindow *) {}
