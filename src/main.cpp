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
#include <fstream>

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <ctime>

#include "svg.h"
#include "drawrend.h"
#include "transforms.h"
#include "svgparser.h"
#include "particle.h"

using namespace std;
using namespace CGL;

#define msg(s) cerr << "[Drawer] " << s << endl;

float start;

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

void load_data(const char* path, int* sample_rate, vector<uint8_t>* signal) {
  char buffer[1024];
  char *answer = getcwd(buffer, sizeof(buffer));
  string cwd;
  if (answer) cwd = answer;
  string full_path = cwd + "/" + string(path);
  cout << full_path << endl;
  ifstream data_file(full_path, ios::in | ios::binary);
  int data_size;
  if (data_file && data_file.read((char*) sample_rate, 4) && data_file.read((char*) &data_size, 4)) {
    cout << "sr=" << *sample_rate << " l=" << data_size << endl;
    signal->resize(data_size);
    data_file.read((char*) &((*signal)[0]), data_size);
  }
  data_file.close();
}

void start_audio(const char* path) {
  // macOS's AVAudioPlayer is probably the move here, but I spent 3 hours trying to install/use
  // random audio libraries. Obviously this does not work on Windows, but otherwise has few
  // functional drawbacks
  char buffer[1024];
  char *answer = getcwd(buffer, sizeof(buffer));
  string cwd;
  if (answer) cwd = answer;
  string full_path = "afplay " + cwd + "/" + string(path) + " &";
  cout << full_path << endl;
  system("killall afplay");
  system(full_path.c_str());
  atexit([] () {system("killall afplay");});
}

int main( int argc, char** argv ) {
  if (argc < 3) {
    msg("Usage: ./draw <audio file> <data file>")
    msg("Example: ./draw res/test_song/test_song.mp3 res/test_song/test_song_8b_10Hz")
    return 0;
  }

  // Placeholder inputs
  int sample_rate;
  std::vector<uint8_t> audio_signal;

  load_data(argv[2], &sample_rate, &audio_signal);
  start_audio(argv[1]);
  struct timespec ts{};
  clock_gettime(CLOCK_MONOTONIC, &ts);
  start = ts.tv_sec + ts.tv_nsec * 1.0e-9;

    int debug = 0;
    
  // create application
  DrawRend app(sample_rate, audio_signal);



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
