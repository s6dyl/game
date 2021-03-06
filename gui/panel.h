#ifndef PANEL_H
#define PANEL_H

#include <Tempest/Widget>
#include <Tempest/Sprite>

class Resource;

class Panel : public Tempest::Widget {
  public:
    Panel(Resource &res );

    Tempest::Sprite frame, back[2];

    void setDragable( bool d );
    bool isDragable();

  protected:
    void mouseDownEvent(Tempest::MouseEvent &e);
    void mouseDragEvent(Tempest::MouseEvent &e);
    void mouseMoveEvent(Tempest::MouseEvent &e);
    void mouseUpEvent(Tempest::MouseEvent &e);

    void mouseWheelEvent(Tempest::MouseEvent &e);
    void gestureEvent(Tempest::AbstractGestureEvent &e);

    void paintEvent( Tempest::PaintEvent &p);
    virtual void paintFrame(Tempest::Painter &p );

  private:
    bool mouseTracking, dragable;
    Tempest::Point mpos, oldPos;
  };

#endif // PANEL_H
