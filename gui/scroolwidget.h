#ifndef SCROOLWIDGET_H
#define SCROOLWIDGET_H

#include <MyWidget/Widget>
#include <MyWidget/Layout>
#include "gui/panel.h"
#include "gui/scroolbar.h"

class ScroolWidget : public MyWidget::Widget {
  public:
    ScroolWidget( Resource & res );

    Widget& centralWidget();
  private:
    ScroolBar sb;
    Widget box, *cen;

    struct ProxyLayout: public MyWidget::LinearLayout{
      void applyLayout();

      ScroolBar *scrool;
      MyWidget::Size sizeHint( const Widget *w );
      };
    ProxyLayout *lay;

    void scrool( int v );
    void resizeEv(int w, int h);

    using MyWidget::Widget::layout;
  };

#endif // SCROOLWIDGET_H
