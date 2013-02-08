#ifndef INPUTHOOK_H
#define INPUTHOOK_H

#include <MyWidget/Event>
#include <MyWidget/signal>

#include "gui/minimapview.h"

class InputHookBase {
  public:
    virtual ~InputHookBase(){}

    MyWidget::signal<> onRemove;

    virtual void mouseDownEvent ( MyWidget::MouseEvent & e ){ e.ignore(); }
    virtual void mouseUpEvent   ( MyWidget::MouseEvent & e ){ e.ignore();}

    virtual void mouseMoveEvent ( MyWidget::MouseEvent & e ){ e.ignore(); }

    virtual void mouseWheelEvent ( MyWidget::MouseEvent & e ){ e.ignore(); }

    virtual void keyDownEvent( MyWidget::KeyEvent & e ) { e.ignore(); }
    virtual void keyUpEvent  ( MyWidget::KeyEvent & e ) { e.ignore(); }

    virtual bool minimapMouseEvent( float /*x*/, float /*y*/,
                                    MyWidget::Event::MouseButton /*btn*/,
                                    MiniMapView::Mode /*m*/ ){return 0; }
  };

struct InputHook : public InputHookBase {
  virtual void mouseDownEvent ( MyWidget::MouseEvent & );
  virtual void mouseUpEvent   ( MyWidget::MouseEvent & );
  virtual void mouseMoveEvent ( MyWidget::MouseEvent & );

  virtual bool minimapMouseEvent(float x, float y,
                                 MyWidget::Event::MouseButton btn,
                                 MiniMapView::Mode m );

  virtual void mouseWheelEvent ( MyWidget::MouseEvent & );

  virtual void keyDownEvent( MyWidget::KeyEvent & );
  virtual void keyUpEvent  ( MyWidget::KeyEvent & );

  MyWidget::signal<MyWidget::MouseEvent &> mouseDown, mouseMove,
                                           mouseUp,   mouseWheel;
  MyWidget::signal<MyWidget::KeyEvent &>   keyDown, keyUp;
  };

#endif // INPUTHOOK_H
