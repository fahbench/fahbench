#include "OneShotButton.h"

OneShotButton::OneShotButton(const QString &text, QWidget *parent) : QPushButton(text, parent) {
    connect(this, SIGNAL(clicked()), this, SLOT(disable()));
}

void OneShotButton::disable() { 
    setEnabled(false); 
}

void OneShotButton::enable() { 
    setEnabled(true); 
}