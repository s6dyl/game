#include "unitview.h"

#include "resource.h"
#include "game/gameobject.h"
#include "game.h"

UnitView::UnitView( Resource &res )
  : TextureView(res), res(res) {
  //texture = res.texHolder.create(120, 200);

  scene.lights().direction().resize(1);
  MyGL::DirectionLight light;
  light.setDirection( -2, 1, -2.0 );
  light.setColor    ( MyGL::Color( 0.7, 0.7, 0.7 ) );
  light.setAblimient( MyGL::Color( 0.33, 0.33,  0.35) );
  scene.lights().direction()[0] = light;
  }

UnitView::~UnitView() {
  }

void UnitView::setupUnit(GameObject *obj) {
  view.reset(0);

  if( obj ){
    const ProtoObject & p = obj->game().prototype( obj->getClass().name );

    view.reset( new GameObjectView( scene,
                                    obj->world(),
                                    p,
                                    obj->prototypes ) );
    view->loadView( obj->game().resources(), obj->world().physics, 0 );
    view->setSelectionVisible(0);
    view->teamColor = MyGL::Color(1, 1, 0, 1);
    setupCamera();
    }
  }

void UnitView::paintEvent(MyWidget::PaintEvent &e) {
  if( texture.width()!=w() || texture.height()!=h() ){
    texture = res.texHolder.create( w(), h() );
    setupCamera();
    }

  renderScene( scene, texture );
  TextureView::paintEvent(e);
  }

void UnitView::setupCamera() {
  MyGL::Camera camera;
  camera.setPerespective( true, w(), h() );
  camera.setPosition( 0, 0, 0 );
  camera.setDistance( 2 );

  bool mv = 0;
  if( view ){
    for( size_t i=0; i<view->getClass().behaviors.size(); ++i )
      if( view->getClass().behaviors[i]=="move" )
        mv = 1;

    camera.setPosition( 0, 0, view->viewHeight()*0.5 );
    }

  if( mv )
    camera.setSpinX(-15-180-90); else
    camera.setSpinX(-15-180);

  camera.setSpinY(-130);

  if( view )
    camera.setZoom( 0.8/view->radius() );

  scene.setCamera( camera );
  }