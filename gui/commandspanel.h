#ifndef COMMANDSPANEL_H
#define COMMANDSPANEL_H

#include "gui/panel.h"

#include <Tempest/Layout>

#include "game/protoobject.h"
#include "gui/inputhook.h"

class GameObject;
class BehaviorMSGQueue;

class CommandsPanel : public Panel {
  public:
    CommandsPanel(Resource &res, BehaviorMSGQueue & q );

    void bind(GameObject *u );

    class Layout: public Tempest::Layout{
      void applyLayout();
      };

    Tempest::signal<> onPageCanged;
  private:
    Resource & res;
    BehaviorMSGQueue & msg;

    InputHook hook;
    bool  instaled;
    std::string spellToCast;

    class BtnBase;
    class BuyBtn;
    class PageBtn;
    class SpellBtn;

    int pl;
    GameObject * u0;

    enum Mode{
      CastToCoord,
      CastToUnit
      } mode;

    void bind( const ProtoObject::Commans::Page &u );
    void buyEvent( const std::string& unit );
    void gradeEvent( const std::string& unit );
    void moveClick();
    void stopClick();
    void holdClick();
    void patrulClick();
    void atkClick();

    void setPage( int p );

    void bindStartPage(const ProtoObject::Commans::Page *p );
    void bindPage( const ProtoObject::Commans::Page &p );

    void setupHook ( const std::string& unit );
    void setupHookU( const std::string& unit );

    void mouseDown( Tempest::MouseEvent& e );
    void mouseUp  ( Tempest::MouseEvent& e );
    void onRemoveHook();
  };

#endif // COMMANDSPANEL_H
