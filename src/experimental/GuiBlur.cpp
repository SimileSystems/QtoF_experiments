#include <assert.h>
#include <stdio.h>
#include <sstream>
#include <experimental/GuiBlur.h>

/* ----------------------------------------------------------------- */

static const char* GUI_BLUR_VS = ""
  "#version 330\n"
  ""
  "const vec2 pos[] = vec2[]("
  " vec2(-1.0, 1.0), "
  " vec2(-1.0, -1.0), "
  " vec2(1.0, 1.0), "
  " vec2(1.0, -1.0)  "
  ");"
  ""
  "const vec2 tex[] = vec2[]("
  "  vec2(0.0, 1.0), "
  "  vec2(0.0, 0.0), "
  "  vec2(1.0, 1.0), "
  "  vec2(1.0, 0.0) "
  " );"
  ""
  "out vec2 v_tex;"
  ""
  "void main() {"
  "  gl_Position = vec4(pos[gl_VertexID], 0.0, 1.0);"
  "  v_tex = tex[gl_VertexID];"
  "}"
  "";

/* ----------------------------------------------------------------- */

GuiBlur::GuiBlur()
  :scene_width(0)
  ,scene_height(0)
  ,blur_width(0)
  ,blur_height(0)
  ,fbo_scene(0)
  ,fbo_blur(0)
  ,vao(0)
  ,tex_vert(0)
  ,tex_hor(0)
  ,tex_scene(0)
{
}

int GuiBlur::init(int sourceWidth, int sourceHeight, int blurWidth, int blurHeight) {

  int r = 0;
  
  if (sourceWidth <= 0) {
    printf("sourceWidth <= 0.\n");
    return -1;
  }
  
  if (sourceHeight <= 0) {
    printf("sourceHeight <= 0.\n");
    return -2;
  }
  
  if (blurWidth <= 0) {
    printf("blurWidth <= 0.\n");
    return -3;
  }
  
  if (blurHeight <= 0) {
    printf("blurHeight <= 0\n.");
    return -4;
  }
  
  scene_width = sourceWidth;
  scene_height = sourceHeight;
  blur_width = blurWidth;
  blur_height = blurHeight;

  if (0 != createFbo()) {
    r = -5;
    goto error;
  }

  if (0 != createShaders()) {
    r = -6;
    goto error;
  }

  glGenVertexArrays(1, &vao);

 error:
  
  if (r < 0) {
    shutdown();
  }
  return r;
}


int GuiBlur::shutdown() {

  int r = 0;
  
  if (0 != fbo_blur) {
    glDeleteFramebuffers(1, &fbo_blur);
    fbo_blur = 0;
  }

  if (0 != tex_vert) {
    glDeleteTextures(1, &tex_vert);
    tex_vert = 0;
  }

  if (0 != tex_hor) {
    glDeleteTextures(1, &tex_hor);
    tex_hor = 0;
  }

  if (true == shader_vert.isLoaded()) {
    shader_vert.unload();
  }

  if (true == shader_hor.isLoaded()) {
    shader_hor.unload();
  }

  if (0 != vao) {
    glDeleteVertexArrays(1, &vao);
    vao = 0;
  }
  
  return r;
}

void GuiBlur::beginCaptureScene() {
  glBindFramebuffer(GL_FRAMEBUFFER, fbo_scene);
  glClear(GL_COLOR_BUFFER_BIT);
  glDrawBuffer(GL_COLOR_ATTACHMENT0);
}

void GuiBlur::endCaptureScene() {
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GuiBlur::blur() {

  /* Copy the scene buffer into our X-pass and Y-pass destinations. */
  glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo_scene);
  glReadBuffer(GL_COLOR_ATTACHMENT0);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo_blur);
  glDrawBuffer(GL_COLOR_ATTACHMENT0);
  glBlitFramebuffer(0, 0, scene_width, scene_height, 0, 0, blur_width, blur_height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
  glDrawBuffer(GL_COLOR_ATTACHMENT1);
  glBlitFramebuffer(0, 0, scene_width, scene_height, 0, 0, blur_width, blur_height, GL_COLOR_BUFFER_BIT, GL_NEAREST);

  glBindVertexArray(vao);
  glBindFramebuffer(GL_FRAMEBUFFER, fbo_blur);
  
  /* Vertical blur. */
  glDrawBuffer(GL_COLOR_ATTACHMENT0);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, tex_scene);

  shader_vert.begin();
  drawBlurBoxes();
  shader_vert.end();

  /* Horizontal blur. */
  glBindFramebuffer(GL_FRAMEBUFFER, fbo_blur);
  glDrawBuffer(GL_COLOR_ATTACHMENT1);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, tex_vert);

  shader_hor.begin();
  drawBlurBoxes();
  shader_hor.end();

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo_blur);
  glReadBuffer(GL_COLOR_ATTACHMENT1);
  glBlitFramebuffer(0, 0, blur_width, blur_height, 0, 0, scene_width, scene_height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
  
#if !defined(NDEBUG)
  static int d = 0;
  if (0 == d) {
    printf("tex_scene: %d, tex_hor: %d, tex_vert: %d\n", tex_scene, tex_hor, tex_vert);
    d = 1;
  }
#endif  
}

void GuiBlur::drawBlurBoxes() {

  if (blur_boxes.size() < 4) {
    return;
  }
  
  glEnable(GL_SCISSOR_TEST);
  
  for (size_t i = 0; i < blur_boxes.size() / 4; ++i) {
    int x = blur_boxes[i * 4 + 0];
    int y = blur_boxes[i * 4 + 1];
    int w = blur_boxes[i * 4 + 2];
    int h = blur_boxes[i * 4 + 3];
    y = scene_height - (y + h);
    glScissor(x, y, w, h);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  }
  
  glDisable(GL_SCISSOR_TEST);
}

/* ----------------------------------------------------------------- */

int GuiBlur::createFbo() {

  /* Create the FBO for the scene target. */
  glGenFramebuffers(1, &fbo_scene);
  glBindFramebuffer(GL_FRAMEBUFFER, fbo_scene);
  createFboTexture(fbo_scene, scene_width, scene_height, GL_COLOR_ATTACHMENT0, tex_scene);

  if (GL_FRAMEBUFFER_COMPLETE != glCheckFramebufferStatus(GL_FRAMEBUFFER)) {
    printf("Error: scene framebuffer not complete.\n");
    return -1;
  }
  
  /* Create the FBO for the blur targets. */
  glGenFramebuffers(1, &fbo_blur);
  glBindFramebuffer(GL_FRAMEBUFFER, fbo_blur);
  createFboTexture(fbo_blur, blur_width, blur_height, GL_COLOR_ATTACHMENT0, tex_vert);
  createFboTexture(fbo_blur, blur_width, blur_height, GL_COLOR_ATTACHMENT1, tex_hor);

  if (GL_FRAMEBUFFER_COMPLETE != glCheckFramebufferStatus(GL_FRAMEBUFFER)) {
    printf("Error: blur framebuffer not complete.\n");
    return -1;
  }
  
  return 0;
}

int GuiBlur::createShaders() {

  /* Get the vertical and horizontal shader sources. */
  std::string vert_source;
  std::string hor_source;
  if (0 != getShaderSources(5, vert_source, hor_source)) {
    return -1;
  }

  /* Create the GL objects. */
  shader_vert.setupShaderFromSource(GL_VERTEX_SHADER, GUI_BLUR_VS);
  shader_vert.setupShaderFromSource(GL_FRAGMENT_SHADER, vert_source);
  if (false == shader_vert.linkProgram()) {
    printf("Error: failed to link the vertical blur shader.\n");
    return -1;
  }

  shader_hor.setupShaderFromSource(GL_VERTEX_SHADER, GUI_BLUR_VS);
  shader_hor.setupShaderFromSource(GL_FRAGMENT_SHADER, hor_source);
  if (false == shader_hor.linkProgram()) {
    printf("Error: failed to link the horizontal blur shader.\n");
    return -2;
  }

  /* And set some defaults. */
  shader_hor.begin();
  shader_hor.setUniform1f("u_step_size", 8.0f);
  shader_hor.setUniform1i("u_tex", 0);
  shader_hor.end();

  shader_vert.begin();
  shader_vert.setUniform1f("u_step_size", 8.0f);
  shader_vert.setUniform1i("u_tex", 0);
  shader_vert.end();
      
  return 0;
}

int GuiBlur::getShaderSources(int steps, std::string& vert, std::string& hor) {

  int sample_offset = steps - 1;
  float inv_w = 1.0f / scene_width;
  float inv_h = 1.0f / scene_height;
  std::stringstream ss_header;
  std::stringstream ss_vert_a;
  std::stringstream ss_vert_b;
  std::stringstream ss_hor_a;
  std::stringstream ss_hor_b;
  std::stringstream ss_footer;
  std::stringstream ss_vert;
  std::stringstream ss_hor;

  ss_header << "#version 330\n"
            << "uniform sampler2D u_tex;\n"
            << "uniform float u_step_size;\n"
            << "layout (location = 0) out vec4 fragcolor;\n"
            << "in vec2 v_tex;\n"
            << "\n"
            << "void main() {\n"
            << "  vec4 color = " << steps << ".0 * texture(u_tex, v_tex);\n"
            << "";

  for (int i = 1; i < steps; ++i) {
    ss_vert_a << "  color += " << i << ".0 * texture(u_tex, vec2(v_tex.x, v_tex.y + u_step_size * -" << (sample_offset * inv_h) << "));\n";
    ss_vert_b << "  color += " << i << ".0 * texture(u_tex, vec2(v_tex.x, v_tex.y + u_step_size * " << (sample_offset * inv_h) << "));\n";
    ss_hor_a  << "  color += " << i << ".0 * texture(u_tex, vec2(v_tex.x + u_step_size * -" << (sample_offset * inv_w) << ", v_tex.y));\n";
    ss_hor_b  << "  color += " << i << ".0 * texture(u_tex, vec2(v_tex.x + u_step_size * " << (sample_offset * inv_w) << ", v_tex.y));\n";
    sample_offset--;
  }

  ss_footer  << "  color /= " << (steps * steps) << ".0;\n"
             << "  fragcolor = color;\n"
             << "  fragcolor.a = 1.0;"
             << "}";

  ss_vert << ss_header.str() << ss_vert_a.str() << ss_vert_b.str() << ss_footer.str();
  ss_hor << ss_header.str() << ss_hor_a.str() << ss_hor_b.str() << ss_footer.str();

  vert = ss_vert.str();
  hor = ss_hor.str();
  
  return 0;
}

int GuiBlur::createFboTexture(GLuint fbo, int w, int h, GLenum attachment, GLuint& dest) {
  
  if (w <= 0) {
    printf("Error, given texture width <= 0.\n");
    return -1;
  }

  if (h <= 0) {
    printf("Error, given texture height <= 0.\n");
    return -2;
  }

  glGenTextures(1, &dest);
  glBindTexture(GL_TEXTURE_2D, dest);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, dest, 0);

  return 0;
}

/* ----------------------------------------------------------------- */

