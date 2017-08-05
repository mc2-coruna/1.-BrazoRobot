#pragma once

#include "ofMain.h"



class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
    
    
    

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    
    void timers();
    ofColor alertColor1;
    ofColor alertColor2;

    void brazoBotInit(float distanciaEjes, float ancho, float X0, float Y0, float angulo);

    
    
    void drawBrazoBotCam(int x, int y, int w, int h, float wr);
    void drawPersonalCam(int x, int y, int w, int h);
    void drawInstructions(int x, int y, int w, int h);
    
    
    void drawSurface(float x, float y);
    void drawBrazoBotTopView (int x, int y, int width, int height);
    void brazoBotDrawSegment (float X0, float Y0, float distanciaEjes, float ancho, float angulo, ofColor structColor, ofColor jointColor);
    
    void drawLabel (float xAnchor, float yAnchor, float xLabel, float yLabel, char labelText[256], ofColor labelColor, ofColor textColor, ofTrueTypeFont textFont);

    
    
    void brazoBotDelta_phi_ro_h (float d_phi, float d_ro, float d_h, float d_w, float d_wr);

    
    

        void drawBrazoBot2(int x, int y, int w, int h, float phi, float s, float e, float wrist, float wr);

    
    
        void brazoBotStart(float distanciaEjes, float ancho, float X0, float Y0, float angulo);
     //   void brazoBotXYZ_angulos(float d_x, float d_y, float d_z, float d_m, float d_g);

        void brazoBotMove(float phi,float s,float e,float w, float wR, float grip);
        void brazoBotInitBH();
        int degToServoAX(float angulo);
    
    
    // Variables genericas
    ofTrueTypeFont mono09;
    ofTrueTypeFont mono12;
    ofTrueTypeFont mono14;
    ofTrueTypeFont mono16;
    ofTrueTypeFont mono18;
    ofTrueTypeFont mono24;
    ofTrueTypeFont mono36;
    ofTrueTypeFont mono48;

    
    // Graphic enviroment variables
    float escalaH,escalaV,escala;
    unsigned long frameCount;
    
    ofVideoGrabber brazoBotGrabber;
    int  brazoBotCamID = -1;
    bool brazoBotGrabberInit = false;
    bool brazoBotGrabberUpdate = false;
    int  brazoBotGrabberFrameRefres = 15;   //frames between image refreshes
    unsigned long nextRefreshTime;
    
    ofVideoGrabber personalCamGrabber;
    int  personalCamID = -1;
    bool personalCamGrabberInit = false;
    
    ofImage missionImage;
    
    struct joint {
        float angle = 0,      angle_min,    angle_Max;
        float ro_joint = 0,   ro_min,       ro_Max;
        float h_joint = 0,    h_min =-999,  h_Max = 999;

        float angle_draw;
        float angle_ant = 0, angle_post = 0;
        
        float ro_draw,  h_draw;
        float ro_label, h_label;
        
        float x_joint = 0, x_min =-999, x_Max = 999;
        float y_joint = 0, y_min =-999, y_Max = 999;
        ofColor jointColor = ofColor(0,255,0);
        
        char  pos_label[50];
        char  angle_label[50];
        ofColor labelColor = ofColor(0,255,0);
        ofColor textColor  = ofColor(0,255,0);
        
        int   angle_limits = 0;
        int   ro_limits = 0;
        int   h_limits = 0;
        int   x_limits = 0;
        int   y_limits = 0;

        
    };
    
    joint base;
    joint shoulder;
    joint elbow;
    joint wrist;
    joint hand;
    
    
    ofSerial brazoBotSerial;
    int brazoBotSerialID = -1;
    bool brazoBotSerialInited = false;
    ofColor botStrcColor;
    ofColor botShoulderColor;
    ofColor botElbowColor;
    ofColor botWristColor;

    ofColor shoulderAngleColor;

    bool brazoBotInited = false;
    
    //Varaiables del brazo robot
    struct movementBuffer{
        int index=0;
        int indexMax=20;
        unsigned long timeStamp[20] {};
        string command[20]{};
        float  parameter[20] {};
    };
    
    movementBuffer keyBuffer;
    // This is the size of the KEY Buffer
    int keyBufferSize = 2;
    
    
    
    
    
    // int brazoBotMovimiento;
    int brazoBotModo=0;
    float brazoBotDelta = 5.0;
    
    unsigned long lastOrderSeconds;
            

    float brazoBotArmLenght = 155;  // longitud de cada uno de los tramos del brazo en mm
    float brazoBotHandLenght = 145;  // longitud de la mano en mm
    
    
    
    
    

		
};
