#include "missiontargets.h"

#include <MyWidget/Painter>
#include "maingui.h"

#include "richtext.h"
#include "lang/lang.h"

#include "game.h"

MissionTargets::MissionTargets( Game & game, Resource &res )
               :game(game), res(res) {
  frame.data = res.pixmap("gui/hintFrame");

  ckFrame.data = res.pixmap("gui/ckBoxFrame");
  ck.data      = res.pixmap("gui/ckBox");

  game.updateMissionTargets.bind(*this, &MissionTargets::setupTagets);

  setupTagets();
  }

void MissionTargets::paintEvent(MyWidget::PaintEvent &e) {
  MyWidget::Painter p(e);

  p.setBlendMode( MyWidget::alphaBlend );
  MainGui::drawFrame(p, frame, box->pos(), box->size() );

  for( size_t i=0; i<box->layout().widgets().size(); ++i ){
    MyWidget::Widget *wx = box->layout().widgets()[i];
    int sz = wx->h();
    p.setTexture( ckFrame );
    p.drawRect( 7, wx->pos().y, sz, sz,
                 0, 0, ckFrame.data.rect.w, ckFrame.data.rect.h );

    if( game.scenario().tagets()[i].done ){
      p.setTexture( ck );
      p.drawRect( 7, wx->pos().y, sz, sz,
                   0, 0, ck.data.rect.w, ck.data.rect.h );
      }
    }

  paintNested(e);
  }

void MissionTargets::setupTagets() {
  layout().removeAll();

  int w = 180, h = 20;

  setLayout( MyWidget::Vertical );

  box = new MyWidget::Widget();
  box->setLayout( MyWidget::Vertical );

  for( size_t i=0; i<game.scenario().tagets().size(); ++i ){
    RichText *t = new RichText(res);
    std::wstring str = Lang::tr( game.scenario().tagets()[i].hint );
    t->setText( str );

    MyWidget::Size sz = RichText::bounds(res, str);
    t->setMaximumSize( sz );

    w = std::max(w, sz.w);
    h+= sz.h;

    box->layout().add( t );
    }

  box->setMaximumSize( w+40, h );
  box->layout().setMargin( 20, 10, 10, 10 );

  box->setSizePolicy( MyWidget::FixedMax );
  setMaximumSize( box->sizePolicy().maxSize.w, sizePolicy().maxSize.h );

  setVisible( box->layout().widgets().size() );

  layout().add( box );
  layout().add( new Widget() );
  }