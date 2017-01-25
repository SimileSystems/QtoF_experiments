set(bd ${CMAKE_CURRENT_LIST_DIR}/../)
set(of_bd ${bd}/of)
set(of_ld ${of_bd}/libs)
set(of_sd ${of_bd}/libs/openFrameworks)
set(of_ed ${bd}/extern)

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
  ${of_sd}/video/ofVideoGrabber.cpp
  ${of_sd}/video/ofVideoPlayer.cpp
  )

if (WIN32)
  
  list(APPEND of_sources
    ${of_sd}/video/ofDirectShowGrabber.cpp
    ${of_sd}/video/ofDirectShowPlayer.cpp
    )

  include_directories(
    ${of_ld}/videoInput/include
    )

  add_definitions(
    -DPOCO_STATIC
    -DCAIRO_WIN32_STATIC_BUILD
    -DDISABLE_SOME_FLOATING_POINT
    -DUNICODE
    -D_UNICODE
    -DCURL_STATICLIB
    )
  
  #SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_RELEASE} /GL")
  
elseif (APPLE)

  list(APPEND of_sources
    ${of_sd}/video/ofAVFoundationGrabber.mm
    ${of_sd}/video/ofQTKitGrabber.mm
    ${of_sd}/video/ofQTKitMovieRenderer.m
    ${of_sd}/video/ofQtUtils.cpp
    ${of_sd}/video/ofQuickTimeGrabber.cpp
    ${of_sd}/video/ofQuickTimePlayer.cpp
    )
  
  set_source_files_properties(${of_sd}/app/ofAppGLFWWindow.cpp PROPERTIES COMPILE_FLAGS "-x objective-c++")
  set_source_files_properties(${of_sd}/utils/ofSystemUtils.cpp PROPERTIES COMPILE_FLAGS "-x objective-c++")

endif()

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
  ${of_ld}/utf8/include
  )

if (WIN32)

  list(APPEND of_libs
    ${of_ld}/assimp/lib/vs/x64/assimp64.lib
    ${of_ld}/boost/lib/vs/x64/libboost_filesystem-vc140-mt-1_62.lib
    ${of_ld}/boost/lib/vs/x64/libboost_system-vc140-mt-1_62.lib
    ${of_ld}/cairo/lib/vs/x64/cairo-static.lib
    ${of_ld}/cairo/lib/vs/x64/libpng.lib
    ${of_ld}/cairo/lib/vs/x64/pixman-1.lib
    ${of_ld}/cairo/lib/vs/x64/zlib.lib
    ${of_ld}/curl/lib/vs/x64/libcurl.lib
    ${of_ld}/fmodex/lib/vs/x64/fmodex64_vc.lib
    ${of_ld}/FreeImage/lib/vs/x64/FreeImage.lib
    ${of_ld}/freetype/lib/vs/x64/libfreetype.lib 
    ${of_ld}/glew/lib/vs/x64/glew32s.lib
    ${of_ld}/glfw/lib/vs/x64/glfw3.lib
    ${of_ld}/ippicv/lib/vs/x64/ippicvmt.lib
    ${of_ld}/libxml2/lib/vs/x64/libxml2.lib
    ${of_ld}/opencv/lib/vs/x64/Release/libwebp.lib
    ${of_ld}/opencv/lib/vs/x64/Release/opencv_calib3d310.lib
    ${of_ld}/opencv/lib/vs/x64/Release/opencv_core310.lib
    ${of_ld}/opencv/lib/vs/x64/Release/opencv_features2d310.lib
    ${of_ld}/opencv/lib/vs/x64/Release/opencv_flann310.lib
    ${of_ld}/opencv/lib/vs/x64/Release/opencv_highgui310.lib
    ${of_ld}/opencv/lib/vs/x64/Release/opencv_imgcodecs310.lib
    ${of_ld}/opencv/lib/vs/x64/Release/opencv_imgproc310.lib
    ${of_ld}/opencv/lib/vs/x64/Release/opencv_ml310.lib
    ${of_ld}/opencv/lib/vs/x64/Release/opencv_objdetect310.lib
    ${of_ld}/opencv/lib/vs/x64/Release/opencv_photo310.lib
    ${of_ld}/opencv/lib/vs/x64/Release/opencv_shape310.lib
    ${of_ld}/opencv/lib/vs/x64/Release/opencv_stitching310.lib
    ${of_ld}/opencv/lib/vs/x64/Release/opencv_superres310.lib
    ${of_ld}/opencv/lib/vs/x64/Release/opencv_video310.lib
    ${of_ld}/opencv/lib/vs/x64/Release/opencv_videoio310.lib
    ${of_ld}/opencv/lib/vs/x64/Release/opencv_videostab310.lib
    ${of_ld}/opencv/lib/vs/x64/Release/zlib.lib # also in png?
    ${of_ld}/openssl/lib/vs/x64/libeay32md.lib
    ${of_ld}/openssl/lib/vs/x64/ssleay32md.lib
    ${of_ld}/poco/lib/vs/x64/Release/PocoFoundationmd.lib
    ${of_ld}/poco/lib/vs/x64/Release/PocoNetmd.lib
    ${of_ld}/poco/lib/vs/x64/Release/PocoUtilmd.lib
    ${of_ld}/poco/lib/vs/x64/Release/PocoXMLmd.lib
    ${of_ld}/pugixml/lib/vs/x64/pugixml.lib
    ${of_ld}/rtAudio/lib/vs/x64/rtAudio.lib
    ${of_ld}/svgtiny/lib/vs/x64/svgtiny.lib
    ${of_ld}/tess2/lib/vs/x64/tess2.lib
    ${of_ld}/videoInput/lib/vs/x64/VideoInput.lib
    ${of_ed}/lib/vs2015/uriparser.lib
    
    Ws2_32.lib
    Wldap32.lib
    Psapi.lib
    Userenv.lib
    Winmm.lib
    Opengl32.lib
    )

  install(FILES ${of_ld}/assimp/lib/vs/x64/assimp64.dll DESTINATION bin)
  install(FILES ${of_ld}/fmodex/lib/vs/x64/fmodex64.dll DESTINATION bin)
  install(FILES ${of_ld}/FreeImage/lib/vs/x64/FreeImage.dll DESTINATION bin)

elseif (APPLE)

  include_directories(
    ${of_ld}/uriparser/include
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

  find_library(fr_corefoundation CoreFoundation)
  find_library(fr_cocoa Cocoa)
  find_library(fr_carbon Carbon)
  find_library(fr_iokit IOKit)
  find_library(fr_coremedia CoreMedia)
  find_library(fr_avfoundation AVFoundation)
  find_library(fr_qtkit QTKit)
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
    ${fr_qtkit}
    ${fr_corevideo}
    ${fr_coremedia}
    ${fr_accelerate}
    ${fr_ldap}
    ${fr_security}
    pthread
    z
    )

  install(FILES ${of_ld}/fmodex/lib/osx/libfmodex.dylib DESTINATION bin)
  file(COPY ${of_ld}/fmodex/lib/osx/libfmodex.dylib DESTINATION ${CMAKE_BINARY_DIR})
  
endif()

