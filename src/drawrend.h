#include <utility>

#ifndef CGL_DRAWREND_H
#define CGL_DRAWREND_H

#include "CGL/CGL.h"
#include "CGL/renderer.h"
#include "CGL/color.h"
#include <vector>
#include <cstring>
#include "GLFW/glfw3.h"
#include "svg.h"
#include "particle.h"

extern float start;

namespace CGL {

class DrawRend : public Renderer {
 public:
  DrawRend(int audio_rate, std::vector<uint8_t> magnitude, std::vector<uint8_t> onsets, std::vector<uint8_t> beats):
      audio_rate(audio_rate), magnitude(std::move(magnitude)), onsets(std::move(onsets)), beats(std::move(beats)), grid(50, 1.0/audio_rate){
  }

  ~DrawRend() override;

  // inherited Renderer interface functions
  void init() override;
  void render() override;
  void resize( size_t w, size_t h ) override;
  std::string name() override { return "Draw"; }
  std::string info() override;
  void cursor_event( float x, float y ) override;
  void scroll_event( float offset_x, float offset_y ) override;
  void mouse_event( int key, int event, unsigned char mods ) override;
  void keyboard_event( int key, int event, unsigned char mods ) override;

  void set_gl(bool gl_) { gl = gl_; }

  // write current pixel buffer to disk
  void write_screenshot();

  // write only framebuffer to disk
  void write_framebuffer();

  // drawing functions
  void redraw();
  void draw_pixels();
  void draw_zoom();

  void particles_init();

  // view transform functions
  void view_init();
  void set_view(float x, float y, float span);
  void move_view(float dx, float dy, float scale);

  // rasterize a point
  void rasterize_point( float x, float y, Color color );

  // rasterize a line
  void rasterize_line( float x0, float y0,
                       float x1, float y1,
                       Color color);

  // rasterize a square
  void rasterize_square(float x, float y,
                        float size, Color color);
    
  // rasterize a circle
  void rasterize_circle(float x, float y,
                   float size, Color color);
    
    // rasterize a circle
    void rasterize_circle2(float x, float y,
                           float size, Color color);
    
  // circle_draw helper method
    void circle_draw_helper(int xCoordinate, int yCoordinate,
                            int tempX, int tempY,
                            Color color);

  // rasterize a triangle
  void rasterize_triangle( float x0, float y0,
                           float x1, float y1,
                           float x2, float y2,
                           Color color, Triangle *tri = nullptr );

  void rasterize_particle(const Particle &particle);


private:
  // Global state variables for SVGs, pixels, and view transforms
//  std::vector<SVG*> svgs; size_t current_svg;
  ParticleGrid grid;

  Matrix3x3 ndc_to_screen;

  std::vector<unsigned char> framebuffer;
  size_t width, height;

  // UI state info
  float cursor_x; float cursor_y;
  bool left_clicked;
  int show_zoom;
  int sample_rate;

  // Audio
  int audio_rate;
  std::vector<uint8_t> magnitude;
  std::vector<uint8_t> onsets;
  std::vector<uint8_t> beats;

  PixelSampleMethod psm;
  LevelSampleMethod lsm;

  bool gl;

  // Intuitively, a sample buffer instance is a pixel,
  // or (samples_per_side x samples_per_side) sub-pixels.
  struct SampleBuffer {
    std::vector<unsigned char> sub_pixel;

    SampleBuffer(size_t sps) {
      sub_pixel = std::vector<unsigned char>(3, 255);
    }

    void fill_pixel(Color c) {
      for (int k = 0; k < 3; k++) sub_pixel[k] = (unsigned char) (c[k] * 255);
    }

    Color get_pixel_color() {
      return Color(sub_pixel.data());
    }
    
    void clear() {
      sub_pixel.assign(3, (unsigned char)255);
    }
  };

  std::vector<std::vector<SampleBuffer>> samplebuffer;

  // This function takes the collected sub-pixel samples and
  // combines them together to fill in the framebuffer in preparation
  // for posting pixels to the screen.
  void resolve() {
    for (int x = 0; x < width; ++x) {
      for (int y = 0; y < height; ++y) {
        for (int k = 0; k < 3; ++k) {
          framebuffer[3 * (y * width + x) + k] = (&(samplebuffer[y][x].sub_pixel[0]))[k];
        }
      }
    }
  }


};

} // namespace CGL

#endif // CGL_DRAWREND_H
