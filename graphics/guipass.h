#ifndef GUIPASS_H
#define GUIPASS_H

#include <MyGL/AbstractRenderPass>
#include <MyGL/Algo/GBufferFillPass>

#include <MyGL/VertexShaderHolder>
#include <MyGL/FragmentShaderHolder>

#include <MyGL/Size>
#include <MyWidget/Painter>

#include "pixmapspool.h"

#include <cstdint>

namespace MyGL{
  class PostProcessHelper;
  }

class MainGui;
class Resource;

class GUIPass : public MyGL::AbstractRenderPass {
  public:
    GUIPass( MyGL::RenderTaget & rt,
             MyGL::Texture2d   & depth,
             Resource &res,
             MainGui &gui,
             MyGL::VertexShaderHolder & vsh,
             MyGL::FragmentShaderHolder & fsh,
             MyGL::VertexBufferHolder &vbo,
             MyGL::Size &s );

    void exec( const MyGL::Scene &s, MyGL::Device &d );

    MyGL::RenderTaget & rt;

    void rect( int x0, int y0, int x1, int y1, int texDx, int texDy, int tw, int th );

    void setTexture( const PixmapsPool::TexturePtr &t );
    void unsetTexture();

    void clearBuffers();
    void setBlendMode( MyWidget::BlendMode m );
  private:
    struct Vertex{
      float x,y;
      float u,v;
      };
    MyWidget::Rect texRect;

    struct GeometryBlock{
      size_t begin, size;
      MyGL::RenderState state;
      PixmapsPool::TexturePtr texture;
      };

    std::vector<GeometryBlock> geometryBlocks;

    std::vector<Vertex> guiRawData;
    MyGL::VertexBuffer<Vertex> guiGeometry;
    MyGL::VertexDeclaration    vdecl;

    MyGL::VertexShader   vs;
    MyGL::FragmentShader fs;
    MyGL::VertexBufferHolder & vbHolder;

    MyGL::Texture2d noTexture, testTex;
    MyGL::Texture2d   & depth;

    MyGL::Size         & size;
    MainGui            & gui;

    float dTexCoord[2];

    MyGL::Device *dev;

    static MyGL::RenderState makeRS( MyWidget::BlendMode m );
  };

#endif // GUIPASS_H
