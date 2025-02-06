#include "TextureBuilder.h"
#include "GLTexture.h"
#include <cstdlib> // Include for rand and srand
#include <ctime>   // Include to seed random number generator with current time
#include <glut.h>
#include <cmath>
#include <vector>
#include <iostream>
#include "Model_3DS.h"
#include <irrKlang.h>
#include <string>
#include <algorithm> // for std::all_of
#include <iterator>  // for std::begin and std::end
using namespace irrklang;
//Global-Variables-General
ISoundEngine* soundEngine = nullptr;
const char* Collecatble_Music = "media/bell.wav";
const char* Trap_Music = "media/Crumbling sound effect.wav";
const char* Unlock_Music = "media/Puzzle_Unlock.wav";
const char* BackGround_Music = "media/Background_Music.wav";
const char* bushSnake_Music = "media/Snake.wav";
const char* boing_Music = "media/boing.wav";
const char* win_Music = "media/win.wav";
const char* lose_Music = "media/lose.wav";

Model_3DS wallModel, PillarModel, BushModel, TrapModel, CoinModel, ArtifactModel, ScrollModel, playerModel, portalModel, fireStickModel,model_tree, model_gemstones, model_statues, model_flowers,model_artifact,riverModel;
GLTexture tex_ground, puzzle_Texture,texground;




#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

//Global-Variables-1stEnvironment
float cameraX1 = 0.0f, cameraY1 = 1.0f, cameraZ1 = 0.0f;
float cameraAngle1 = 0.0f;
int lastBushTrapTime = 0;
int lastArtifactTime = 0;
const int bushTrapInterval = 10000; // 10 seconds in milliseconds
const int artifactInterval = 20000; // 20 seconds in milliseconds
bool wKeyPressed = false; // Flag to check if 'w' is pressed
int cameraMode = 0;
float cameraDistance = 5.0;  // Distance from the camera to the player
float cameraHeight = 2.0;    // Height of the camera above the player
int CurrentScore = 0;
int TargetScore = 100;
bool portalActive = false;
float portalZ = 0.0f; // Portal's Z position
bool puzzleActive = false; // Is the puzzle active?
int jumpCount = 0;         // Number of jumps made
float puzzleZ = 0.0f;      // Z position of the puzzle quad
float puzzleY = 1.0f;// Y position of the puzzle quad (e.g., mid-air)
const int fireStickInterval = 3000; // 15 seconds in milliseconds
int lastFireStickTime = 0;
bool secondEnvironment = false;
float lightIntensity = 0.8f;    // Starting intensity
bool lightIncreasing = true;    // To control whether light is increasing or decreasing
float lightColor[3] = { 1.0f, 1.0f, 1.0f }; // Initial white light
float lightChangeSpeed = 0.01f; // Speed of intensity change



//Global-Variables-2ndEnvironment
int WIDTH = 1280;
int HEIGHT = 720;
GLuint tex;
char title[] = "3D Model Loader Sample";
GLdouble fovy = 45.0;
GLdouble aspectRatio = (GLdouble)WIDTH / (GLdouble)HEIGHT;
GLdouble zNear = 0.1;
GLdouble zFar = 100;
float cameraX2 = 0.0f, cameraY2 = 0.0f, cameraZ2 = 0.0f; // Initial camera position
float cameraAngle2 = 0.0f; // Camera rotation angle
float timeOfDay = 0.0f; // 0.0 to 1.0, where 0.0 is day, and 1.0 is night
bool showGreenBeam = false;  // Flag to show the beam
GLfloat beamPosition[3] = { 0.0f, 0.0f, 0.0f };  // Position of the beam
float beamTimer = 0.0f;       // Timer for the beam duration
const float BEAM_DURATION = 3.0f; // Duration the beam stays active (3 seconds)
bool gameOver = false;
bool gameWon = false;
bool isGameActive = false;
float movementSpeed = 0.5f;
bool isJumping = false;        // Whether the player is in the air
float jumpHeight = 0.0f;       // Current jump height
float gravity = -0.5f;        // Gravity effect
float jumpForce = 1.5f;        // Jump force
float playerAngle = 0.0f;                           // Initial player angle (rotated around Y-axis)
float treePositions[][3] = {
    {-15, 0, -15}, {-5, 0, -15}, {5, 0, -15}, {15, 0, -15},
    {-15, 0, -5}, {-5, 0, -5}, {5, 0, -5}, {15, 0, -5},
    {-15, 0, 5}, {-5, 0, 5}, {5, 0, 5}, {15, 0, 5},
    {-15, 0, 15}, {-5, 0, 15}, {5, 0, 15}, {15, 0, 15}
};
float artifactPosition[3] = { -4.0f, 1.5f, -10.0f };
float riverPositions[][3] = {
    {8, 0.2, 7}, {-4, 0.2, 5}, {3, 0.2, -8}
};// Statue positions remain unchanged
float statuePositions[][3] = {
    {9, 1, 7}, {-6, 1, 5}, {7, 1, -8}, {-7, 1, -7}  // Statue positions remain unchanged
};
float gemstonePositions[][3] = {
    {6.0f, 0.0f, 9.0f}, {-8.0f, 0.0f, 8.0f}, {10.0f, 0.0f, -10.0f}, {-10.0f, 0.0f, -5.0f}  // Gemstone positions remain unchanged
};
float flowerPositions[5][3] = {
    { 5.0f, 1.0f, -12.0f },
    { -9.0f, 1.0f, -11.0f },
    { 12.0f, 1.0f, 10.0f },
    { -5.0f, 1.0f, 14.0f },
    { 8.0f, 1.0f, 3.0f }
};
int playerLives = 5;
bool hasCollectedStatues = false;
bool hasCollectedGemstones = false;
bool artifactSpawned = false;
bool isFirstPerson = true;  // First-person or third-person camera
bool treeCollided[16] = { false };
bool statueCollected[4] = { false };
bool gemstoneCollected[4] = { false };
bool flowerCollected[5] = { false };
bool riverCollided[5] = { false };
// Assuming you have 5 flowers
bool hasCollectedAllCollectibles = false;



//Sound-Engine Fucntions
void initializeSoundEngine() {
    if (!soundEngine) {
        soundEngine = createIrrKlangDevice();
        if (!soundEngine) {
            std::cerr << "Error initializing IrrKlang sound engine!" << std::endl;
            exit(-1);
        }
    }
}

void playMusicWithIrrKlang(const char* filePath, bool loop) {
    if (!soundEngine) {
        std::cerr << "Sound engine not initialized!" << std::endl;
        return;
    }
    soundEngine->play2D(filePath, loop, false, true);
}


void stopAllMusic() {
    if (soundEngine) {
        soundEngine->stopAllSounds();
    }
}

void cleanupSoundEngine() {
    if (soundEngine) {
        soundEngine->drop();  // Releases the engine
        soundEngine = nullptr;
    }
}


void initAllModelsandTextures() {
	//FirstEnvironment-Models
    wallModel.Load("Models/Wall/untitled.3DS");
    PillarModel.Load("Models/2983903.5f33df2bdb5dc/untitled.3DS");
    BushModel.Load("Models/Bush/Bush 3 N030413.3DS");
    TrapModel.Load("Models/Trap/Trap2.3DS");
    CoinModel.Load("Models/Coin/Coin2.3DS");
    ArtifactModel.Load("Models/Mask/untitled.3DS");
    ScrollModel.Load("Models/Scroll2/Scroll2.3DS");
    playerModel.Load("Models/Player2/untitled5.3DS");
    portalModel.Load("Models/dark_portal/untitled3.3DS");
    fireStickModel.Load("Models/Torch/Torch.3DS");
    
	//FirstEnvironment-Textures
    tex_ground.Load("Textures/DirtyGround.bmp");
    puzzle_Texture.Load("Textures/Puzzle.bmp");

	//SecondEnvironment-Models
    model_tree.Load("Models/tree/Tree1.3ds");
    model_gemstones.Load("Models/gemstones/Stones decor N190616.3DS");
    model_statues.Load("Models/statues/Flower and Angel.3DS");
    model_flowers.Load("Models/flowers/Roses bush N271216.3DS");
    model_artifact.Load("Models/artifact/Mausoleum N240618.3ds");
    riverModel.Load("Models/river/river.3ds");

	//SecondEnvironment-Textures
    tex_ground.Load("Textures/ground.bmp");
}


//Init-FirstEnviorment 
void init_FirstEnvironment() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_NORMALIZE);  // Ensure normal vectors do not scale after transformations

    // Updated Ambient Light: Bright enough to make the entire scene visible
    GLfloat ambientLight[] = { 0.8f, 0.8f, 0.8f, 1.0f }; // Brighter ambient light
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);

    // Updated Directional Light: Simulating brighter directional lighting
    GLfloat lightPos0[] = { 1.0f, 2.0f, -1.0f, 0.0f };  // Directional light (w = 0)
    GLfloat diffuse0[] = { 1.0f, 1.0f, 1.0f, 1.0f };    // Brighter diffuse light
    GLfloat specular0[] = { 0.8f, 0.8f, 0.8f, 1.0f };   // Brighter specular highlights
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse0);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular0);

    GLfloat torchDiffuse[] = { 1.0f, 0.8f, 0.6f, 1.0f }; // Warm torchlight color
    GLfloat torchSpecular[] = { 1.0f, 0.8f, 0.6f, 1.0f };
    GLfloat torchAmbient[] = { 0.2f, 0.1f, 0.05f, 1.0f }; // Low ambient for torchlight

    glEnable(GL_LIGHT1);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, torchDiffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, torchSpecular);
    glLightfv(GL_LIGHT1, GL_AMBIENT, torchAmbient);

    // Set spotlight properties
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 30.0f); // Spotlight cone angle
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 10.0f); // Focus intensity
}

//Init-SecondEnviorment
void InitLightSource()
{
    glEnable(GL_LIGHTING);
    //glEnable(GL_LIGHT0);

    GLfloat ambient[] = { 0.1f, 0.1f, 0.1f, 1.0f };
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);

    GLfloat diffuse[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);

    GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

    GLfloat light_position[] = { 0.0f, 10.0f, 0.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
}

// Material Configuration
void InitMaterial()
{
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular);

    GLfloat shininess[] = { 96.0f };
    glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
}

// OpenGL Configuration
void myInit(void)
{
    glClearColor(1.0, 1.0, 1.0, 1.0);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fovy, aspectRatio, zNear, zFar);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    InitLightSource();
    InitMaterial();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    isGameActive = true;
}






//FirstEnvironment-Structs


struct GroundSegment {
    float z;  // Z position of the ground
};

struct WallSegment {
    float z;  // Z position of the wall
};

struct PillarSegment {
    float z;  // Z position of the pillar
};

struct BushSegment {
    float z;  // Z position of the bush
};

struct TrapSegment {
    float z;  // Z position of the trap
};

struct CoinSegment {
    float z;
};

struct ArtifactSegment {
    float z;
};

struct ScrollSegment {
    float z;
};

struct Particle {
    float x, y, z;       // Position
    float vx, vy, vz;    // Velocity
    float lifetime;      // Remaining lifetime
};

struct Spotlight {
    float x, y, z;        // Position of the spotlight
    bool isActive;        // Whether the spotlight is active
    float remainingTime;  // Time remaining for the spotlight to be active
};

struct Player {
    float x = 0.0f, y = 0.0f, z = 0.0f, angle = 0.0f;  // Player's position and facing angle
    bool isJumping = false;
    float velocityY = 0.0f;  // Vertical velocity
    float velocityZ = 0.0f;  // Vertical velocity
    float gravity = -0.1f;  // Gravity value
} player;

struct FireStick {
    float x, y, z;         // Position
    float scale;           // Scale factor
    float animationTime;   // Timer for animation
};
struct FireStickSpotlight {
    float x, y, z;        // Position of the spotlight
    bool isActive;        // Whether the spotlight is active
    float color[3];       // RGB color for the spotlight
};

//FirstEnvironment-Vectors
std::vector<CoinSegment> coinSegments;
std::vector<ArtifactSegment> artifactSegments;
std::vector<ScrollSegment> scrollSegments;
std::vector<FireStick> fireSticks;

std::vector<GroundSegment> groundSegments;
std::vector<WallSegment> wallSegments;
std::vector<PillarSegment> pillarSegments;
std::vector<BushSegment> bushSegments;
std::vector<TrapSegment> trapSegments;
std::vector<Particle> particles; // Active particles

//FirstEnvironment-Intialized_Objects
Spotlight collectibleSpotlight = { 0.0f, 0.0f, 0.0f, false, 0.0f };
FireStickSpotlight fireStickSpotlight = { 0.0f, 0.0f, 0.0f, false, {1.0f, 1.0f, 1.0f} };

//General-Functions
void renderBitmapString(float x, float y, void* font, const char* string) {
    const char* c;
    glRasterPos2f(x, y);
    for (c = string; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
}

void handleResize(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)w / (double)h, 0.1, 500.0);
}



//FirstEnvironment-DrawFunctions

void drawWalls(float z) {
    float wallOffset = 5.0f;
    glPushMatrix();
    glTranslatef(-wallOffset, 0, z);
    glRotatef(90, 0, 1, 0);
    wallModel.Draw();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(wallOffset, 0, z);
    glRotatef(-90, 0, 1, 0);
    wallModel.Draw();
    glPopMatrix();

}
void drawGround(float z) {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tex_ground.texture[0]);
    glColor3f(0.6f, 0.6f, 0.6f);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0); glVertex3f(-5.0f, 0.0f, z);
    glTexCoord2f(1.0, 0.0); glVertex3f(5.0f, 0.0f, z);
    glTexCoord2f(1.0, 1.0); glVertex3f(5.0f, 0.0f, z - 10.0f);
    glTexCoord2f(0.0, 1.0); glVertex3f(-5.0f, 0.0f, z - 10.0f);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}
void drawPillars(float z) {
    float pillarOffsetX = -1.0f;
    glPushMatrix();
    glTranslatef(pillarOffsetX, 0, z);
    glScalef(0.01f, 0.01f, 0.01f);
    glRotatef(90, 0, 1, 0);
    PillarModel.Draw();
    glPopMatrix();
}
void drawBushes(float z) {
    float bushOffsetX = 6.0f;
    glPushMatrix();
    glTranslatef(bushOffsetX - 6, 0, z);
    glScalef(0.01f, 0.01f, 0.01f);
    glRotatef(180, 0, 1, 0);
    BushModel.Draw();
    glPopMatrix();
    //drawPillars(z);
}
void drawTrap(float z) {
    float trapOffsetX = 0.0f;  // Central position for the trap
    glPushMatrix();
    glTranslatef(trapOffsetX, 0.0f, z);
    glScalef(0.1f, 0.1f, 0.1f);  // Adjust scale based on the model's size
    TrapModel.Draw();
    glPopMatrix();
}
void drawCoin(float z) {
    glPushMatrix();
    glTranslatef(0.0f, 2.0f, z);
    glScalef(0.1f, 0.1f, 0.1f);
    glRotatef(90, 0, 1, 0);
    glRotatef(90, 0, 0, 1);
    CoinModel.Draw();
    glPopMatrix();
}
void drawArtifact(float z) {
    glPushMatrix();
    glTranslatef(0.0f, 2.0f, z);  // Offset to the right
    //glScalef(0.01f, 0.01f, 0.01f);
    ArtifactModel.Draw();
    glPopMatrix();
}
void drawScroll(float z) {
    glPushMatrix();
    glTranslatef(0.0f, 2.0f, z);  // Offset to the left
    glScalef(0.1f, 0.1f, 0.1f);
    ScrollModel.Draw();
    glPopMatrix();
}
void drawFireSticks() {
    for (const auto& fireStick : fireSticks) {
        glPushMatrix();
        glTranslatef(fireStick.x, fireStick.y, fireStick.z);
        glScalef(fireStick.scale, fireStick.scale, fireStick.scale);
        fireStickModel.Draw();
        glPopMatrix();
    }

    // Draw the spotlight if active
    if (fireStickSpotlight.isActive) {
        glEnable(GL_LIGHT1);
        GLfloat lightPos[] = { fireStickSpotlight.x, fireStickSpotlight.y, fireStickSpotlight.z, 1.0f };
        GLfloat spotDir[] = { 0.0f, -1.0f, 0.0f }; // Spotlight points downward
        GLfloat lightColor[] = { 1.0f, 0.0f, 0.0f, 1.0f }; // Red spotlight color

        glLightfv(GL_LIGHT1, GL_POSITION, lightPos);
        glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor);
        glLightfv(GL_LIGHT1, GL_SPECULAR, lightColor);
        glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spotDir);
        glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 30.0f);    // Spotlight cone angle
        glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 20.0f);  // Focus intensity

    }
    else {
        glDisable(GL_LIGHT1); // Disable spotlight when not active
    }
}
void drawPuzzleQuad() {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, puzzle_Texture.texture[0]);

    glPushMatrix();
    glTranslatef(0.0f, puzzleY, puzzleZ);

    // Adjust the top height for the quad
    float quadHeight = 4.0f; // Set this value to stretch the quad upwards
    float wallOffset = 5.0f; // Distance of the walls from the center (adjust to match your walls)

    glColor3f(1.0f, 1.0f, 1.0f); // White color to display texture
    glBegin(GL_QUADS);

    // Bottom-left corner
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-wallOffset, -1.0f, 0.0f);

    // Bottom-right corner
    glTexCoord2f(1.0, 0.0);
    glVertex3f(wallOffset, -1.0f, 0.0f);

    // Top-right corner
    glTexCoord2f(1.0, 1.0);
    glVertex3f(wallOffset, quadHeight, 0.0f);

    // Top-left corner
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-wallOffset, quadHeight, 0.0f);

    glEnd();

    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}
void drawPortal() {
    // Calculate the distance between the player and the portal
    float distance = fabs(player.z - portalZ);

    // Define scaling behavior: closer means larger, up to the ideal scale
    float maxScale = 0.1f;    // The ideal scale
    float minScale = 0.02f;   // The starting scale
    float maxDistance = 20.0f; // Distance at which portal reaches ideal scale

    // Calculate the scale factor based on the player's distance
    float scaleFactor = minScale;
    if (distance < maxDistance) {
        scaleFactor = minScale + (1.0f - (distance / maxDistance)) * (maxScale - minScale);
    }

    // Ensure scaling factor does not exceed the ideal scale
    if (scaleFactor > maxScale) {
        scaleFactor = maxScale;
    }

    // Draw the portal with the calculated scale
    glPushMatrix();
    glTranslatef(-0.5f, -1.0f, portalZ); // Position portal at its location
    glScalef(scaleFactor, scaleFactor, scaleFactor); // Dynamically adjust scale
    portalModel.Draw();
    glPopMatrix();
}
void drawParticles() {
    glDisable(GL_LIGHTING); // Disable lighting for particles
    glColor3f(1.0f, 0.5f, 0.0f); // Set color for particles (e.g., orange)

    // Set the size of the particles
    glPointSize(10.0f); // Increase the particle size (default is usually 1.0f)

    glBegin(GL_POINTS);
    for (const auto& p : particles) {
        glVertex3f(p.x, p.y, p.z);
    }
    glEnd();

    glEnable(GL_LIGHTING); // Re-enable lighting
}

//SecondEnviroment-DrawFunctions
void RenderGround()
{
    glEnable(GL_LIGHTING);  // Enable lighting
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tex_ground.texture[0]); // Make sure this texture is loaded properly

    glPushMatrix();
    glBegin(GL_QUADS);
    glNormal3f(0, 1, 0); // Ground normal
    glTexCoord2f(0, 0);
    glVertex3f(-20, 0, -20);
    glTexCoord2f(5, 0);
    glVertex3f(20, 0, -20);
    glTexCoord2f(5, 5);
    glVertex3f(20, 0, 20);
    glTexCoord2f(0, 5);
    glVertex3f(-20, 0, 20);
    glEnd();
    glPopMatrix();

    glDisable(GL_TEXTURE_2D); // Disable texture after rendering the ground
}

void renderGreenBeam() {
    if (showGreenBeam) {
        glPushMatrix();

        // Translate to player's X and Z position, with Y = 0 for ground level
        glTranslatef(cameraX2, 0.0f, cameraZ2);  // Position on the ground at player's feet

        // Set green color for the circle
        glColor3f(0.0f, 1.0f, 0.0f);  // Green color

        // Begin drawing the filled circle (use GL_POLYGON)
        glBegin(GL_POLYGON);

        int numSegments = 30; // Number of segments for the circle
        float radius = 1.0f;  // Circle radius (adjustable)

        // Draw a circle in the XZ plane around the player
        for (int i = 0; i < numSegments; i++) {
            float theta = 2.0f * 3.14159f * float(i) / float(numSegments);  // Angle for each segment
            float x = radius * cosf(theta);  // X position for each point
            float z = radius * sinf(theta);  // Z position for each point
            glVertex3f(x, 0.5f, z);         // Y=0 to keep it on the ground
        }

        glEnd();

        glPopMatrix();
    }
}

void drawGameOverScreen(bool win) {
    // Switch to orthographic projection
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, WIDTH, 0, HEIGHT); // Use the current window dimensions

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // Disable depth testing to render on top of everything
    glDisable(GL_DEPTH_TEST);

    // Draw a background rectangle covering the whole screen
    glColor3f(0.0, 0.0, 0.0); // Set background color (black)
    glBegin(GL_QUADS);
    glVertex2f(0, 0);          // Bottom-left corner
    glVertex2f(WIDTH, 0);      // Bottom-right corner
    glVertex2f(WIDTH, HEIGHT); // Top-right corner
    glVertex2f(0, HEIGHT);     // Top-left corner
    glEnd();

    // Disable lighting for text rendering
    glDisable(GL_LIGHTING);

    // Display the game-over or win message
    glColor3f(1.0, 1.0, 1.0); // Set text color (white)
    const char* message = win ? "Congratulations! You Win!" : "Game Over! You Lose!";
    int messageLength = strlen(message);
    int messageX = (WIDTH / 2) - (messageLength * 9); // Approximation for centering text horizontally
    int messageY = HEIGHT / 2 + 20;                  // Place slightly above center

    glRasterPos2i(messageX, messageY);
    for (const char* c = message; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }

    // Display instructions for quitting
    const char* instructions = "Press ESC to Quit.";
    int instructionsLength = strlen(instructions);
    int instructionsX = (WIDTH / 2) - (instructionsLength * 5); // Center the text horizontally
    int instructionsY = HEIGHT / 2 - 20;                       // Place slightly below center

    glRasterPos2i(instructionsX, instructionsY);
    for (const char* c = instructions; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }
    if (win) {
		playMusicWithIrrKlang(win_Music, false);
    }
    else {
        playMusicWithIrrKlang(lose_Music, false);
    }

    // Re-enable lighting
    glEnable(GL_LIGHTING);

    // Restore previous settings
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    // Re-enable depth testing
    glEnable(GL_DEPTH_TEST);
}




//FirstEnvironment-SpawnFunctions
void spawnParticles(float x, float y, float z) {
    for (int i = 0; i < 50; ++i) { // Adjust the number of particles as needed
        Particle p;
        p.x = x;
        p.y = y;
        p.z = z;
        p.vx = (rand() % 200 - 100) / 1000.0f; // Random velocity X
        p.vy = (rand() % 200) / 500.0f;        // Random velocity Y (upwards)
        p.vz = (rand() % 200 - 100) / 1000.0f; // Random velocity Z
        p.lifetime = 1.0f; // Lifetime of 1 second
        particles.push_back(p);
    }
}

void spawnFireStick(float z) {
    fireSticks.push_back({ -4.0f, 2.0f, z, 0.1f, 0.0f }); // Spawn on the left wall
}


//FirstEnvironment-ActivateFunctions

void activateSpotlight(float x, float y, float z) {
    collectibleSpotlight.x = x;
    collectibleSpotlight.y = y;
    collectibleSpotlight.z = z;
    collectibleSpotlight.isActive = true;
    collectibleSpotlight.remainingTime = 5.0f; // 2 seconds duration
}
void activatePuzzle(float z) {
    puzzleActive = true;
    puzzleZ = z;    // Place puzzle quad in front of the player
    std::cout << "Puzzle activated at z = " << puzzleZ << std::endl;
}
void activateFireStickSpotlight(float x, float y, float z) {
    fireStickSpotlight.x = x;
    fireStickSpotlight.y = y;
    fireStickSpotlight.z = z;
    fireStickSpotlight.isActive = true;

    //std::cout << "Spotlight activated at (" << x << ", " << y << ", " << z << ")" << std::endl; // Debug
}

//FirstEnvironment-UpdateFunctions
void updateCamera() {
    if (cameraMode == 0) { // First-person
        gluLookAt(player.x, player.y + 1.6, player.z,
            player.x + sin(player.angle), player.y + 1.6, player.z - cos(player.angle),
            0.0, 1.0, 0.0);
    }
    else { // Third-person
        float offsetX = sin(player.angle + cameraAngle1) * cameraDistance;
        float offsetZ = cos(player.angle + cameraAngle1) * cameraDistance;
        gluLookAt(player.x + offsetX, player.y + cameraHeight, player.z + offsetZ,
            player.x, player.y + 1.0, player.z,
            0.0, 1.0, 0.0);
    }
}
void updateSpotlight(float deltaTime) {
    if (collectibleSpotlight.isActive) {
        collectibleSpotlight.remainingTime -= deltaTime;
        if (collectibleSpotlight.remainingTime <= 0.0f) {
            collectibleSpotlight.isActive = false; // Turn off the spotlight
        }
    }
}
void updateParticles(float deltaTime) {
    for (auto it = particles.begin(); it != particles.end();) {
        it->x += it->vx;
        it->y += it->vy;
        it->z += it->vz;
        it->vy -= 0.01f; // Simulate gravity
        it->lifetime -= deltaTime;

        if (it->lifetime <= 0) {
            it = particles.erase(it); // Remove expired particles
        }
        else {
            ++it;
        }
    }
}
void updateFireStickSpotlight(float playerX, float playerZ) {
    if (fireStickSpotlight.isActive) {
        float dx = fireStickSpotlight.x - playerX;
        float dz = fireStickSpotlight.z - playerZ;
        float distance = sqrt(dx * dx + dz * dz);

        // Change spotlight color based on distance (closer is redder, farther is greener)
        fireStickSpotlight.color[0] = fmax(0.0f, 1.0f - distance / 10.0f); // Red
        fireStickSpotlight.color[1] = fmin(1.0f, distance / 10.0f);        // Green
        fireStickSpotlight.color[2] = 0.0f;                               // Blue

        // std::cout << "Spotlight color: [" << fireStickSpotlight.color[0] << ", "
           //  << fireStickSpotlight.color[1] << ", "
             //<< fireStickSpotlight.color[2] << "] at distance: " << distance << std::endl; // Debug
    }
}
void updateFireSticks(float deltaTime) {
    for (auto& fireStick : fireSticks) {
        fireStick.animationTime += deltaTime;

        // Oscillate scale around the base size (0.1) with a small amplitude (0.02)
        fireStick.scale = 0.1f + 0.02f * sin(fireStick.animationTime * 2.0f);

        // Check if the player is near the FireStick to activate the spotlight
        if (fabs(player.z - fireStick.z) < 10.0f) {
            activateFireStickSpotlight(fireStick.x, fireStick.y, fireStick.z);
        }
    }
}
void updateTorchlight() {
    // Position the torchlight slightly in front of the player
    GLfloat torchLightPosition[] = {
        player.x + sin(player.angle) * 0.5f,  // Offset in player's direction
        player.y + 1.5f,                      // Slightly above the player's position
        player.z - cos(player.angle) * 0.5f,  // Offset in player's direction
        1.0f                                  // Position light (not directional)
    };

    // Direction of the torchlight
    GLfloat torchLightDirection[] = {
        sin(player.angle),  // Point in the player's facing direction
        -0.5f,              // Slight downward angle
        -cos(player.angle)  // Point in the player's facing direction
    };

    // Update the light position and direction
    glLightfv(GL_LIGHT1, GL_POSITION, torchLightPosition);
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, torchLightDirection);
}

//FirstEnvironment-CollisionFunctions

bool checkCollision1(float playerZ, float trapZ, float playerY, float trapHeight, float zThreshold) {
    float dz = fabs(playerZ - trapZ);  // Calculate the absolute z-distance
    bool isAbove = playerY > trapHeight;  // Check if player is above the trap height
    return dz < zThreshold && !isAbove;  // Must be close in Z and not above the trap
}
bool checkCollision2(float playerX, float playerZ, float objectX, float objectZ, float threshold) {
    float dx = playerX - objectX;
    float dz = playerZ - objectZ;
    return (dz) < (threshold);
}


//SecondEnvironment-CollisionFunctions
bool checkCollision3(float objX, float objY, float objZ, float threshold = 1.0f) {
    float dx = cameraX2 - objX;
    float dz = cameraZ2 - objZ;
    return (sqrt(dx * dx + dz * dz) < threshold);
}

//FirstEnvironment-UpdateGameLogic
void updateFirstEnvironment(int value) {
    float deltaTime = 0.016f; // Assume ~60 FPS for consistent time step
    updateTorchlight();
    if (puzzleActive) {
        // Freeze game updates when the puzzle is active
        glutPostRedisplay();
        glutTimerFunc(16, updateFirstEnvironment, 0); // Keep re-drawing the scene
        return;
    }

    else {



        if (player.isJumping) {
            player.y += player.velocityY;
            player.z += player.velocityZ;
            player.velocityY += player.gravity;

            // Check if the player has landed, adjust to new ground level y = 0
            if (player.y <= 0.0f) {
                player.y = 0.0f;
                player.isJumping = false;
            }
        }

        if (!portalActive && CurrentScore >= TargetScore) {
            portalActive = true;
            portalZ = player.z - 20.0f; // Position portal ahead of the player
            std::cout << "Portal spawned at z = " << portalZ << std::endl;
        }

        // Check collision with the portal
        if (portalActive && checkCollision2(player.x, player.z, 0.0f, portalZ, 1.0f)) {
            std::cout << "Player entered the portal!" << std::endl;
            portalActive = false; // Deactivate the portal
            secondEnvironment = true;
            isGameActive = true;
            glutSwapBuffers();
            return; // Stop further updates
        }
        updateParticles(0.016f);
        updateSpotlight(0.016f);




        // Regular updates for ground and walls
        while (groundSegments.empty() || groundSegments.back().z > player.z - 50.0f) {
            float nextZ = groundSegments.empty() ? player.z : groundSegments.back().z - 10.0f;
            groundSegments.push_back({ nextZ });
        }

        while (wallSegments.empty() || wallSegments.back().z > player.z - 50.0f) {
            float nextZ = wallSegments.empty() ? player.z : wallSegments.back().z - 10.0f;
            wallSegments.push_back({ nextZ });
        }
        while (wallSegments.empty() || wallSegments.back().z > player.z - 50.0f) {
            float nextZ = wallSegments.empty() ? player.z : wallSegments.back().z - 10.0f;
            wallSegments.push_back({ nextZ });
        }
        int currentTime = glutGet(GLUT_ELAPSED_TIME);

        if (currentTime - lastFireStickTime >= fireStickInterval) {
            spawnFireStick(player.z - 30.0f); // Spawn the FireStick ahead of the player
            lastFireStickTime = currentTime;
        }

        updateFireSticks(0.016f);
        updateFireStickSpotlight(player.x, player.z);


        srand(time(NULL));
        int number1 = rand() % 2 + 1;
        int number2 = rand() % 3 + 1;
        if (wKeyPressed) {

            // Check if it's time to spawn a new bush or trap
            if (currentTime - lastBushTrapTime >= bushTrapInterval) {
                float z = player.z - 100.0f; // Adjust this value if needed for your scene scale

                if (number1 == 1) {
                    if (bushSegments.empty() || bushSegments.back().z > z + 50.0f) { // Ensure we're spawning at new locations
                        bushSegments.push_back({ z }); // Spawn a bush
                        pillarSegments.push_back({ z - 2.0f }); // Left pillar
                        pillarSegments.push_back({ z + 2.0f }); // Right pillar
                        lastBushTrapTime = currentTime;
                        std::cout << "Spawned bush and pillars at z = " << z << std::endl;
                    }
                }
                else {
                    if (trapSegments.empty() || trapSegments.back().z > z + 50.0f) {
                        trapSegments.push_back({ z });
                        pillarSegments.push_back({ z - 2.0f }); // Left pillar
                        pillarSegments.push_back({ z + 2.0f }); // Right pillar
                        lastBushTrapTime = currentTime;
                        std::cout << "Spawned trap at z = " << z << std::endl;
                    }
                }
            }

            // Similar logic for artifacts
            if (currentTime - lastArtifactTime >= artifactInterval) {
                float z = player.z - 150.0f; // Adjust this value if needed
                if (number2 == 1) {
                    if (artifactSegments.empty() || artifactSegments.back().z > z + 50.0f) { // Ensure we're spawning at new locations
                        artifactSegments.push_back({ z }); // Choose randomly or cycle through types
                        lastArtifactTime = currentTime;
                        std::cout << "Spawned artifact at z = " << z << std::endl;
                    }
                }
                if (number2 == 2) {
                    if (coinSegments.empty() || coinSegments.back().z > z + 50.0f) { // Ensure we're spawning at new locations
                        coinSegments.push_back({ z }); // Choose randomly or cycle through types
                        lastArtifactTime = currentTime;
                        std::cout << "Spawned Coin at z = " << z << std::endl;
                    }
                }
                if (number2 == 3) {
                    if (scrollSegments.empty() || scrollSegments.back().z > z + 50.0f) { // Ensure we're spawning at new locations
                        scrollSegments.push_back({ z }); // Choose randomly or cycle through types
                        lastArtifactTime = currentTime;
                        std::cout << "Spawned Scroll at z = " << z << std::endl;
                    }
                }
            }
        }

        for (auto it = trapSegments.begin(); it != trapSegments.end();) {
            if (checkCollision1(player.z, it->z, player.y, 0.5f, 1.0f)) { // Assuming trap height is 0.5
                CurrentScore -= 10;// Deduct score for hitting a trap
                playMusicWithIrrKlang(Trap_Music, false);
                spawnParticles(0.0f, 0.5f, it->z); // Spawn particles at trap position
                it = trapSegments.erase(it);  // Correctly remove the trap
            }
            else {
                ++it;
            }
        }

        for (auto it = bushSegments.begin(); it != bushSegments.end();) {
            if (checkCollision1(player.z, it->z, player.y, 0.5f, 1.0f)) {  // Assume bush clearance is also 0.5 units high
                CurrentScore -= 5;  // Deduct score for hitting a bush
                playMusicWithIrrKlang(bushSnake_Music, false);
                it = bushSegments.erase(it);  // Remove the bush
            }
            else {
                ++it;
            }
        }

        // Check collisions with collectibles
        auto checkCollectibleCollision = [&](auto& segments, int scoreIncrease) {
            for (auto it = segments.begin(); it != segments.end(); ) {
                if (checkCollision2(player.x, player.z, 0.0f, it->z, 1.0f)) {
                    CurrentScore += scoreIncrease;
                    activateSpotlight(player.x, player.y + 2.0f, player.z); // Spotlight above the player
                    playMusicWithIrrKlang(Collecatble_Music, false);
                    it = segments.erase(it); // Remove the collectible
                }
                else {
                    ++it;
                }
            }
            };

        checkCollectibleCollision(coinSegments, 10);
        checkCollectibleCollision(artifactSegments, 20);
        checkCollectibleCollision(scrollSegments, 15);





        glutPostRedisplay();
        glutTimerFunc(16, updateFirstEnvironment, 0);
    }
}

//SecondEnvironment-UpdateGameLogic
void updateGameLogic() {
    //myInit();
    // Check collision with trees (trees disappear after collision)
    for (int i = 0; i < 16; i++) {
        if (!treeCollided[i] && checkCollision3(treePositions[i][0], treePositions[i][1], treePositions[i][2])) {
            treeCollided[i] = true;  // Mark tree as collided

            if (playerLives > 0) {
                playerLives--;  // Player loses a life when colliding with a tree
				playMusicWithIrrKlang(boing_Music, false);
            }
        }
    }

    for (int i = 0; i < 3; i++) {
        if (!riverCollided[i] && checkCollision3(riverPositions[i][0], riverPositions[i][1], riverPositions[i][2])) {
            riverCollided[i] = true;  // Mark tree as collided
            if (playerLives > 0) {
                playerLives--;  // Player loses a life when colliding with a tree
                playMusicWithIrrKlang(boing_Music, false);

            }
        }
    }


    // Check collision with statues (collect statues)
    if (!hasCollectedAllCollectibles) {
        for (int i = 0; i < 4; i++) {
            if (!statueCollected[i] && checkCollision3(statuePositions[i][0], statuePositions[i][1], statuePositions[i][2])) {
                playMusicWithIrrKlang(Collecatble_Music, false);
                activateSpotlight(cameraX1, cameraY2 + 2.0f, cameraZ2);
                statueCollected[i] = true;  // Mark statue as collected
                showGreenBeam = true;  // Activate the green beam above the player
                beamPosition[0] = cameraX2;  // Set the beam position to player's position
                beamPosition[1] = cameraY2 + 2.0f;  // Position the beam above the player
                beamPosition[2] = cameraZ2;
                beamTimer = 0.0f;  // Reset timer when item is collected
            }
        }
    }

    // Check collision with gemstones (collect gemstones)
    if (!hasCollectedAllCollectibles) {
        for (int i = 0; i < 4; i++) {  // Loop over all 4 gemstones
            if (!gemstoneCollected[i] && checkCollision3(gemstonePositions[i][0], gemstonePositions[i][1], gemstonePositions[i][2])) {
                playMusicWithIrrKlang(Collecatble_Music, false);
                activateSpotlight(cameraX1, cameraY2 + 2.0f, cameraZ2);
                gemstoneCollected[i] = true;  // Mark gemstone as collected
                showGreenBeam = true;  // Activate the green beam above the player
                beamPosition[0] = cameraX2;  // Set the beam position to player's position
                beamPosition[1] = cameraY2 + 2.0f;  // Position the beam above the player
                beamPosition[2] = cameraZ2;
                beamTimer = 0.0f;  // Reset timer when item is collected
            }

        }
    }


    // Check collision with flowers (collect flowers)
    for (int i = 0; i < 5; i++) {
        if (!flowerCollected[i] && checkCollision3(flowerPositions[i][0], flowerPositions[i][1], flowerPositions[i][2])) {
            playMusicWithIrrKlang(Collecatble_Music, false);
            activateSpotlight(cameraX1, cameraY2 + 2.0f, cameraZ2);
            flowerCollected[i] = true;  // Mark flower as collected
            showGreenBeam = true;  // Activate the green beam above the player
            beamPosition[0] = cameraX2;  // Set the beam position to player's position
            beamPosition[1] = cameraY2 + 2.0f;  // Position the beam above the player
            beamPosition[2] = cameraZ2;
            beamTimer = 0.0f;  // Reset timer when item is collected
        }

    }


    // Check if all statues, flowers, and gemstones are collected, then spawn the artifact
    if (!hasCollectedAllCollectibles &&
        std::all_of(std::begin(statueCollected), std::end(statueCollected), [](bool v) { return v; }) &&
        std::all_of(std::begin(flowerCollected), std::end(flowerCollected), [](bool v) { return v; }) &&
        std::all_of(std::begin(gemstoneCollected), std::end(gemstoneCollected), [](bool v) { return v; })) {

        hasCollectedAllCollectibles = true;  // All collectibles are collected
        artifactSpawned = true;  // Artifact can now spawn
    }


    // Check if the player collects the artifact
    if (artifactSpawned && checkCollision3(artifactPosition[0], artifactPosition[1], artifactPosition[2])) {
        isGameActive = false;
        gameWon = true;
    }

    // Check for game over condition
    if (playerLives <= 0) {
        isGameActive = false;
        gameWon = false;
    }

    if (showGreenBeam) {
        beamTimer += 0.7f;  // Increment timer (this can be adjusted for frame rate)
        if (beamTimer > BEAM_DURATION) {
            showGreenBeam = false;  // Stop showing the beam after the duration
        }
    }
    // Apply gravity when the player is in the air
     // Handle jumping logic (gravity, forward movement, etc.)
    if (isJumping) {
        jumpHeight += jumpForce; // Apply jump force (initial upward velocity)

        if (jumpHeight <= 0.0f) {
            isJumping = false;  // Stop jumping when the player reaches the ground
            jumpHeight = 0.0f;  // Reset jump height to ground level
        }
        else {
            jumpForce += gravity; // Simulate gravity pulling the player down
        }
    }

    // Update player's Y position (cameraY) based on the jump height
    cameraY2 = 1.6f + jumpHeight;

}

//FirstEnviroment-DrawLogic
void drawEnvoirment1() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    updateCamera();
    if (cameraMode == 1) {  // Draw the player in third-person mode
        glPushMatrix();
        glTranslatef(player.x, player.y, player.z);
        glRotatef(-player.angle * 180.0 / M_PI + 180, 0, 1, 0); // Rotate around Y-axis only
        playerModel.Draw();
        glPopMatrix();
    }

    if (portalActive) {
        drawPortal();
    }

    glRotatef(-cameraAngle1, 0.0f, 1.0f, 0.0f);
    glTranslatef(-cameraX1, -cameraY1, -cameraZ1);
    GLfloat lightPos0[] = { 1.0f, 2.0f, -1.0f, 0.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);

    // Draw all elements
    for (auto& segment : groundSegments) {
        drawGround(segment.z);
    }
    for (auto& wall : wallSegments) {
        drawWalls(wall.z);
    }
    for (auto& pillar : pillarSegments) {
        drawPillars(pillar.z);
    }
    for (auto& bush : bushSegments) {
        drawBushes(bush.z);
    }
    for (auto& trap : trapSegments) {
        drawTrap(trap.z);
    }

    for (auto& coin : coinSegments) {
        drawCoin(coin.z);
    }
    for (auto& artifact : artifactSegments) {
        drawArtifact(artifact.z);
    }
    for (auto& scroll : scrollSegments) {
        drawScroll(scroll.z);
    }

    drawParticles();
    drawFireSticks();

    if (puzzleActive) {
        drawPuzzleQuad(); // Draw the puzzle quad if active
    }

    if (collectibleSpotlight.isActive) {
        glEnable(GL_LIGHT1); // Use Light1 for the spotlight
        GLfloat lightPos[] = { collectibleSpotlight.x, collectibleSpotlight.y, collectibleSpotlight.z, 1.0f };
        GLfloat spotDir[] = { 0.0f, -1.0f, 0.0f }; // Spotlight points downward
        GLfloat lightColor[] = { 1.0f, 1.0f, 0.5f, 1.0f }; // Yellowish light

        glLightfv(GL_LIGHT1, GL_POSITION, lightPos);
        glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor);
        glLightfv(GL_LIGHT1, GL_SPECULAR, lightColor);
        glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spotDir);
        glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 30.0f); // Spotlight cone angle
        glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 20.0f); // Focus intensity
    }
    else {
        glDisable(GL_LIGHT1); // Disable spotlight when not active
    }
    // Switch to Ortho view to draw the score
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0.0, glutGet(GLUT_WINDOW_WIDTH), 0.0, glutGet(GLUT_WINDOW_HEIGHT));
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glDisable(GL_LIGHTING);  // Disable lighting for clear text rendering

    // Display the score in the top left corner
    glColor3f(1.0, 1.0, 1.0); // Set text color (white)
    char scoreDisplay[20];
    sprintf(scoreDisplay, "Score: %d", CurrentScore);
    renderBitmapString(10, glutGet(GLUT_WINDOW_HEIGHT) - 20, GLUT_BITMAP_HELVETICA_18, scoreDisplay);

    glEnable(GL_LIGHTING);  // Re-enable lighting
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    glutSwapBuffers();
}

//SecondEnviroment
void myDisplay(void)
{
    if (isGameActive) {
		std::cout << "Game is active" << std::endl;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        timeOfDay += 0.007f;  // Simulate time passing
        if (timeOfDay > 1.0f) timeOfDay = 0.0f; // Loop back to day

        // Update light intensity based on time of day (0.0 = day, 1.0 = night)
        GLfloat lightIntensity[] = { 0.7f * (1.0f - timeOfDay), 0.7f * (1.0f - timeOfDay), 0.7f * (1.0f - timeOfDay), 1.0f };
        glLightfv(GL_LIGHT0, GL_AMBIENT, lightIntensity);

        GLfloat lightPosition[] = { 0.0f, 100.0f, 0.0f, 0.0f };
        // Sky color (white to black)
        float skyColor[] = { 1.0f - timeOfDay, 1.0f - timeOfDay, 1.0f - timeOfDay, 1.0f };
        glClearColor(skyColor[0], skyColor[1], skyColor[2], skyColor[3]);


        glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
        // Apply the new ambient light

        updateGameLogic();

        // Update camera view
        glLoadIdentity();
        if (isFirstPerson) {
            gluLookAt(cameraX2, cameraY2, cameraZ2, cameraX2 + cos(cameraAngle2), cameraY2, cameraZ2 + sin(cameraAngle2), 0.0f, 1.0f, 0.0f);
        }
        else {
            gluLookAt(cameraX2 - 5.0f * cos(cameraAngle2), cameraY2 + 3.0f, cameraZ2 - 5.0f * sin(cameraAngle2),
                cameraX2, cameraY2, cameraZ2,
                0.0f, 1.0f, 0.0f);
        }

        // Draw Ground
        RenderGround();

        // Draw trees if not collided
        glPushMatrix();
        for (int i = 0; i < sizeof(treePositions) / sizeof(treePositions[0]); i++) {
            if (!treeCollided[i]) {
                glPushMatrix();
                glTranslatef(treePositions[i][0], treePositions[i][1], treePositions[i][2]);
                glScalef(0.7, 0.7, 0.7); // Scale trees
                model_tree.Draw();
                glPopMatrix();
            }
        }
        glPopMatrix();

        // Draw gemstones if not collected
        glPushMatrix();
        for (int i = 0; i < 4; i++) {  // Loop over all 4 gemstones
            if (!gemstoneCollected[i]) {
                glPushMatrix();
                glTranslatef(gemstonePositions[i][0], gemstonePositions[i][1], gemstonePositions[i][2]);
                glScalef(0.006f, 0.006f, 0.006f);
                model_gemstones.Draw();
                glPopMatrix();
            }
        }
        glPopMatrix();


        // Draw statues if not collected
        glPushMatrix();
        for (int i = 0; i < sizeof(statuePositions) / sizeof(statuePositions[0]); i++) {
            if (!statueCollected[i]) {
                glPushMatrix();
                glTranslatef(statuePositions[i][0], statuePositions[i][1], statuePositions[i][2]);
                glScalef(0.04f, 0.04f, 0.04f); // Scale statues
                model_statues.Draw();
                glPopMatrix();
            }
        }
        glPopMatrix();

        glPushMatrix();
        for (int i = 0; i < sizeof(flowerPositions) / sizeof(flowerPositions[0]); i++) {
            if (!flowerCollected[i]) {  // Check if the flower is collected
                glPushMatrix();
                glTranslatef(flowerPositions[i][0], flowerPositions[i][1], flowerPositions[i][2]);
                glScalef(0.04f, 0.04f, 0.04f);  // Scale flowers
                model_flowers.Draw();
                glPopMatrix();
            }
        }
        glPopMatrix();

        glPushMatrix();
        for (int i = 0; i < sizeof(riverPositions) / sizeof(riverPositions[0]); i++) {
            if (!riverCollided[i]) {  // Check if the flower is collected
                glPushMatrix();
                glTranslatef(riverPositions[i][0], riverPositions[i][1], riverPositions[i][2]);
                glScalef(0.15f, 0.1f, 0.15f);  // Scale flowers
                riverModel.Draw();
                glPopMatrix();
            }
        }
        glPopMatrix();

        // Draw artifact only after all collectibles are collected
        if (artifactSpawned) {
            glPushMatrix();
            glTranslatef(artifactPosition[0], artifactPosition[1], artifactPosition[2]);
            glScalef(0.02f, 0.02f, 0.02f); // Scale artifact
            model_artifact.Draw();
            glPopMatrix();
        }

        renderGreenBeam();
        activateSpotlight(cameraX1, cameraY2 + 2.0f, cameraZ2);

        // Draw player model if not in first-person view
        if (!isFirstPerson) {
            glPushMatrix();
            glTranslatef(cameraX2, cameraY2, cameraZ2);
            glRotatef(playerAngle * 180.0f / 3.14159f, 0.0f, 1.0f, 0.0f);  // Rotate player with camera
            glScalef(0.9f, 0.9f, 0.9f);
            playerModel.Draw();
            glPopMatrix();
        }

        // Switch to Ortho view to draw the score
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        gluOrtho2D(0.0, glutGet(GLUT_WINDOW_WIDTH), 0.0, glutGet(GLUT_WINDOW_HEIGHT));
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();
        glDisable(GL_LIGHTING);  // Disable lighting for clear text rendering

        // Display the score in the top left corner
        glColor3f(1.0, 1.0, 1.0); // Set text color (white)
        char scoreDisplay[20];
        sprintf(scoreDisplay, "Lives: %d", playerLives);
        renderBitmapString(10, glutGet(GLUT_WINDOW_HEIGHT) - 20, GLUT_BITMAP_HELVETICA_18, scoreDisplay);

        glEnable(GL_LIGHTING);  // Re-enable lighting
        glPopMatrix();
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
    }

    else {
        if (gameWon) {
            drawGameOverScreen(true);
        }
        else {
            drawGameOverScreen(false);
        }
    }

    // Swap buffers
    glutSwapBuffers();
}


//Keypress&Mouse
void handleKeypress(unsigned char key, int x, int y) {
    if (secondEnvironment == false) {
        if (key == 'w') {
            if (!puzzleActive) {
                player.z -= 0.1;
                wKeyPressed = true;

            }
        }
        if (key == 's') {
            if (!puzzleActive) {
                player.z += 0.1;
            }

        }
        if (key == 'p' && !puzzleActive) { // Press 'P' to spawn the puzzle
            activatePuzzle(player.z - 10.0f); // Place puzzle 10 units in front of the player
        }
        if (key == 'e' && puzzleActive) {
            playMusicWithIrrKlang(Collecatble_Music, false);
            puzzleActive = false;  // Deactivate the puzzle
            std::cout << "Puzzle deactivated!" << std::endl;
        }

    }
    else {
        switch (key) {
        case 27: // ESCAPE key
            exit(0);
        case 'd': // Rotate camera and move player to the right in third person
            if (isFirstPerson) {
                cameraAngle2 += 0.2f;
                // Rotate camera to the right
            }
            else {
                cameraAngle2 += 0.2f;
                playerAngle += 2.0f;             // Rotate the player to the right in third-person
            }
            break;
        case 'a': // Rotate camera and move player to the left in third person
            if (isFirstPerson) {
                cameraAngle2 -= 0.2f;  // Rotate camera to the left
            }
            else {
                cameraAngle2 -= 0.2f;
                playerAngle -= 2.0f;// Rotate the player to the left in third-person
            }
            break;
        case 'w': // Move player forward
            if (isFirstPerson) {
                cameraX2 += movementSpeed * cos(cameraAngle2);
                cameraZ2 += movementSpeed * sin(cameraAngle2);
            }
            else {
                cameraX2 += movementSpeed * cos(cameraAngle2);  // Move player forward
                cameraZ2 += movementSpeed * sin(cameraAngle2);
            }
            break;
        case 's': // Move player backward
            if (isFirstPerson) {
                cameraX2 -= movementSpeed * cos(cameraAngle2);
                cameraZ2 -= movementSpeed * sin(cameraAngle2);
            }
            else {
                cameraX2 -= movementSpeed * cos(cameraAngle2);  // Move player backward
                cameraZ2 -= movementSpeed * sin(cameraAngle2);
            }
            break;
        
        }

    }
    glutPostRedisplay();

}
void handleKeyRelease(unsigned char key, int x, int y) {
    if (secondEnvironment == false) {
        if (key == 'w') {
            wKeyPressed = false;
        }
    }
}
void handleMouse(int button, int state, int x, int y) {

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        if (secondEnvironment == false) {


            if (!player.isJumping) { // Initiate jump if not already jumping
                player.isJumping = true;
                player.velocityY = 1.0;// Initial jump velocity, adjust as needed
                player.velocityZ = -0.5;

            }
        }
        else {
            if (!isJumping) {  // Start jump if not already jumping
                isJumping = true;
                jumpForce = 1.5f;  // Reset jump force when jumping starts
            }
        }
    }
    else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        if (secondEnvironment == false) {
            cameraMode = 1 - cameraMode;  // Toggle camera mode
        }
        else {
            isFirstPerson = !isFirstPerson;
        }
        glutPostRedisplay();
    }
}

void update(int value) {
    if (secondEnvironment == false) {
        updateFirstEnvironment(value);
    }
    else {
        updateGameLogic();
    }
}

void display() {
	if (secondEnvironment == false) {
		drawEnvoirment1();
	}
	else {
        //myInit();
        //glDisable(GL_LIGHT1);
		myDisplay();
	}
}





int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Mystic Explorer");
    initAllModelsandTextures();
    init_FirstEnvironment();
    glutDisplayFunc(display);
    glutKeyboardFunc(handleKeypress);
    glutKeyboardUpFunc(handleKeyRelease); // Register the key release function
    glutMouseFunc(handleMouse);
    glutReshapeFunc(handleResize);
    glutTimerFunc(16, update, 0);
    initializeSoundEngine();
    playMusicWithIrrKlang(BackGround_Music, true);
    glutMainLoop();
    cleanupSoundEngine();
    return 0;
}
