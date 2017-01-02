set(of_bd ${OF_DIR})
set(of_ld ${of_bd}/libs)
set(of_sd ${of_bd}/libs/openFrameworks)

list(APPEND of_sources
  ${of_sd}/3d/of3dPrimitives.cpp
  ${of_sd}/3d/of3dUtils.cpp
  ${of_sd}/3d/ofCamera.cpp
  ${of_sd}/3d/ofEasyCam.cpp
  ${of_sd}/3d/ofNode.cpp
  ${of_sd}/app/ofAppGLFWWindow.cpp
  ${of_sd}/app/ofAppRunner.cpp
  ${of_sd}/app/ofMainLoop.cpp
  ${of_sd}/communication/ofArduino.cpp
  ${of_sd}/communication/ofSerial.cpp
  ${of_sd}/events/ofEvents.cpp
  ${of_sd}/gl/ofBufferObject.cpp
  ${of_sd}/gl/ofFbo.cpp
  ${of_sd}/gl/ofGLProgrammableRenderer.cpp
  ${of_sd}/gl/ofGLRenderer.cpp
  ${of_sd}/gl/ofGLUtils.cpp
  ${of_sd}/gl/ofLight.cpp
  ${of_sd}/gl/ofMaterial.cpp
  ${of_sd}/gl/ofShader.cpp
  ${of_sd}/gl/ofTexture.cpp
  ${of_sd}/gl/ofVbo.cpp
  ${of_sd}/gl/ofVboMesh.cpp
  ${of_sd}/graphics/of3dGraphics.cpp
  ${of_sd}/graphics/ofBitmapFont.cpp
  ${of_sd}/graphics/ofCairoRenderer.cpp
  ${of_sd}/graphics/ofGraphics.cpp
  ${of_sd}/graphics/ofImage.cpp
  ${of_sd}/graphics/ofPath.cpp
  ${of_sd}/graphics/ofPixels.cpp
  ${of_sd}/graphics/ofRendererCollection.cpp
  ${of_sd}/graphics/ofTessellator.cpp
  ${of_sd}/graphics/ofTrueTypeFont.cpp
  ${of_sd}/math/ofMath.cpp
  ${of_sd}/math/ofMatrix3x3.cpp
  ${of_sd}/math/ofMatrix4x4.cpp
  ${of_sd}/math/ofQuaternion.cpp
  ${of_sd}/math/ofVec2f.cpp
  ${of_sd}/math/ofVec4f.cpp
  ${of_sd}/sound/ofBaseSoundStream.cpp
  ${of_sd}/sound/ofFmodSoundPlayer.cpp
  ${of_sd}/sound/ofOpenALSoundPlayer.cpp
  ${of_sd}/sound/ofRtAudioSoundStream.cpp
  ${of_sd}/sound/ofSoundBuffer.cpp
  ${of_sd}/sound/ofSoundPlayer.cpp
  ${of_sd}/sound/ofSoundStream.cpp
  ${of_sd}/types/ofBaseTypes.cpp
  ${of_sd}/types/ofColor.cpp
  ${of_sd}/types/ofParameter.cpp
  ${of_sd}/types/ofParameterGroup.cpp
  ${of_sd}/types/ofRectangle.cpp
  ${of_sd}/utils/ofFileUtils.cpp
  ${of_sd}/utils/ofFpsCounter.cpp
  ${of_sd}/utils/ofLog.cpp
  ${of_sd}/utils/ofMatrixStack.cpp
  ${of_sd}/utils/ofSystemUtils.cpp
  ${of_sd}/utils/ofThread.cpp
  ${of_sd}/utils/ofTimer.cpp
  ${of_sd}/utils/ofURLFileLoader.cpp
  ${of_sd}/utils/ofUtils.cpp
  ${of_sd}/utils/ofXml.cpp
  ${of_sd}/video/ofAVFoundationGrabber.mm
  ${of_sd}/video/ofQtUtils.cpp
  ${of_sd}/video/ofQuickTimeGrabber.cpp
  ${of_sd}/video/ofQuickTimePlayer.cpp
  ${of_sd}/video/ofVideoGrabber.cpp
  ${of_sd}/video/ofVideoPlayer.cpp
  )

set_source_files_properties(${of_sd}/app/ofAppGLFWWindow.cpp PROPERTIES COMPILE_FLAGS "-x objective-c++")
set_source_files_properties(${of_sd}/utils/ofSystemUtils.cpp PROPERTIES COMPILE_FLAGS "-x objective-c++")

include_directories(
  ${of_ld}/openFrameworks
  ${of_ld}/openFrameworks/3d
  ${of_ld}/openFrameworks/app
  ${of_ld}/openFrameworks/communication
  ${of_ld}/openFrameworks/events
  ${of_ld}/openFrameworks/gl
  ${of_ld}/openFrameworks/graphics
  ${of_ld}/openFrameworks/math
  ${of_ld}/openFrameworks/ofMain.h
  ${of_ld}/openFrameworks/sound
  ${of_ld}/openFrameworks/types
  ${of_ld}/openFrameworks/utils
  ${of_ld}/openFrameworks/video
  ${of_ld}/FreeImage/include
  ${of_ld}/boost/include
  ${of_ld}/cairo/include/cairo/
  ${of_ld}/curl/include
  ${of_ld}/fmodex/include
  ${of_ld}/freetype/include/freetype2/
  ${of_ld}/glew/include
  ${of_ld}/glfw/include
  ${of_ld}/glm/include
  ${of_ld}/json/include
  ${of_ld}/openssl/include
  ${of_ld}/pugixml/include
  ${of_ld}/rtAudio/include
  ${of_ld}/tess2/include
  ${of_ld}/uriparser/include
  ${of_ld}/utf8/include
  )

list(APPEND of_libs
  ${of_ld}/boost/lib/osx/boost_filesystem.a
  ${of_ld}/boost/lib/osx/boost_system.a
  ${of_ld}/cairo/lib/osx/cairo.a
  ${of_ld}/cairo/lib/osx/cairo-script-interpreter.a
  ${of_ld}/cairo/lib/osx/pixman-1.a
  ${of_ld}/cairo/lib/osx/png.a
  ${of_ld}/curl/lib/osx/curl.a
  ${of_ld}/fmodex/lib/osx/libfmodex.dylib
  ${of_ld}/FreeImage/lib/osx/freeimage.a
  ${of_ld}/freetype/lib/osx/freetype.a
  ${of_ld}/glew/lib/osx/glew.a
  ${of_ld}/glfw/lib/osx/glfw3.a
  ${of_ld}/pugixml/lib/osx/pugixml.a
  ${of_ld}/tess2/lib/osx/tess2.a
  ${of_ld}/uriparser/lib/osx/uriparser.a
  )

# OSX dependencies
find_library(fr_corefoundation CoreFoundation)
find_library(fr_cocoa Cocoa)
find_library(fr_carbon Carbon)
find_library(fr_iokit IOKit)
find_library(fr_coremedia CoreMedia)
find_library(fr_avfoundation AVFoundation)
find_library(fr_corevideo CoreVideo)
find_library(fr_opengl OpenGL)
find_library(fr_accelerate Accelerate)
find_library(fr_ldap LDAP)
find_library(fr_security Security)

list(APPEND of_libs
  ${fr_carbon}
  ${fr_corefoundation}
  ${fr_cocoa}
  ${fr_iokit}
  ${fr_opengl}
  ${fr_avfoundation}
  ${fr_corevideo}
  ${fr_coremedia}
  ${fr_accelerate}
  ${fr_ldap}
  ${fr_security}
  pthread
  z
  )

install(FILES ${of_ld}/fmodex/lib/osx/libfmodex.dylib DESTINATION bin)