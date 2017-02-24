#ifndef PROGRESS_BAR_H_
#define PROGRESS_BAR_H_

#include <QProgressBar>

#ifdef _WIN32
#include <QWinTaskbarProgress>
#endif

class ProgressBar {
private:
	QProgressBar * pbar;
#ifdef _WIN32
	QWinTaskbarProgress * tbpbar;
#endif
public:
	ProgressBar();

	int minimum(void) const;
	void setMinimum(int);

	int maximum(void) const;
	void setMaximum(int);

	int value(void) const;
	void setValue(int);

	bool isTaskbarVisible(void) const;
	void setTaskbarVisible(bool);

	void reset(void);
	
	void initialize(QWindow *);
	QWidget * widget(void) const;
};


#endif // PROGRESS_BAR_H_