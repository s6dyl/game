#include "wayfindalgo.h"

#include "landscape/terrain.h"
#include "game/gameobject.h"
#include "behavior/movebehavior.h"
#include "algo/algo.h"

#include <MyWidget/signal>

#include <fstream>

array2d<int> WayFindAlgo::clasterMap, WayFindAlgo::wayMap;

WayFindAlgo::WayFindAlgo( const Terrain &t ) : terrain(t){
  clasterMap.resize( t.width() -1,
                     t.height()-1 );
  wayMap.resize( t.width()-1, t.height()-1 );

  std::fill( clasterMap.begin(), clasterMap.end(), 0 );
  }

void WayFindAlgo::fillClasrerMap( const std::vector< GameObject* > &objects ) {
  std::fill( clasterMap.begin(), clasterMap.end(), 0 );

  for( size_t i=0; i<objects.size(); ++i ){
    GameObject & obj = *objects[i];

    int x = obj.x()/Terrain::quadSize,
        y = obj.y()/Terrain::quadSize,
        sz = obj.getClass().data.size,
        szd2 = sz/2;

    for( int ix=0; ix<sz; ++ix )
      for( int iy=0; iy<sz; ++iy )
        if( clasterMap.validate( x-szd2+ix, y-szd2+iy ) )
          clasterMap[x-szd2+ix][y-szd2+iy] = -1;
    }

  clasterNum = 1;
  for( size_t i=0; i<objects.size(); ++i ){
    GameObject & obj = *objects[i];

    int x = obj.x()/Terrain::quadSize,
        y = obj.y()/Terrain::quadSize;

    if( clasterMap.validate(x,y) && clasterMap[x][y]== -1 ){
      fill( x, y, clasterNum );
      ++clasterNum;
      }
    }

  }

void WayFindAlgo::findWay( std::vector< GameObject* >& objs,
                           int rx, int ry ) {
  rx /= Terrain::quadSize;
  ry /= Terrain::quadSize;

  std::vector< std::shared_ptr< std::vector<Point> > > ways( objs.size() );

  for( size_t i=0; i<objs.size(); ++i ){
    GameObject & obj = *objs[i];
    MoveBehavior *bobj = obj.behavior.find<MoveBehavior>();

    int x = obj.x()/Terrain::quadSize,
        y = obj.y()/Terrain::quadSize;

    if( !ways[i] ){
      findWay( obj, x, y, rx, ry );
      if( bobj ){
        bobj->setWay( way );
        }
      ways[i] = std::make_shared<std::vector<Point>>( way );
      }

    for( size_t r=i+1; r<objs.size(); ++r ){
      if( !ways[r] ){
        GameObject & tg = *objs[r];
        int d = tg.distanceSQ(obj);

        int maxD = (( tg.getClass().data.size +
                      obj.getClass().data.size )*Terrain::quadSize)/2;
        maxD = maxD*maxD;

        if( d<=2*maxD ){
          if( MoveBehavior *btg = tg.behavior.find<MoveBehavior>() ){
            btg->setWay( *ways[i] );
            }

          ways[r] = ways[i];
          }
        }
      }
    }
  }

void WayFindAlgo::dump() {
  std::ofstream fout("./debug.txt");

  for( int i=0; i<wayMap.width(); ++i ){
    for( int r=0; r<wayMap.height(); ++r ){
      char x = ' ';
      bool v = terrain.isEnable(i,r);

      if( wayMap.at(i,r)<10 )
        x = '0' + wayMap.at(i,r);

      if( !v )
        x = 'x';

      for( size_t q=0; q<way.size(); ++q )
        if( way[q].x==i && way[q].y==r ){
          if( terrain.isEnableQuad(i-1,r, 3) )
            x = '*'; else
            x = '@';
          }

      fout << x;
      }

    fout << std::endl;
    }

  fout.close();
  }

void WayFindAlgo::fill( int x, int y, int val ) {
  wave( clasterMap, x, y, val,
        negTest<int>,
        copyMapPoint<int>,
        falseFunc,
        16 );
  }

void WayFindAlgo::findWay(std::vector<GameObject*> &objs,
                          GameObject & obj,
                          int x,
                          int y,
                          int rx,
                          int ry,
                          int cls) {
  if( !wayMap.validate(rx,ry) )
    return;

  findWay( obj, x, y, rx, ry );

  for( size_t i=0; i<objs.size(); ++i ){
    GameObject & obj = *objs[i];
    MoveBehavior * b = obj.behavior.find<MoveBehavior>();


    int x = obj.x()/Terrain::quadSize,
        y = obj.y()/Terrain::quadSize;

    if( b && clasterMap.validate(x,y) && clasterMap[x][y]==cls ){
      b->setWay( way );
      }
    }
  }

void WayFindAlgo::findWay(GameObject & obj, int x, int y, int rx, int ry) {
  auto fix = terrain.nearestEnable( rx, ry, 1 );
  rx = fix.first;
  ry = fix.second;

  if( !wayMap.validate(rx,ry) )
    return;

  fix = terrain.nearestEnable( x, y, 1 );
  x = fix.first;
  y = fix.second;

  rPointX = rx;
  rPointY = ry;

  std::fill( wayMap.begin(), wayMap.end(), -1 );

  MemberFunc< WayFindAlgo, bool,
              const array2d<int> &, Point, Point>
      func(*this, &WayFindAlgo::isQuadEnable);

  MemberFunc< WayFindAlgo, bool >
      isEnd(*this, &WayFindAlgo::isRPoint );

  int dimCount = 4;
  wave( wayMap, x, y, 1, func, incMapPoint<int>, isEnd, dimCount );
  //dump();

  if( wayMap.at(rx,ry)==-1 )
    return;

  Point re = {rx, ry};
  Point* dxy = dXY();

  way.clear();
  while( (re.x!=x || re.y!=y) ){
    bool found = 0;

    for( int i=0; i<dimCount; ++i ){
      Point p = {re.x+dxy[i].x, re.y+dxy[i].y };

      if( wayMap.validate(p.x, p.y) &&
          wayMap[p.x][p.y]+1 == wayMap[re.x][re.y] ){
        way.push_back(re);
        re = p;
        found = true;
        //break;
        }
      }

    assert( found );
    }

  way.push_back(re);
  //dump();
  optimizeWay();
  //dump();
  }

bool WayFindAlgo::isQuadEnable( const array2d<int> &  map,
                                Point p,
                                Point /*src*/ ) {
  return terrain.isEnable( p.x, p.y ) && map[p.x][p.y]<0;
  }

void WayFindAlgo::optimizeWay() {
  //return;

  if( way.size()==0 )
    return;

  int a = 1, b = 2, wr = 1;

  for( ; b < int(way.size());  ){
    if( !optimizeWay( way[a], way[b] ) || b-a>20 ){
      way[wr] = way[b];
      ++wr;
      //++a;
      a = b;
      ++b;
      } else {
      ++b;
      }
    }

  Point l = way.back();
  way.resize(wr+1);
  way[wr] = l;
  }

bool WayFindAlgo::optimizeWay(Point a, Point b) {
  Point p = a;

  while( p.x!=b.x || p.y!=b.y ){
    //if( !terrain.isEnableQuad(p.x, p.y, 2) )
      //return false;

    if( abs(p.x-b.x) > abs(p.y-b.y) ){
      if( p.x>b.x )
        --p.x; else
        ++p.x;
      } else {
      if( p.y>b.y )
        --p.y; else
        ++p.y;
      }

    }

  return terrain.isEnableQuad(b.x, b.y, 3);
  }

bool WayFindAlgo::isRPoint() {
  int v = wayMap.at( rPointX, rPointY );
  return v != -1;
  }
