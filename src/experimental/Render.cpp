#include <stdio.h>
#include <stdlib.h>
#include <Render.h>
#include <Events.h>
#include <ofAppRunner.h>

namespace dk {

  /* ------------------------------------------------------------------------*/
  /* Embeddable OpenGL wrappers.                                             */
  /* ------------------------------------------------------------------------*/
  static int create_shader(GLuint* out, GLenum type, const char* src);           /* create a shader, returns 0 on success, < 0 on error. e.g. create_shader(&vert, GL_VERTEX_SHADER, DRAWER_VS); */
  static int create_program(GLuint* out, GLuint vert, GLuint frag, int link);    /* create a program from the given vertex and fragment shader. returns 0 on success, < 0 on error. e.g. create_program(&prog, vert, frag, 1); */
  static int print_shader_compile_info(GLuint shader);                           /* prints the compile info of a shader. returns 0 when shader compiled, < 0 on error. */
  static int print_program_link_info(GLuint prog);                               /* prints the program link info. returns 0 on success, < 0 on error. */
  /* ------------------------------------------------------------------------*/

  static const char* TEST_VS = ""
    ""
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
  
  static const char* TEST_FS = ""
    "#version 330\n"
    ""
    "layout (location = 0) out vec4 fragcolor;"
    "in vec2 v_tex;"
    ""
    "void main() {"
    "   fragcolor = vec4(v_tex.s, v_tex.t, 0.0, 0.0);"
    "}"
    "";

  /* ------------------------------------------------------------------------*/

  Render::Render()
    :prog(0)
    ,vert(0)
    ,frag(0)
    ,vao(0)
    ,win(nullptr)
  {
    printf("Render::Render()\n");
  }

  int Render::init(int winWidth, int winHeight, float pixRatio) {
    printf("Render::init()\n");

    if (0 != prog) {
      printf("Render already initialized.\n");
      return -1;
    }

    if (!gladLoadGL()) {
      printf("Failed to load GL bindings.\n");
      return -2;
    }

    if (NULL == glCreateShader) {
      printf("Error: failed to load GL bindings; did you create a GL context?\n");
      return -3;
    }
    
    if (0 != create_shader(&vert, GL_VERTEX_SHADER, TEST_VS)) {
      printf("Failed to create test vertex shader.\n");
      return -2;
    }

    if (0 != create_shader(&frag, GL_FRAGMENT_SHADER, TEST_FS)) {
      printf("Failed to create test fragment shader.\n");
      return -3;
    }

    if (0 != create_program(&prog, vert, frag, true)) {
      printf("Failed to create program.\n");
      return -4;
    }

    glGenVertexArrays(1, &vao);

    /* TMP BEGIN: SETUP OPENFRAMEWORKS */
    if (winWidth <= 0) {
      printf("Render::init() - error: winWidth <= 0.\n");
      return -1;
    }
    
    if (winHeight <= 0) {
      printf("Render::init() - error: winHeight <= 0.\n");
      return -2;
    }

    if (pixRatio <= 0.0f) {
      printf("Render::init() - error: pixRatio <= 0.0f.\n");
      return -3;
    }
    
    printf("glCreateProgram: %p in Render.cpp.\n", glCreateProgram);
    win = std::shared_ptr<ofAppQtWindow>(new ofAppQtWindow());
    ofGetMainLoop()->addWindow(win);

    ofWindowSettings win_settings;
    win_settings.width = winWidth;
    win_settings.height = winHeight;
    win->setup(win_settings);

    win->setPixelRatio(pixRatio);

    /* TMP END: SETUP OPENFRAMEWORKS */

    img.load(ofFilePath::getCurrentExeDir() +"640x480@2x.png");
    printf("Loaded an image: %f x %f\n", img.getWidth(), img.getHeight());

    video_grabber.listDevices();
    video_grabber.setup(0, 640, 480);
    
   
    return 0;
  }

  int Render::shutdown() {
    return 0;
  }

  int Render::update() {
    win->update();
    return 0;
  }

  int Render::draw() {

    if (0 == prog) {
      printf("prog is 0, not initialized?\n");
      return -1;
    }

    glBindVertexArray(vao);
    glUseProgram(prog);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glBindVertexArray(0);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);

    win->startRender();
    
    img.draw(0, 0, 640 * win->getPixelRatio(), 480 * win->getPixelRatio());
    video_grabber.update();
    video_grabber.draw(0, 0, 320 * win->getPixelRatio(), 240 * win->getPixelRatio());

    win->finishRender();
    
    return 0;
  }

  int Render::onEvent(unsigned int event, void* data) {

    switch (event) {
      case EVENT_WINDOW_RESIZED: {
        if (nullptr == data) {
          printf("Error: resize data is invalid (exiting).\n");
          exit(EXIT_FAILURE);
        }
        int* s = (int*)data;
        win->setWindowShape(s[0], s[1]);
        printf("Resized: %d x %d\n", s[0], s[1]);
        break;
      }
      case EVENT_PIXEL_RATIO_CHANGED: {
        if (nullptr == data) {
          printf("Error: pixel ratio is null (exiting). \n");
          exit(EXIT_FAILURE);
        }
        win->setPixelRatio(*(float*)data);
        printf("Pixel ratio: %f\n", *(float*)data);
        break;
      }
      case EVENT_MOUSE_PRESS: {
        if (nullptr == data) {
          printf("Error: mouse press data null.\n");
          exit(EXIT_FAILURE);
        }
        int* mp = (int*)data;
        printf("Mouse press: %d %d\n", mp[0], mp[1]);
        break;
      }
      case EVENT_MOUSE_RELEASE: {
        if (nullptr == data) {
          printf("Error: mouse release data null.\n");
          exit(EXIT_FAILURE);
        }
        int* mp = (int*)data;
        printf("Mouse release: %d %d\n", mp[0], mp[1]);
        break;
      }
      case EVENT_MOUSE_MOVE: {
        if (nullptr == data) {
          printf("Error: mouse move data is null.\n");
          exit(EXIT_FAILURE);
        }
        int* mp = (int*)data;
        printf("Mouse move: %d x %d\n", mp[0], mp[1]);
        //        glViewport(0, 0, 640,480);
        break;
      }
      default: {
        printf("Warning: unhandled event.\n");
        break;
      }
    };

    return 0;
  }

  /* ------------------------------------------------------------------------*/
  /* Embeddable OpenGL wrappers.                                             */
  /* ------------------------------------------------------------------------*/

  static int create_shader(GLuint* out, GLenum type, const char* src) {

    *out = glCreateShader(type);
    glShaderSource(*out, 1, &src, NULL);
    glCompileShader(*out);

    if (0 != print_shader_compile_info(*out)) {
      *out = 0;
      return -1;
    }

    return 0;
  }

  /* create a program, store the result in *out. when link == 1 we link too. returns -1 on error, otherwise 0 */
  static int create_program(GLuint* out, GLuint vert, GLuint frag, int link) {
    *out = glCreateProgram();
    glAttachShader(*out, vert);
    glAttachShader(*out, frag);

    if(1 == link) {
      glLinkProgram(*out);
      if (0 != print_program_link_info(*out)) {
        *out = 0;
        return -1;
      }
    }

    return 0;
  }

  /* checks + prints program link info. returns 0 when linking didn't result in an error, on link erorr < 0 */
  static int print_program_link_info(GLuint prog) {
    GLint status = 0;
    GLint count = 0;
    GLchar* error = NULL;
    GLsizei nchars = 0;

    glGetProgramiv(prog, GL_LINK_STATUS, &status);
    if(status) {
      return 0;
    }

    glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &count);
    if(count <= 0) {
      return 0;
    }

    error = (GLchar*)malloc(count);
    glGetProgramInfoLog(prog, count, &nchars, error);
    if (nchars <= 0) {
      free(error);
      error = NULL;
      return -1;
    }

    printf("\nPROGRAM LINK ERROR");
    printf("\n--------------------------------------------------------\n");
    printf("%s", error);
    printf("--------------------------------------------------------\n\n");

    free(error);
    error = NULL;
    return -1;
  }

  /* checks the compile info, if it didn't compile we return < 0, otherwise 0 */
  static int print_shader_compile_info(GLuint shader) {

    GLint status = 0;
    GLint count = 0;
    GLchar* error = NULL;

    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if(status) {
      return 0;
    }

    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &count);
    if (0 == count) {
      return 0;
    }

    error = (GLchar*) malloc(count);
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &count);
    if(count <= 0) {
      free(error);
      error = NULL;
      return 0;
    }

    glGetShaderInfoLog(shader, count, NULL, error);
    printf("\nSHADER COMPILE ERROR");
    printf("\n--------------------------------------------------------\n");
    printf("%s", error);
    printf("--------------------------------------------------------\n\n");

    free(error);
    error = NULL;
    return -1;
  }

  /* ------------------------------------------------------------------------*/
  
} /* namespace dk */
