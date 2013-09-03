// This is a one shot button that disables itself after being clicked once. 
#ifndef ONESHOTBUTTON_H_
#define ONESHOTBUTTON_H_

#include <QPushButton>
#include <QString>
#include <QWidget>

class OneShotButton : public QPushButton {
    
    Q_OBJECT

public:
    OneShotButton(const QString &text, QWidget *parent = 0);

public slots:
    void disable();
    void enable();
};

#endif  