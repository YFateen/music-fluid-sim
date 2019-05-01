#include "CGL/CGL.h"
#include "CGL/viewer.h"
// #include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>

#define TINYEXR_IMPLEMENTATION
#include "CGL/tinyexr.h"
// typedef uint32_t gid_t;
#include <iostream>

#include "svg.h"
#include "drawrend.h"
#include "transforms.h"
#include "svgparser.h"
#include "particle.h"

using namespace std;
using namespace CGL;

#define msg(s) cerr << "[Drawer] " << s << endl;


SVG *loadFile( const char* path ) {

  SVG* svg = new SVG();

  if( SVGParser::load( path, svg ) < 0) {
    delete svg;
    return NULL;
  }
  
  return svg;
}

vector<SVG*> loadDirectory( const char* path ) {

  DIR *dir = opendir (path);
  vector<SVG*> svgs;
  if(dir) {
    
    struct dirent *ent; size_t n = 0;
    
    // load files
    string pathname = path; 
    if (pathname[pathname.size()-1] != '/') pathname.push_back('/');
    while (((ent = readdir (dir)) != NULL) && (n < 9)) {

      string filename = ent->d_name;
      string filesufx = filename.substr(filename.find_last_of(".") + 1, 3);
      if (filesufx == "svg" ) {
        cerr << "[Drawer] Loading " << filename << "... "; 
        SVG *svg = loadFile((pathname + filename).c_str());
        if (!svg) {
          cerr << "Failed (Invalid SVG file)" << endl;
        } else {
          cerr << "Succeeded" << endl;
          svgs.push_back(svg);
          n++;
        }
      }
    }

    closedir (dir);

    if (n) {
      msg("Successfully Loaded " << n << " files from " << path);
      return svgs;
    }

    msg("No valid svg files found in " << path);
    return svgs;
  } 

  msg("Could not open directory" << path);
  return svgs;
}

vector<SVG*> loadPath( const char* path ) {

  struct stat st;

  // file exist?
  if(stat(path, &st) < 0 ) {
    msg("File does not exit: " << path);
    return vector<SVG*>();
  }

  // load directory
  if( st.st_mode & S_IFDIR ) {
    return loadDirectory(path);
  } 

  // load file
  if( st.st_mode & S_IFREG ) {
    return vector<SVG*>(1,loadFile(path));
  }

  msg("Invalid path: " << path);
  return vector<SVG*>();
}


int main( int argc, char** argv ) {

  // TODO: get rid of all this svg stuff

//  if (argc < 2) {
//    msg("Not enough arguments. Pass in an .svg or a directory of .svg files.");
//    return 0;
//  }

//  vector<SVG*> svgs(loadPath(argv[1]));
//  if (svgs.empty()) {
//    msg("No svg files successfully loaded. Exiting.");
//    return 0;
//  }

  // create application
  DrawRend app;



  if (argc > 4 && strcmp(argv[2],"nogl") == 0) {
    app.init();
    app.set_gl(false);
    app.resize(stoi(argv[3]), stoi(argv[4]));
    app.write_framebuffer();
    return 0;
  }

  // create viewer
  Viewer viewer = Viewer();

  // set renderer
  viewer.set_renderer(&app);

  // init viewer
  viewer.init();

  // start viewer
  viewer.start();

  exit(EXIT_SUCCESS);

  return 0;

}

//int main() {
//    glewExperimental = true; // Needed for core profile
//    if( !glfwInit() )
//    {
//        fprintf( stderr, "Failed to initialize GLFW\n" );
//        return -1;
//    }
//
//    glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL
//
//    // Open a window and create its OpenGL context
//    GLFWwindow* window; // (In the accompanying source code, this variable is global for simplicity)
//    window = glfwCreateWindow( 1024, 768, "Basic Box", NULL, NULL);
//    if( window == NULL ){
//        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
//        glfwTerminate();
//        return -1;
//    }
//    glfwMakeContextCurrent(window); // Initialize GLEW
//    glewExperimental=true; // Needed in core profile
//    if (glewInit() != GLEW_OK) {
//        fprintf(stderr, "Failed to initialize GLEW\n");
//        return -1;
//    }
//
//    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
//
//    do{
//        // Clear the screen. It's not mentioned before Tutorial 02, but it can cause flickering, so it's there nonetheless.
//        glClear( GL_COLOR_BUFFER_BIT );
//
//        // Draw nothing, see you in tutorial 2 !
//
//
//        // Swap buffers
//        glfwSwapBuffers(window);
//        glfwPollEvents();
//
//    } // Check if the ESC key was pressed or the window was closed
//    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
//          glfwWindowShouldClose(window) == 0 );
//
//}
