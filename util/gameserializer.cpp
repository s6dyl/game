#include "gameserializer.h"

const unsigned GameSerializer::currentVersion = 3;

GameSerializer::GameSerializer( const std::string &s, OpenMode m )
               :FileSerialize(s,m){
  cversion = currentVersion;

  *this + cversion;
  }

unsigned GameSerializer::version() const {
  return cversion;
  }

